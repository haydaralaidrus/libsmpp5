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
						 1) == SMPP_TLV_OK);

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
test_outbind_round_trip(void)
{
	uint8_t wire[64];
	smpp_outbind_t in = { 0 };
	in.system_id = "themc";
	in.password = "secret12";

	size_t needed = smpp_outbind_encoded_length(&in);
	CHECK(smpp_outbind_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_outbind_t out;
	CHECK(smpp_outbind_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.system_id, "themc") == 0);
	CHECK(strcmp(out.password, "secret12") == 0);
	return 0;
}

static int
test_bind_encode_validation_errors(void)
{
	uint8_t wire[128];

	smpp_bind_t bad_bind = { 0 };
	char long_id[17];
	memset(long_id, 'a', 16);
	long_id[16] = '\0';
	bad_bind.system_id = long_id;
	CHECK(smpp_bind_encode(&bad_bind, wire, sizeof(wire)) == SMPP_ERR_INVALID);

	smpp_bind_t ok_bind = { 0 };
	ok_bind.system_id = "x";
	uint8_t tiny[3];
	CHECK(smpp_bind_encode(&ok_bind, tiny, sizeof(tiny)) ==
		  SMPP_ERR_BUFFER_TOO_SMALL);
	return 0;
}

static int
test_bind_decode_validation_errors(void)
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

int
main(void)
{
	int failures = 0;

	failures += test_bind_round_trip();
	failures += test_bind_resp_round_trip_with_tlv();
	failures += test_outbind_round_trip();
	failures += test_bind_encode_validation_errors();
	failures += test_bind_decode_validation_errors();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
