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
test_spec_constants(void)
{
	CHECK(SMPP_CMD_SUBMIT_SM == 0x00000004u);
	CHECK(SMPP_CMD_SUBMIT_SM_RESP == 0x80000004u);
	CHECK(SMPP_CMD_DELIVER_SM_RESP == 0x80000005u);
	CHECK(SMPP_CMD_BIND_TRANSCEIVER_RESP == 0x80000009u);
	CHECK(SMPP_CMD_QUERY_SM_RESP == 0x80000003u); /* spec PDF typo: 0x8000003 */
	CHECK(SMPP_ESME_ROK == 0);
	CHECK(SMPP_ESME_RINVCMDID == 0x00000003u);
	CHECK(SMPP_ESME_RINVBCASTCHANIND == 0x00000112u);
	CHECK(SMPP_TAG_MESSAGE_PAYLOAD == 0x0424u);
	CHECK(SMPP_TAG_SC_INTERFACE_VERSION == 0x0210u);
	CHECK(SMPP_TAG_ITS_SESSION_INFO == 0x1383u);
	CHECK(SMPP_MESSAGE_STATE_SKIPPED == 0x09u);
	CHECK(SMPP_DEST_FLAG_DISTRIBUTION_LIST == 0x02u);
	return 0;
}

static int
test_strerror(void)
{
	CHECK(strcmp(smpp_strerror(SMPP_OK), "success") == 0);
	CHECK(strlen(smpp_strerror(SMPP_ERR_INVALID)) > 0);
	CHECK(strlen(smpp_strerror(SMPP_ERR_BUFFER_TOO_SMALL)) > 0);
	CHECK(smpp_strerror((smpp_status_t)999) != NULL);

	CHECK(strcmp(smpp_esme_strerror(SMPP_ESME_ROK), "success") == 0);
	CHECK(strlen(smpp_esme_strerror(SMPP_ESME_RINVMSGLEN)) > 0);
	CHECK(strlen(smpp_esme_strerror(SMPP_ESME_RBINDFAIL)) > 0);
	CHECK(strlen(smpp_esme_strerror(SMPP_ESME_RINVBCASTCHANIND)) > 0);

	CHECK(smpp_esme_strerror(0x00000012u) != NULL);
	CHECK(smpp_esme_strerror(0x00000450u) != NULL);
	return 0;
}

int
main(void)
{
	int failures = 0;

	failures += test_spec_constants();
	failures += test_strerror();

	if (failures > 0) {
		fprintf(stderr, "%d check(s) failed\n", failures);
		return 1;
	}
	printf("All tests passed\n");
	return 0;
}
