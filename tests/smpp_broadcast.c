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
test_broadcast_sm_round_trip(void)
{
	uint8_t tlv_blob[16];
	size_t off = 0;
	const uint8_t area[] = { 0x01, 0x02, 0x03 };
	CHECK(smpp_tlv_write(tlv_blob, sizeof(tlv_blob), &off,
						 SMPP_TAG_BROADCAST_AREA_IDENTIFIER, area,
						 (uint16_t)sizeof(area)) == SMPP_TLV_OK);

	uint8_t wire[128];
	smpp_broadcast_sm_t in = { 0 };
	in.service_type = "";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";
	in.message_id = "";
	in.priority_flag = 1;
	in.schedule_delivery_time = "";
	in.validity_period = "";
	in.replace_if_present_flag = SMPP_REPLACE_IF_PRESENT_NO;
	in.data_coding = SMPP_DATA_CODING_DEFAULT;
	in.tlvs = tlv_blob;
	in.tlvs_length = (uint16_t)off;

	size_t needed = smpp_broadcast_sm_encoded_length(&in);
	CHECK(smpp_broadcast_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_broadcast_sm_t out;
	CHECK(smpp_broadcast_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.source_addr, "15551234567") == 0);

	smpp_tlv_t tlv;
	CHECK(smpp_tlv_find(out.tlvs, out.tlvs_length,
						SMPP_TAG_BROADCAST_AREA_IDENTIFIER, &tlv) == 1);
	CHECK(tlv.length == sizeof(area));
	return 0;
}

static int
test_broadcast_sm_resp_round_trip(void)
{
	uint8_t wire[64];
	smpp_broadcast_sm_resp_t in = { 0 };
	in.message_id = "BCAST-00001";

	size_t needed = smpp_broadcast_sm_resp_encoded_length(&in);
	CHECK(smpp_broadcast_sm_resp_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_broadcast_sm_resp_t out;
	CHECK(smpp_broadcast_sm_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "BCAST-00001") == 0);
	return 0;
}

static int
test_query_broadcast_sm_round_trip(void)
{
	uint8_t wire[64];
	smpp_query_broadcast_sm_t in = { 0 };
	in.message_id = "BCAST-00001";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";

	size_t needed = smpp_query_broadcast_sm_encoded_length(&in);
	CHECK(smpp_query_broadcast_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_query_broadcast_sm_t out;
	CHECK(smpp_query_broadcast_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "BCAST-00001") == 0);
	CHECK(out.tlvs_length == 0);
	return 0;
}

static int
test_query_broadcast_sm_resp_round_trip(void)
{
	uint8_t wire[64];
	smpp_query_broadcast_sm_resp_t in = { 0 };
	in.message_id = "BCAST-00001";

	size_t needed = smpp_query_broadcast_sm_resp_encoded_length(&in);
	CHECK(smpp_query_broadcast_sm_resp_encode(&in, wire, sizeof(wire)) ==
		  SMPP_OK);

	smpp_query_broadcast_sm_resp_t out;
	CHECK(smpp_query_broadcast_sm_resp_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "BCAST-00001") == 0);
	return 0;
}

static int
test_cancel_broadcast_sm_round_trip(void)
{
	uint8_t wire[64];
	smpp_cancel_broadcast_sm_t in = { 0 };
	in.service_type = "";
	in.message_id = "BCAST-00001";
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	in.source_addr = "15551234567";

	size_t needed = smpp_cancel_broadcast_sm_encoded_length(&in);
	CHECK(smpp_cancel_broadcast_sm_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_cancel_broadcast_sm_t out;
	CHECK(smpp_cancel_broadcast_sm_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.message_id, "BCAST-00001") == 0);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_broadcast_sm_round_trip();
	failures += test_broadcast_sm_resp_round_trip();
	failures += test_query_broadcast_sm_round_trip();
	failures += test_query_broadcast_sm_resp_round_trip();
	failures += test_cancel_broadcast_sm_round_trip();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
