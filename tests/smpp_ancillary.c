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
test_query_sm_round_trip(void)
{
	uint8_t wire[64];
	smpp_query_sm_t in = { 0 };
	in.message_id = "MC-00012345";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";

	size_t needed = smpp_query_sm_encoded_length(&in);
	CHECK(smpp_query_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_query_sm_t out;
	CHECK(smpp_query_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "MC-00012345") == 0);
	CHECK(strcmp(out.source_addr, "15551234567") == 0);
	return 0;
}

static int
test_query_sm_resp_round_trip(void)
{
	uint8_t wire[64];
	smpp_query_sm_resp_t in = { 0 };
	in.message_id = "MC-00012345";
	in.final_date = "";
	in.message_state = SMPP_MESSAGE_STATE_DELIVERED;
	in.error_code = 0;

	size_t needed = smpp_query_sm_resp_encoded_length(&in);
	CHECK(smpp_query_sm_resp_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_query_sm_resp_t out;
	CHECK(smpp_query_sm_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "MC-00012345") == 0);
	CHECK(strcmp(out.final_date, "") == 0);
	CHECK(out.message_state == SMPP_MESSAGE_STATE_DELIVERED);
	return 0;
}

static int
test_query_sm_resp_with_absolute_final_date(void)
{
	uint8_t wire[64];
	smpp_query_sm_resp_t in = { 0 };
	in.message_id = "1";
	in.final_date = "251231235959000R";
	in.message_state = SMPP_MESSAGE_STATE_EXPIRED;
	in.error_code = 5;

	size_t needed = smpp_query_sm_resp_encoded_length(&in);
	CHECK(smpp_query_sm_resp_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_query_sm_resp_t out;
	CHECK(smpp_query_sm_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.final_date, "251231235959000R") == 0);
	CHECK(out.message_state == SMPP_MESSAGE_STATE_EXPIRED);
	CHECK(out.error_code == 5);
	return 0;
}

static int
test_cancel_sm_round_trip(void)
{
	uint8_t wire[128];
	smpp_cancel_sm_t in = { 0 };
	in.service_type = "";
	in.message_id = "MC-00012345";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";
	in.dest_addr_ton = SMPP_TON_INTERNATIONAL;
	in.dest_addr_npi = SMPP_NPI_ISDN;
	in.destination_addr = "15559876543";

	size_t needed = smpp_cancel_sm_encoded_length(&in);
	CHECK(smpp_cancel_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_cancel_sm_t out;
	CHECK(smpp_cancel_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "MC-00012345") == 0);
	CHECK(strcmp(out.source_addr, "15551234567") == 0);
	CHECK(strcmp(out.destination_addr, "15559876543") == 0);
	return 0;
}

static int
test_replace_sm_round_trip(void)
{
	const uint8_t payload[] = "New text";
	uint8_t tlv_blob[8];
	size_t off = 0;
	CHECK(smpp_tlv_write(tlv_blob, sizeof(tlv_blob), &off,
						 SMPP_TAG_USER_MESSAGE_REFERENCE,
						 (const uint8_t *)"\x00\x01", 2) == SMPP_TLV_OK);

	uint8_t wire[256];
	smpp_replace_sm_t in = { 0 };
	in.message_id = "MC-00012345";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";
	in.schedule_delivery_time = "";
	in.validity_period = "";
	in.registered_delivery = SMPP_REGISTERED_DELIVERY_SUCCESS_OR_FAILURE;
	in.short_message = payload;
	in.short_message_length = (uint8_t)(sizeof(payload) - 1);
	in.tlvs = tlv_blob;
	in.tlvs_length = (uint16_t)off;

	size_t needed = smpp_replace_sm_encoded_length(&in);
	CHECK(smpp_replace_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_replace_sm_t out;
	CHECK(smpp_replace_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "MC-00012345") == 0);
	CHECK(out.short_message_length == sizeof(payload) - 1);
	CHECK(memcmp(out.short_message, payload, sizeof(payload) - 1) == 0);
	CHECK(out.tlvs_length == off);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_query_sm_round_trip();
	failures += test_query_sm_resp_round_trip();
	failures += test_query_sm_resp_with_absolute_final_date();
	failures += test_cancel_sm_round_trip();
	failures += test_replace_sm_round_trip();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
