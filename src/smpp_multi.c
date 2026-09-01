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

#include "libsmpp5/smpp.h"
#include "smpp_internal.h"

smpp_list_status_t
smpp_dest_address_next(const uint8_t **cursor, const uint8_t *end,
					   smpp_dest_address_t *dest)
{
	const uint8_t *p = *cursor;

	if (p == end)
		return SMPP_LIST_END;

	smpp_dest_address_t result = { 0 };
	result.dest_flag = p[0];
	p++;

	if (result.dest_flag == SMPP_DEST_FLAG_SME_ADDRESS) {
		if ((size_t)(end - p) < 2)
			return SMPP_LIST_MALFORMED;

		result.dest_addr_ton = p[0];
		result.dest_addr_npi = p[1];
		p += 2;

		size_t window = (size_t)(end - p);
		if (window > SMPP_ADDR_MAX)
			window = SMPP_ADDR_MAX;

		size_t i;
		for (i = 0; i < window; i++) {
			if (p[i] == 0x00)
				break;
		}
		if (i == window)
			return SMPP_LIST_MALFORMED;

		result.destination_addr = (const char *)p;
		p += i + 1;
	} else if (result.dest_flag == SMPP_DEST_FLAG_DISTRIBUTION_LIST) {
		size_t window = (size_t)(end - p);
		if (window > SMPP_DL_NAME_MAX)
			window = SMPP_DL_NAME_MAX;

		size_t i;
		for (i = 0; i < window; i++) {
			if (p[i] == 0x00)
				break;
		}
		if (i == window)
			return SMPP_LIST_MALFORMED;

		result.dl_name = (const char *)p;
		p += i + 1;
	} else {
		return SMPP_LIST_MALFORMED;
	}

	*cursor = p;
	*dest = result;
	return SMPP_LIST_OK;
}

smpp_list_status_t
smpp_dest_address_write(uint8_t *buffer, size_t buffer_length, size_t *offset,
						const smpp_dest_address_t *dest)
{
	if (dest->dest_flag == SMPP_DEST_FLAG_SME_ADDRESS) {
		size_t addr_len = smpp_priv_safe_strlen(dest->destination_addr);
		if (addr_len + 1 > SMPP_ADDR_MAX)
			return SMPP_LIST_MALFORMED;

		size_t needed = 3 + addr_len + 1;
		if (*offset + needed > buffer_length)
			return SMPP_LIST_ERR_BUFFER_TOO_SMALL;

		uint8_t *p = buffer + *offset;
		p[0] = SMPP_DEST_FLAG_SME_ADDRESS;
		p[1] = dest->dest_addr_ton;
		p[2] = dest->dest_addr_npi;
		if (addr_len > 0)
			memcpy(p + 3, dest->destination_addr, addr_len);
		p[3 + addr_len] = '\0';

		*offset += needed;
		return SMPP_LIST_OK;
	}

	if (dest->dest_flag == SMPP_DEST_FLAG_DISTRIBUTION_LIST) {
		size_t name_len = smpp_priv_safe_strlen(dest->dl_name);
		if (name_len + 1 > SMPP_DL_NAME_MAX)
			return SMPP_LIST_MALFORMED;

		size_t needed = 1 + name_len + 1;
		if (*offset + needed > buffer_length)
			return SMPP_LIST_ERR_BUFFER_TOO_SMALL;

		uint8_t *p = buffer + *offset;
		p[0] = SMPP_DEST_FLAG_DISTRIBUTION_LIST;
		if (name_len > 0)
			memcpy(p + 1, dest->dl_name, name_len);
		p[1 + name_len] = '\0';

		*offset += needed;
		return SMPP_LIST_OK;
	}

	return SMPP_LIST_MALFORMED;
}

smpp_list_status_t
smpp_unsuccess_sme_next(const uint8_t **cursor, const uint8_t *end,
						smpp_unsuccess_sme_t *sme)
{
	const uint8_t *p = *cursor;

	if (p == end)
		return SMPP_LIST_END;

	if ((size_t)(end - p) < 2)
		return SMPP_LIST_MALFORMED;

	smpp_unsuccess_sme_t result = { 0 };
	result.dest_addr_ton = p[0];
	result.dest_addr_npi = p[1];
	p += 2;

	size_t window = (size_t)(end - p);
	if (window > SMPP_ADDR_MAX)
		window = SMPP_ADDR_MAX;

	size_t i;
	for (i = 0; i < window; i++) {
		if (p[i] == 0x00)
			break;
	}
	if (i == window)
		return SMPP_LIST_MALFORMED;

	result.destination_addr = (const char *)p;
	p += i + 1;

	if ((size_t)(end - p) < 4)
		return SMPP_LIST_MALFORMED;

	result.error_status_code = ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
							   ((uint32_t)p[2] << 8) | (uint32_t)p[3];
	p += 4;

	*cursor = p;
	*sme = result;
	return SMPP_LIST_OK;
}

smpp_list_status_t
smpp_unsuccess_sme_write(uint8_t *buffer, size_t buffer_length, size_t *offset,
						 const smpp_unsuccess_sme_t *sme)
{
	size_t addr_len = smpp_priv_safe_strlen(sme->destination_addr);
	if (addr_len + 1 > SMPP_ADDR_MAX)
		return SMPP_LIST_MALFORMED;

	size_t needed = 2 + addr_len + 1 + 4;
	if (*offset + needed > buffer_length)
		return SMPP_LIST_ERR_BUFFER_TOO_SMALL;

	uint8_t *p = buffer + *offset;
	p[0] = sme->dest_addr_ton;
	p[1] = sme->dest_addr_npi;
	if (addr_len > 0)
		memcpy(p + 2, sme->destination_addr, addr_len);
	p[2 + addr_len] = '\0';

	size_t err_off = 2 + addr_len + 1;
	p[err_off] = (uint8_t)(sme->error_status_code >> 24);
	p[err_off + 1] = (uint8_t)(sme->error_status_code >> 16);
	p[err_off + 2] = (uint8_t)(sme->error_status_code >> 8);
	p[err_off + 3] = (uint8_t)sme->error_status_code;

	*offset += needed;
	return SMPP_LIST_OK;
}

smpp_status_t
smpp_submit_multi_decode(const uint8_t *body, size_t body_length,
						 smpp_submit_multi_t *multi)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	multi->service_type = smpp_priv_r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	multi->source_addr_ton = smpp_priv_r_u8(&r);
	multi->source_addr_npi = smpp_priv_r_u8(&r);
	multi->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);
	multi->dest_count = smpp_priv_r_u8(&r);

	/* The list walk below is outside reader_t's sticky-error tracking, so
	 * an earlier failure must be caught explicitly here rather than
	 * relying on later smpp_priv_r_* calls to no-op it away. */
	if (r.error)
		return r.error;

	const uint8_t *list_start = r.buf + r.pos;
	const uint8_t *cursor = list_start;
	const uint8_t *end = r.buf + r.len;

	for (uint8_t i = 0; i < multi->dest_count; i++) {
		smpp_dest_address_t entry;
		if (smpp_dest_address_next(&cursor, end, &entry) != SMPP_LIST_OK)
			return SMPP_ERR_INVALID;
	}

	multi->dest_addresses = list_start;
	multi->dest_addresses_length = (uint16_t)(cursor - list_start);
	r.pos = (size_t)(cursor - r.buf);

	multi->esm_class = smpp_priv_r_u8(&r);
	multi->protocol_id = smpp_priv_r_u8(&r);
	multi->priority_flag = smpp_priv_r_u8(&r);
	multi->schedule_delivery_time = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	multi->validity_period = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	multi->registered_delivery = smpp_priv_r_u8(&r);
	multi->replace_if_present_flag = smpp_priv_r_u8(&r);
	multi->data_coding = smpp_priv_r_u8(&r);
	multi->sm_default_msg_id = smpp_priv_r_u8(&r);
	multi->short_message_length = smpp_priv_r_u8(&r);
	multi->short_message = smpp_priv_r_octets(&r, multi->short_message_length);

	smpp_priv_r_tlvs(&r, &multi->tlvs, &multi->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_submit_multi_encode(const smpp_submit_multi_t *multi, uint8_t *buffer,
						 size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, multi->service_type, SMPP_SERVICE_TYPE_MAX);
	smpp_priv_w_u8(&w, multi->source_addr_ton);
	smpp_priv_w_u8(&w, multi->source_addr_npi);
	smpp_priv_w_cstring(&w, multi->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_u8(&w, multi->dest_count);
	smpp_priv_w_bytes(&w, multi->dest_addresses, multi->dest_addresses_length);
	smpp_priv_w_u8(&w, multi->esm_class);
	smpp_priv_w_u8(&w, multi->protocol_id);
	smpp_priv_w_u8(&w, multi->priority_flag);
	smpp_priv_w_time_cstring(&w, multi->schedule_delivery_time);
	smpp_priv_w_time_cstring(&w, multi->validity_period);
	smpp_priv_w_u8(&w, multi->registered_delivery);
	smpp_priv_w_u8(&w, multi->replace_if_present_flag);
	smpp_priv_w_u8(&w, multi->data_coding);
	smpp_priv_w_u8(&w, multi->sm_default_msg_id);
	smpp_priv_w_u8(&w, multi->short_message_length);
	smpp_priv_w_bytes(&w, multi->short_message, multi->short_message_length);
	smpp_priv_w_bytes(&w, multi->tlvs, multi->tlvs_length);

	return w.error;
}

size_t
smpp_submit_multi_encoded_length(const smpp_submit_multi_t *multi)
{
	return smpp_priv_safe_strlen(multi->service_type) + 1 + 2 +
		   smpp_priv_safe_strlen(multi->source_addr) + 1 + 1 +
		   multi->dest_addresses_length + 3 +
		   smpp_priv_safe_strlen(multi->schedule_delivery_time) + 1 +
		   smpp_priv_safe_strlen(multi->validity_period) + 1 + 5 +
		   multi->short_message_length + multi->tlvs_length;
}

smpp_status_t
smpp_submit_multi_resp_decode(const uint8_t *body, size_t body_length,
							  smpp_submit_multi_resp_t *resp)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	resp->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	resp->unsuccess_count = smpp_priv_r_u8(&r);

	if (r.error)
		return r.error;

	const uint8_t *list_start = r.buf + r.pos;
	const uint8_t *cursor = list_start;
	const uint8_t *end = r.buf + r.len;

	for (uint8_t i = 0; i < resp->unsuccess_count; i++) {
		smpp_unsuccess_sme_t entry;
		if (smpp_unsuccess_sme_next(&cursor, end, &entry) != SMPP_LIST_OK)
			return SMPP_ERR_INVALID;
	}

	resp->unsuccess_smes = list_start;
	resp->unsuccess_smes_length = (uint16_t)(cursor - list_start);
	r.pos = (size_t)(cursor - r.buf);

	smpp_priv_r_tlvs(&r, &resp->tlvs, &resp->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_submit_multi_resp_encode(const smpp_submit_multi_resp_t *resp,
							  uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, resp->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, resp->unsuccess_count);
	smpp_priv_w_bytes(&w, resp->unsuccess_smes, resp->unsuccess_smes_length);
	smpp_priv_w_bytes(&w, resp->tlvs, resp->tlvs_length);

	return w.error;
}

size_t
smpp_submit_multi_resp_encoded_length(const smpp_submit_multi_resp_t *resp)
{
	return smpp_priv_safe_strlen(resp->message_id) + 1 + 1 +
		   resp->unsuccess_smes_length + resp->tlvs_length;
}
