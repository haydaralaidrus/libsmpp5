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

#include "libsmpp5/pdu.h"
#include "libsmpp5/smpp.h"
#include "libsmpp5/smpp_tlv.h"
#include "tests.h"

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
						 2) == SMPP_TLV_OK);

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
test_sm_per_op_aliases(void)
{
	uint8_t wire[128];

	smpp_submit_sm_t submit = { 0 };
	submit.destination_addr = "15559876543";
	size_t submit_needed = smpp_submit_sm_encoded_length(&submit);
	CHECK(smpp_submit_sm_encode(&submit, wire, sizeof(wire)) == SMPP_OK);
	smpp_submit_sm_t submit_out;
	CHECK(smpp_submit_sm_decode(wire, submit_needed, &submit_out) == SMPP_OK);
	CHECK(strcmp(submit_out.destination_addr, "15559876543") == 0);

	smpp_deliver_sm_t deliver = { 0 };
	deliver.destination_addr = "15551234567";
	size_t deliver_needed = smpp_deliver_sm_encoded_length(&deliver);
	CHECK(smpp_deliver_sm_encode(&deliver, wire, sizeof(wire)) == SMPP_OK);
	smpp_deliver_sm_t deliver_out;
	CHECK(smpp_deliver_sm_decode(wire, deliver_needed, &deliver_out) ==
		  SMPP_OK);
	CHECK(strcmp(deliver_out.destination_addr, "15551234567") == 0);

	smpp_submit_sm_resp_t submit_resp = { 0 };
	submit_resp.message_id = "MC-SUBMIT-1";
	size_t submit_resp_needed =
		smpp_submit_sm_resp_encoded_length(&submit_resp);
	CHECK(smpp_submit_sm_resp_encode(&submit_resp, wire, sizeof(wire)) ==
		  SMPP_OK);
	smpp_submit_sm_resp_t submit_resp_out;
	CHECK(smpp_submit_sm_resp_decode(wire, submit_resp_needed,
									 &submit_resp_out) == SMPP_OK);
	CHECK(strcmp(submit_resp_out.message_id, "MC-SUBMIT-1") == 0);

	smpp_deliver_sm_resp_t deliver_resp = { 0 };
	deliver_resp.message_id = "MC-DELIVER-1";
	size_t deliver_resp_needed =
		smpp_deliver_sm_resp_encoded_length(&deliver_resp);
	CHECK(smpp_deliver_sm_resp_encode(&deliver_resp, wire, sizeof(wire)) ==
		  SMPP_OK);
	smpp_deliver_sm_resp_t deliver_resp_out;
	CHECK(smpp_deliver_sm_resp_decode(wire, deliver_resp_needed,
									  &deliver_resp_out) == SMPP_OK);
	CHECK(strcmp(deliver_resp_out.message_id, "MC-DELIVER-1") == 0);
	return 0;
}

static int
test_sm_encode_validation_errors(void)
{
	uint8_t wire[128];

	smpp_sm_t bad_sm = { 0 };
	bad_sm.schedule_delivery_time = "12345";
	CHECK(smpp_sm_encode(&bad_sm, wire, sizeof(wire)) == SMPP_ERR_INVALID);
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

	failures += test_submit_sm_round_trip();
	failures += test_submit_sm_with_absolute_time_and_tlv();
	failures += test_sm_resp_round_trip();
	failures += test_sm_per_op_aliases();
	failures += test_sm_encode_validation_errors();
	failures += test_full_pipeline_through_pdu_layer();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
