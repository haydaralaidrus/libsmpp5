#include <stdio.h>
#include <string.h>

#include "pdu.h"
#include "smpp.h"
#include "tests.h"

static int
test_spec_constants(void)
{
	CHECK(SMPP_CMD_SUBMIT_SM == 0x00000004u);
	CHECK(SMPP_CMD_SUBMIT_SM_RESP == 0x80000004u);
	CHECK(SMPP_CMD_DELIVER_SM_RESP == 0x80000005u);
	CHECK(SMPP_CMD_BIND_TRANSCEIVER_RESP == 0x80000009u);
	CHECK(SMPP_ESME_ROK == 0);
	CHECK(SMPP_ESME_RINVCMDID == 0x00000003u);
	CHECK(SMPP_ESME_RINVBCASTCHANIND == 0x00000112u);
	CHECK(SMPP_TAG_MESSAGE_PAYLOAD == 0x0424u);
	CHECK(SMPP_TAG_SC_INTERFACE_VERSION == 0x0210u);
	CHECK(SMPP_TAG_ITS_SESSION_INFO == 0x1383u);
	return 0;
}

static int
test_bind_round_trip(void)
{
	uint8_t wire[128];
	smpp_bind_t in = { 0 };
	in.system_id = "myesme";
	in.password = "secret12";
	in.system_type = NULL;
	in.interface_version = SMPP_INTERFACE_VERSION_5_0;
	in.addr_ton = SMPP_TON_INTERNATIONAL;
	in.addr_npi = SMPP_NPI_ISDN;
	in.address_range = "";

	size_t needed = smpp_bind_encoded_length(&in);
	CHECK(smpp_bind_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_bind_t out;
	CHECK(smpp_bind_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.system_id, "myesme") == 0);
	CHECK(strcmp(out.password, "secret12") == 0);
	CHECK(strcmp(out.system_type, "") == 0);
	CHECK(out.interface_version == SMPP_INTERFACE_VERSION_5_0);
	CHECK(out.addr_ton == SMPP_TON_INTERNATIONAL);
	CHECK(out.addr_npi == SMPP_NPI_ISDN);
	CHECK(strcmp(out.address_range, "") == 0);
	return 0;
}

static int
test_bind_resp_round_trip_with_tlv(void)
{
	uint8_t tlv_blob[8];
	size_t tlv_offset = 0;
	uint8_t version_value = SMPP_INTERFACE_VERSION_5_0;
	CHECK(smpp_tlv_write(tlv_blob, sizeof(tlv_blob), &tlv_offset,
						 SMPP_TAG_SC_INTERFACE_VERSION, &version_value,
						 1) == SMPP_OK);

	uint8_t wire[64];
	smpp_bind_resp_t in = { 0 };
	in.system_id = "themc";
	in.tlvs = tlv_blob;
	in.tlvs_length = (uint16_t)tlv_offset;

	size_t needed = smpp_bind_resp_encoded_length(&in);
	CHECK(needed == strlen("themc") + 1 + tlv_offset);
	CHECK(smpp_bind_resp_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_bind_resp_t out;
	CHECK(smpp_bind_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.system_id, "themc") == 0);
	CHECK(out.tlvs_length == tlv_offset);

	smpp_tlv_t tlv;
	CHECK(smpp_tlv_find(out.tlvs, out.tlvs_length,
						SMPP_TAG_SC_INTERFACE_VERSION, &tlv) == 1);
	CHECK(tlv.length == 1);
	CHECK(tlv.value[0] == SMPP_INTERFACE_VERSION_5_0);
	return 0;
}

static int
test_submit_sm_round_trip(void)
{
	const uint8_t payload[] = "Hello, world!";
	uint8_t wire[256];
	smpp_sm_t in = { 0 };
	in.service_type = "";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";
	in.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	in.dest_addr_npi = SMPP_NPI_ISDN;
	in.destination_addr = "15559876543";
	in.esm_class = SMPP_ESM_CLASS_DEFAULT;
	in.registered_delivery = SMPP_REGISTERED_DELIVERY_SUCCESS_OR_FAILURE;
	in.replace_if_present_flag = SMPP_REPLACE_IF_PRESENT_NO;
	in.data_coding = SMPP_DATA_CODING_IA5;
	in.short_message = payload;
	in.short_message_length = (uint8_t)(sizeof(payload) - 1);

	size_t needed = smpp_sm_encoded_length(&in);
	CHECK(smpp_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	uint8_t oversized[512];
	CHECK(smpp_sm_encode(&in, oversized, sizeof(oversized)) == SMPP_OK);
	CHECK(memcmp(wire, oversized, needed) == 0);

	smpp_sm_t out;
	CHECK(smpp_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.source_addr, "15551234567") == 0);
	CHECK(strcmp(out.destination_addr, "15559876543") == 0);
	CHECK(out.registered_delivery ==
		  SMPP_REGISTERED_DELIVERY_SUCCESS_OR_FAILURE);
	CHECK(out.short_message_length == sizeof(payload) - 1);
	CHECK(memcmp(out.short_message, payload, sizeof(payload) - 1) == 0);
	CHECK(out.tlvs_length == 0);
	CHECK(out.tlvs == NULL);
	return 0;
}

static int
test_submit_sm_with_absolute_time_and_tlv(void)
{
	uint8_t tlv_blob[16];
	size_t off = 0;
	uint16_t ref = 42;
	uint8_t ref_be[2] = { (uint8_t)(ref >> 8), (uint8_t)ref };
	CHECK(smpp_tlv_write(tlv_blob, sizeof(tlv_blob), &off,
						 SMPP_TAG_USER_MESSAGE_REFERENCE, ref_be,
						 2) == SMPP_OK);

	uint8_t wire[256];
	smpp_sm_t in = { 0 };
	in.destination_addr = "1000";
	in.schedule_delivery_time = "251231235959000R";
	in.validity_period = "";
	in.tlvs = tlv_blob;
	in.tlvs_length = (uint16_t)off;

	size_t needed = smpp_sm_encoded_length(&in);
	CHECK(smpp_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_sm_t out;
	CHECK(smpp_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.schedule_delivery_time, "251231235959000R") == 0);
	CHECK(strcmp(out.validity_period, "") == 0);

	smpp_tlv_t tlv;
	CHECK(smpp_tlv_find(out.tlvs, out.tlvs_length,
						SMPP_TAG_USER_MESSAGE_REFERENCE, &tlv) == 1);
	CHECK(tlv.value[0] == ref_be[0] && tlv.value[1] == ref_be[1]);
	return 0;
}

static int
test_sm_resp_round_trip(void)
{
	uint8_t wire[64];
	smpp_sm_resp_t in = { 0 };
	in.message_id = "MC-00012345";

	size_t needed = smpp_sm_resp_encoded_length(&in);
	CHECK(smpp_sm_resp_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_sm_resp_t out;
	CHECK(smpp_sm_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "MC-00012345") == 0);
	CHECK(out.tlvs == NULL && out.tlvs_length == 0);
	return 0;
}

static int
test_encode_validation_errors(void)
{
	uint8_t wire[128];

	smpp_bind_t bad_bind = { 0 };
	char long_id[17];
	memset(long_id, 'a', 16);
	long_id[16] = '\0';
	bad_bind.system_id = long_id;
	CHECK(smpp_bind_encode(&bad_bind, wire, sizeof(wire)) == SMPP_ERR_INVALID);

	smpp_sm_t bad_sm = { 0 };
	bad_sm.schedule_delivery_time = "12345";
	CHECK(smpp_sm_encode(&bad_sm, wire, sizeof(wire)) == SMPP_ERR_INVALID);

	smpp_bind_t ok_bind = { 0 };
	ok_bind.system_id = "x";
	uint8_t tiny[3];
	CHECK(smpp_bind_encode(&ok_bind, tiny, sizeof(tiny)) ==
		  SMPP_ERR_BUFFER_TOO_SMALL);
	return 0;
}

static int
test_decode_validation_errors(void)
{
	smpp_bind_t bind;
	uint8_t no_nul[40];
	memset(no_nul, 'a', sizeof(no_nul));
	CHECK(smpp_bind_decode(no_nul, sizeof(no_nul), &bind) == SMPP_ERR_INVALID);

	uint8_t truncated[3] = { 'a', 0x00, 0x00 };
	CHECK(smpp_bind_decode(truncated, sizeof(truncated), &bind) ==
		  SMPP_ERR_INVALID);
	return 0;
}

static int
test_tlv_iteration(void)
{
	uint8_t blob[32];
	size_t off = 0;
	const uint8_t v1[] = { 0xAA };
	const uint8_t v2[] = { 0xBB, 0xCC };
	CHECK(smpp_tlv_write(blob, sizeof(blob), &off, 0x0001, v1, 1) == SMPP_OK);
	CHECK(smpp_tlv_write(blob, sizeof(blob), &off, 0x0002, v2, 2) == SMPP_OK);

	const uint8_t *cursor = blob;
	const uint8_t *end = blob + off;
	smpp_tlv_t tlv;

	CHECK(smpp_tlv_next(&cursor, end, &tlv) == SMPP_TLV_OK);
	CHECK(tlv.tag == 0x0001 && tlv.length == 1 && tlv.value[0] == 0xAA);
	CHECK(smpp_tlv_next(&cursor, end, &tlv) == SMPP_TLV_OK);
	CHECK(tlv.tag == 0x0002 && tlv.length == 2);
	CHECK(smpp_tlv_next(&cursor, end, &tlv) == SMPP_TLV_END);
	CHECK(cursor == end);

	/* length says 10, only 2 octets remain: ESME_RINVTLVSTREAM (4.7.6) */
	uint8_t malformed[] = { 0x00, 0x01, 0x00, 0x0A, 0xAA, 0xBB };
	const uint8_t *bad_cursor = malformed;
	CHECK(smpp_tlv_next(&bad_cursor, malformed + sizeof(malformed), &tlv) ==
		  SMPP_TLV_MALFORMED);
	CHECK(smpp_tlv_find(malformed, sizeof(malformed), 0x0001, &tlv) == 0);
	return 0;
}

static int
test_full_pipeline_through_pdu_layer(void)
{
	smpp_sm_t sm = { 0 };
	sm.destination_addr = "447700900123";
	sm.data_coding = SMPP_DATA_CODING_UCS2;
	const uint8_t msg[] = "hi";
	sm.short_message = msg;
	sm.short_message_length = (uint8_t)(sizeof(msg) - 1);

	uint8_t body[128];
	CHECK(smpp_sm_encode(&sm, body, sizeof(body)) == SMPP_OK);

	pdu_t pdu = { 0 };
	pdu.header.command_id = SMPP_CMD_SUBMIT_SM;
	pdu.header.sequence_number = 7;
	pdu.body = body;
	pdu.body_length = (uint32_t)smpp_sm_encoded_length(&sm);

	uint8_t wire[256];
	CHECK(pdu_encode(&pdu, wire, sizeof(wire)) == PDU_OK);

	pdu_t decoded_pdu;
	CHECK(pdu_decode(wire, pdu_encoded_length(&pdu), &decoded_pdu) == PDU_OK);
	CHECK(decoded_pdu.header.command_id == SMPP_CMD_SUBMIT_SM);
	CHECK(decoded_pdu.header.sequence_number == 7);

	smpp_sm_t decoded_sm;
	CHECK(smpp_sm_decode(decoded_pdu.body, decoded_pdu.body_length,
						 &decoded_sm) == SMPP_OK);
	CHECK(strcmp(decoded_sm.destination_addr, "447700900123") == 0);
	CHECK(decoded_sm.data_coding == SMPP_DATA_CODING_UCS2);
	CHECK(memcmp(decoded_sm.short_message, msg, sizeof(msg) - 1) == 0);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_spec_constants();
	failures += test_bind_round_trip();
	failures += test_bind_resp_round_trip_with_tlv();
	failures += test_submit_sm_round_trip();
	failures += test_submit_sm_with_absolute_time_and_tlv();
	failures += test_sm_resp_round_trip();
	failures += test_encode_validation_errors();
	failures += test_decode_validation_errors();
	failures += test_tlv_iteration();
	failures += test_full_pipeline_through_pdu_layer();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
