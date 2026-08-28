#include "pdu.h"

#include <stdio.h>
#include <string.h>

#define CHECK(cond)                                                         \
	do {                                                                    \
		if (!(cond)) {                                                      \
			fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			return 1;                                                       \
		}                                                                   \
	} while (0)

static int
test_spec_byte_order_example(void)
{
	/* Spec section 3.1: 31022623 decimal encodes as octets 01 D9 5E 1F. */
	uint8_t buf[PDU_HEADER_LENGTH] = { 0 };
	pdu_t pdu = { 0 };
	pdu.header.command_id = 31022623u;
	CHECK(pdu_encode(&pdu, buf, sizeof(buf)) == PDU_OK);
	CHECK(buf[4] == 0x01 && buf[5] == 0xD9 && buf[6] == 0x5E && buf[7] == 0x1F);
	return 0;
}

static int
test_round_trip_with_body(void)
{
	const uint8_t body[] = "system_id\0password\0";
	uint8_t wire[64];
	pdu_t in = { 0 };
	in.header.command_id = 0x00000002u; /* bind_transmitter */
	in.header.sequence_number = 42;
	in.body = body;
	in.body_length = sizeof(body);

	size_t needed = pdu_encoded_length(&in);
	CHECK(needed == PDU_HEADER_LENGTH + sizeof(body));
	CHECK(pdu_encode(&in, wire, sizeof(wire)) == PDU_OK);

	pdu_t out;
	CHECK(pdu_decode(wire, needed, &out) == PDU_OK);
	CHECK(out.header.command_length == needed);
	CHECK(out.header.command_id == in.header.command_id);
	CHECK(out.header.sequence_number == in.header.sequence_number);
	CHECK(out.body_length == in.body_length);
	CHECK(memcmp(out.body, body, sizeof(body)) == 0);
	CHECK(out.body == wire + PDU_HEADER_LENGTH); /* zero-copy alias */
	return 0;
}

static int
test_header_only_pdu(void)
{
	/* e.g. enquire_link: no body at all. */
	uint8_t wire[PDU_HEADER_LENGTH];
	pdu_t in = { 0 };
	in.header.command_id = 0x00000015u;
	in.header.sequence_number = 7;

	CHECK(pdu_encoded_length(&in) == PDU_HEADER_LENGTH);
	CHECK(pdu_encode(&in, wire, sizeof(wire)) == PDU_OK);

	pdu_t out;
	CHECK(pdu_decode(wire, sizeof(wire), &out) == PDU_OK);
	CHECK(out.body == NULL);
	CHECK(out.body_length == 0);
	return 0;
}

static int
test_incomplete_buffer(void)
{
	uint8_t short_buf[10] = { 0 };
	pdu_t out;
	CHECK(pdu_decode(short_buf, sizeof(short_buf), &out) == PDU_ERR_INCOMPLETE);

	/* Header claims a 20-octet PDU, but only 16 octets are present. */
	uint8_t partial[PDU_HEADER_LENGTH] = { 0 };
	partial[3] = 20; /* command_length = 20, big-endian */
	CHECK(pdu_decode(partial, sizeof(partial), &out) == PDU_ERR_INCOMPLETE);
	return 0;
}

static int
test_invalid_length(void)
{
	uint8_t buf[PDU_HEADER_LENGTH] = { 0 };
	buf[3] = 10; /* command_length = 10, smaller than the 16-octet header */
	pdu_t out;
	CHECK(pdu_decode(buf, sizeof(buf), &out) == PDU_ERR_INVALID_LENGTH);
	return 0;
}

static int
test_buffer_too_small_on_encode(void)
{
	uint8_t tiny[4];
	pdu_t pdu = { 0 };
	CHECK(pdu_encode(&pdu, tiny, sizeof(tiny)) == PDU_ERR_BUFFER_TOO_SMALL);
	return 0;
}

static int
test_two_pdus_back_to_back(void)
{
	uint8_t wire[2 * PDU_HEADER_LENGTH];
	pdu_t a = { 0 };
	a.header.command_id = 1;
	a.header.sequence_number = 1;
	pdu_t b = { 0 };
	b.header.command_id = 2;
	b.header.sequence_number = 2;

	CHECK(pdu_encode(&a, wire, PDU_HEADER_LENGTH) == PDU_OK);
	CHECK(pdu_encode(&b, wire + PDU_HEADER_LENGTH, PDU_HEADER_LENGTH) ==
		  PDU_OK);

	pdu_t out_a, out_b;
	CHECK(pdu_decode(wire, sizeof(wire), &out_a) == PDU_OK);
	CHECK(out_a.header.command_length == PDU_HEADER_LENGTH);
	CHECK(out_a.header.sequence_number == 1);

	size_t consumed = out_a.header.command_length;
	CHECK(pdu_decode(wire + consumed, sizeof(wire) - consumed, &out_b) ==
		  PDU_OK);
	CHECK(out_b.header.sequence_number == 2);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_spec_byte_order_example();
	failures += test_round_trip_with_body();
	failures += test_header_only_pdu();
	failures += test_incomplete_buffer();
	failures += test_invalid_length();
	failures += test_buffer_too_small_on_encode();
	failures += test_two_pdus_back_to_back();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}

	printf("All tests passed\n");
	return 0;
}
