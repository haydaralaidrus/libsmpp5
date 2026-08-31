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

#include <string.h>

#include "smpp_tlv.h"

smpp_tlv_status_t
smpp_tlv_next(const uint8_t **cursor, const uint8_t *end, smpp_tlv_t *tlv)
{
	const uint8_t *p = *cursor;

	if (p == end)
		return SMPP_TLV_END;

	if ((size_t)(end - p) < 4)
		return SMPP_TLV_MALFORMED;

	uint16_t tag = (uint16_t)(((uint16_t)p[0] << 8) | (uint16_t)p[1]);
	uint16_t length = (uint16_t)(((uint16_t)p[2] << 8) | (uint16_t)p[3]);

	if ((size_t)(end - p) < (size_t)4 + length)
		return SMPP_TLV_MALFORMED;

	tlv->tag = tag;
	tlv->length = length;
	tlv->value = length > 0 ? p + 4 : NULL;

	*cursor = p + 4 + length;
	return SMPP_TLV_OK;
}

int
smpp_tlv_find(const uint8_t *tlvs, uint16_t tlvs_length, uint16_t tag,
			  smpp_tlv_t *tlv)
{
	if (tlvs == NULL || tlvs_length == 0)
		return 0;

	const uint8_t *cursor = tlvs;
	const uint8_t *end = tlvs + tlvs_length;

	while (cursor != end) {
		smpp_tlv_t candidate;
		smpp_tlv_status_t status = smpp_tlv_next(&cursor, end, &candidate);
		if (status != SMPP_TLV_OK)
			return 0;

		if (candidate.tag == tag) {
			*tlv = candidate;
			return 1;
		}
	}

	return 0;
}

smpp_tlv_status_t
smpp_tlv_write(uint8_t *buffer, size_t buffer_length, size_t *offset,
			   uint16_t tag, const uint8_t *value, uint16_t value_length)
{
	size_t needed = (size_t)4 + value_length;
	if (*offset + needed > buffer_length)
		return SMPP_TLV_ERR_BUFFER_TOO_SMALL;

	uint8_t *p = buffer + *offset;
	p[0] = (uint8_t)(tag >> 8);
	p[1] = (uint8_t)tag;
	p[2] = (uint8_t)(value_length >> 8);
	p[3] = (uint8_t)value_length;
	if (value_length > 0)
		memcpy(p + 4, value, value_length);

	*offset += needed;
	return SMPP_TLV_OK;
}
