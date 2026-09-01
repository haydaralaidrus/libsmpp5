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
test_alert_notification_round_trip(void)
{
	uint8_t tlv_blob[8];
	size_t off = 0;
	uint8_t status = 0x01;
	CHECK(smpp_tlv_write(tlv_blob, sizeof(tlv_blob), &off,
						 SMPP_TAG_MS_AVAILABILITY_STATUS, &status,
						 1) == SMPP_TLV_OK);

	uint8_t wire[128];
	smpp_alert_notification_t in = { 0 };
	in.source_addr_ton = SMPP_TON_INTERNATIONAL;
	in.source_addr_npi = SMPP_NPI_ISDN;
	/* Longer than SMPP_ADDR_MAX(21): only valid because alert_notification
	 * uses SMPP_LONG_ADDR_MAX(65), unlike most other operations. */
	in.source_addr = "1555512345678901234567890";
	in.esme_addr_ton = SMPP_TON_INTERNATIONAL;
	in.esme_addr_npi = SMPP_NPI_ISDN;
	in.esme_addr = "15559876543";
	in.tlvs = tlv_blob;
	in.tlvs_length = (uint16_t)off;

	size_t needed = smpp_alert_notification_encoded_length(&in);
	CHECK(smpp_alert_notification_encode(&in, wire, sizeof(wire)) == SMPP_OK);

	smpp_alert_notification_t out;
	CHECK(smpp_alert_notification_decode(wire, needed, &out) == SMPP_OK);
	CHECK(strcmp(out.source_addr, "1555512345678901234567890") == 0);
	CHECK(strcmp(out.esme_addr, "15559876543") == 0);

	smpp_tlv_t tlv;
	CHECK(smpp_tlv_find(out.tlvs, out.tlvs_length,
						SMPP_TAG_MS_AVAILABILITY_STATUS, &tlv) == 1);
	CHECK(tlv.value[0] == 0x01);
	return 0;
}

static int
test_alert_notification_encode_validation_errors(void)
{
	uint8_t wire[128];

	smpp_alert_notification_t bad = { 0 };
	char long_addr[67];
	memset(long_addr, '1', 66);
	long_addr[66] = '\0';
	bad.source_addr = long_addr;
	CHECK(smpp_alert_notification_encode(&bad, wire, sizeof(wire)) ==
		  SMPP_ERR_INVALID);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_alert_notification_round_trip();
	failures += test_alert_notification_encode_validation_errors();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
