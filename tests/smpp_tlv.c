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

#include "libsmpp5/smpp_tlv.h"
#include "tests.h"

static int
test_tlv_iteration(void)
{
	uint8_t blob[32];
	size_t off = 0;
	const uint8_t v1[] = { 0xAA };
	const uint8_t v2[] = { 0xBB, 0xCC };
	CHECK(smpp_tlv_write(blob, sizeof(blob), &off, 0x0001, v1, 1) ==
		  SMPP_TLV_OK);
	CHECK(smpp_tlv_write(blob, sizeof(blob), &off, 0x0002, v2, 2) ==
		  SMPP_TLV_OK);

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
test_strerror(void)
{
	CHECK(strcmp(smpp_tlv_strerror(SMPP_TLV_OK), "success") == 0);
	CHECK(strlen(smpp_tlv_strerror(SMPP_TLV_END)) > 0);
	CHECK(strlen(smpp_tlv_strerror(SMPP_TLV_MALFORMED)) > 0);
	CHECK(strlen(smpp_tlv_strerror(SMPP_TLV_ERR_BUFFER_TOO_SMALL)) > 0);
	CHECK(smpp_tlv_strerror((smpp_tlv_status_t)999) != NULL);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_tlv_iteration();
	failures += test_strerror();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
