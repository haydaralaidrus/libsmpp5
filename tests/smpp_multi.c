/*
 * * Copyright (C) 2026 Haydar Alaidrus <haydar.alaidrus@gmail.com>
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU Lesser General Public License as published by
 ** the Free Software Foundation; either version 2.1 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <string.h>

#include "libsmpp5/smpp.h"
#include "libsmpp5/smpp_tlv.h"
#include "tests.h"

static int
test_dest_address_iteration(void)
{
	uint8_t blob[64];
	size_t off = 0;

	smpp_dest_address_t sme = { 0 };
	sme.dest_flag = SMPP_DEST_FLAG_SME_ADDRESS;
	sme.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	sme.dest_addr_npi = SMPP_NPI_ISDN;
	sme.destination_addr = "15551234567";
	CHECK(smpp_dest_address_write(blob, sizeof(blob), &off, &sme) ==
		  SMPP_LIST_OK);

	smpp_dest_address_t dl = { 0 };
	dl.dest_flag = SMPP_DEST_FLAG_DISTRIBUTION_LIST;
	dl.dl_name = "sales-team";
	CHECK(smpp_dest_address_write(blob, sizeof(blob), &off, &dl) ==
		  SMPP_LIST_OK);

	const uint8_t *cursor = blob;
	const uint8_t *end = blob + off;
	smpp_dest_address_t entry;

	CHECK(smpp_dest_address_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(entry.dest_flag == SMPP_DEST_FLAG_SME_ADDRESS);
	CHECK(strcmp(entry.destination_addr, "15551234567") == 0);

	CHECK(smpp_dest_address_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(entry.dest_flag == SMPP_DEST_FLAG_DISTRIBUTION_LIST);
	CHECK(strcmp(entry.dl_name, "sales-team") == 0);

	CHECK(smpp_dest_address_next(&cursor, end, &entry) == SMPP_LIST_END);
	CHECK(cursor == end);
	return 0;
}

static int
test_unsuccess_sme_iteration(void)
{
	uint8_t blob[64];
	size_t off = 0;

	smpp_unsuccess_sme_t a = { 0 };
	a.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	a.dest_addr_npi = SMPP_NPI_ISDN;
	a.destination_addr = "15551234567";
	a.error_status_code = SMPP_ESME_RSUBMITFAIL;
	CHECK(smpp_unsuccess_sme_write(blob, sizeof(blob), &off, &a) ==
		  SMPP_LIST_OK);

	smpp_unsuccess_sme_t b = { 0 };
	b.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	b.dest_addr_npi = SMPP_NPI_ISDN;
	b.destination_addr = "15559876543";
	b.error_status_code = SMPP_ESME_RTHROTTLED;
	CHECK(smpp_unsuccess_sme_write(blob, sizeof(blob), &off, &b) ==
		  SMPP_LIST_OK);

	const uint8_t *cursor = blob;
	const uint8_t *end = blob + off;
	smpp_unsuccess_sme_t entry;

	CHECK(smpp_unsuccess_sme_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(strcmp(entry.destination_addr, "15551234567") == 0);
	CHECK(entry.error_status_code == SMPP_ESME_RSUBMITFAIL);

	CHECK(smpp_unsuccess_sme_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(strcmp(entry.destination_addr, "15559876543") == 0);
	CHECK(entry.error_status_code == SMPP_ESME_RTHROTTLED);

	CHECK(smpp_unsuccess_sme_next(&cursor, end, &entry) == SMPP_LIST_END);
	CHECK(cursor == end);
	return 0;
}

static int
test_submit_multi_round_trip_mixed_destinations(void)
{
	const uint8_t payload[] = "Hello, everyone!";

	uint8_t dest_blob[64];
	size_t dest_off = 0;
	smpp_dest_address_t d1 = { 0 };
	d1.dest_flag = SMPP_DEST_FLAG_SME_ADDRESS;
	d1.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	d1.dest_addr_npi = SMPP_NPI_ISDN;
	d1.destination_addr = "15551234567";
	CHECK(smpp_dest_address_write(dest_blob, sizeof(dest_blob), &dest_off,
								  &d1) == SMPP_LIST_OK);

	smpp_dest_address_t d2 = { 0 };
	d2.dest_flag = SMPP_DEST_FLAG_DISTRIBUTION_LIST;
	d2.dl_name = "vip-list";
	CHECK(smpp_dest_address_write(dest_blob, sizeof(dest_blob), &dest_off,
								  &d2) == SMPP_LIST_OK);

	uint8_t wire[256];
	smpp_submit_multi_t in = { 0 };
	in.service_type = "";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15550001111";
	in.dest_count = 2;
	in.dest_addresses = dest_blob;
	in.dest_addresses_length = (uint16_t)dest_off;
	in.esm_class = SMPP_ESM_CLASS_DEFAULT;
	in.schedule_delivery_time = "";
	in.validity_period = "";
	in.registered_delivery = SMPP_REGISTERED_DELIVERY_SUCCESS_OR_FAILURE;
	in.data_coding = SMPP_DATA_CODING_IA5;
	in.short_message = payload;
	in.short_message_length = (uint8_t)(sizeof(payload) - 1);

	size_t needed = smpp_submit_multi_encoded_length(&in);
	CHECK(smpp_submit_multi_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_submit_multi_t out;
	CHECK(smpp_submit_multi_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.source_addr, "15550001111") == 0);
	CHECK(out.dest_count == 2);
	CHECK(out.dest_addresses_length == dest_off);
	CHECK(memcmp(out.short_message, payload, sizeof(payload) - 1) == 0);

	const uint8_t *cursor = out.dest_addresses;
	const uint8_t *end = out.dest_addresses + out.dest_addresses_length;
	smpp_dest_address_t entry;

	CHECK(smpp_dest_address_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(entry.dest_flag == SMPP_DEST_FLAG_SME_ADDRESS);
	CHECK(strcmp(entry.destination_addr, "15551234567") == 0);

	CHECK(smpp_dest_address_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(entry.dest_flag == SMPP_DEST_FLAG_DISTRIBUTION_LIST);
	CHECK(strcmp(entry.dl_name, "vip-list") == 0);

	CHECK(smpp_dest_address_next(&cursor, end, &entry) == SMPP_LIST_END);
	return 0;
}

static int
test_submit_multi_resp_round_trip_multiple_unsuccess(void)
{
	uint8_t unsuccess_blob[64];
	size_t off = 0;
	smpp_unsuccess_sme_t a = { 0 };
	a.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	a.dest_addr_npi = SMPP_NPI_ISDN;
	a.destination_addr = "15551234567";
	a.error_status_code = SMPP_ESME_RSUBMITFAIL;
	CHECK(smpp_unsuccess_sme_write(unsuccess_blob, sizeof(unsuccess_blob), &off,
								   &a) == SMPP_LIST_OK);

	smpp_unsuccess_sme_t b = { 0 };
	b.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	b.dest_addr_npi = SMPP_NPI_ISDN;
	b.destination_addr = "15559876543";
	b.error_status_code = SMPP_ESME_RTHROTTLED;
	CHECK(smpp_unsuccess_sme_write(unsuccess_blob, sizeof(unsuccess_blob), &off,
								   &b) == SMPP_LIST_OK);

	uint8_t wire[128];
	smpp_submit_multi_resp_t in = { 0 };
	in.message_id = "MC-00012345";
	in.unsuccess_count = 2;
	in.unsuccess_smes = unsuccess_blob;
	in.unsuccess_smes_length = (uint16_t)off;

	size_t needed = smpp_submit_multi_resp_encoded_length(&in);
	CHECK(smpp_submit_multi_resp_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_submit_multi_resp_t out;
	CHECK(smpp_submit_multi_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "MC-00012345") == 0);
	CHECK(out.unsuccess_count == 2);
	CHECK(out.unsuccess_smes_length == off);

	const uint8_t *cursor = out.unsuccess_smes;
	const uint8_t *end = out.unsuccess_smes + out.unsuccess_smes_length;
	smpp_unsuccess_sme_t entry;

	CHECK(smpp_unsuccess_sme_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(entry.error_status_code == SMPP_ESME_RSUBMITFAIL);
	CHECK(smpp_unsuccess_sme_next(&cursor, end, &entry) == SMPP_LIST_OK);
	CHECK(entry.error_status_code == SMPP_ESME_RTHROTTLED);
	CHECK(smpp_unsuccess_sme_next(&cursor, end, &entry) == SMPP_LIST_END);
	return 0;
}

/* dest_count claims 3 entries but the body only actually contains 2: decode
 * must fail the whole PDU rather than silently accepting a short list. This
 * is the count-vs-END hazard called out in smpp_list_status_t's doc comment.
 * SMPP_LIST_END on the 3rd call must be treated as truncation, not a
 * valid stop. */
static int
test_submit_multi_decode_truncated_dest_address_block(void)
{
	uint8_t dest_blob[64];
	size_t dest_off = 0;
	smpp_dest_address_t d1 = { 0 };
	d1.dest_flag = SMPP_DEST_FLAG_SME_ADDRESS;
	d1.destination_addr = "1000";
	CHECK(smpp_dest_address_write(dest_blob, sizeof(dest_blob), &dest_off,
								  &d1) == SMPP_LIST_OK);

	smpp_dest_address_t d2 = { 0 };
	d2.dest_flag = SMPP_DEST_FLAG_DISTRIBUTION_LIST;
	d2.dl_name = "list";
	CHECK(smpp_dest_address_write(dest_blob, sizeof(dest_blob), &dest_off,
								  &d2) == SMPP_LIST_OK);

	uint8_t wire[256];
	smpp_submit_multi_t in = { 0 };
	in.source_addr = "";
	in.dest_count = 3; /* lying: only 2 entries actually follow */
	in.dest_addresses = dest_blob;
	in.dest_addresses_length = (uint16_t)dest_off;
	in.schedule_delivery_time = "";
	in.validity_period = "";

	size_t needed = smpp_submit_multi_encoded_length(&in);
	CHECK(smpp_submit_multi_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_submit_multi_t out;
	CHECK(smpp_submit_multi_decode(wire, needed, &out) == SMPP_ERR_INVALID);
	return 0;
}

static int
test_submit_multi_decode_bad_dest_flag(void)
{
	uint8_t wire[256];
	smpp_submit_multi_t in = { 0 };
	in.source_addr = "";
	in.dest_count = 1;
	uint8_t bad_dest[1] = { 0x03 }; /* not a defined dest_flag value */
	in.dest_addresses = bad_dest;
	in.dest_addresses_length = 1;
	in.schedule_delivery_time = "";
	in.validity_period = "";

	size_t needed = smpp_submit_multi_encoded_length(&in);
	CHECK(smpp_submit_multi_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_submit_multi_t out;
	CHECK(smpp_submit_multi_decode(wire, needed, &out) == SMPP_ERR_INVALID);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_dest_address_iteration();
	failures += test_unsuccess_sme_iteration();
	failures += test_submit_multi_round_trip_mixed_destinations();
	failures += test_submit_multi_resp_round_trip_multiple_unsuccess();
	failures += test_submit_multi_decode_truncated_dest_address_block();
	failures += test_submit_multi_decode_bad_dest_flag();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
