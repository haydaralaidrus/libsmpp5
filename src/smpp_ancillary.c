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

/* Spec section 4.5, "Ancillary Submission Operations": query_sm, cancel_sm,
 * replace_sm. cancel_sm_resp/replace_sm_resp have no body (header only). */

#include "libsmpp5/smpp.h"
#include "smpp_internal.h"

smpp_status_t
smpp_query_sm_decode(const uint8_t *body, size_t body_length,
					 smpp_query_sm_t *query)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	query->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	query->source_addr_ton = smpp_priv_r_u8(&r);
	query->source_addr_npi = smpp_priv_r_u8(&r);
	query->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);

	return r.error;
}

smpp_status_t
smpp_query_sm_encode(const smpp_query_sm_t *query, uint8_t *buffer,
					 size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, query->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, query->source_addr_ton);
	smpp_priv_w_u8(&w, query->source_addr_npi);
	smpp_priv_w_cstring(&w, query->source_addr, SMPP_ADDR_MAX);

	return w.error;
}

size_t
smpp_query_sm_encoded_length(const smpp_query_sm_t *query)
{
	return smpp_priv_safe_strlen(query->message_id) + 1 + 2 +
		   smpp_priv_safe_strlen(query->source_addr) + 1;
}

smpp_status_t
smpp_query_sm_resp_decode(const uint8_t *body, size_t body_length,
						  smpp_query_sm_resp_t *resp)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	resp->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	resp->final_date = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	resp->message_state = smpp_priv_r_u8(&r);
	resp->error_code = smpp_priv_r_u8(&r);

	return r.error;
}

smpp_status_t
smpp_query_sm_resp_encode(const smpp_query_sm_resp_t *resp, uint8_t *buffer,
						  size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, resp->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_time_cstring(&w, resp->final_date);
	smpp_priv_w_u8(&w, resp->message_state);
	smpp_priv_w_u8(&w, resp->error_code);

	return w.error;
}

size_t
smpp_query_sm_resp_encoded_length(const smpp_query_sm_resp_t *resp)
{
	return smpp_priv_safe_strlen(resp->message_id) + 1 +
		   smpp_priv_safe_strlen(resp->final_date) + 1 + 2;
}

smpp_status_t
smpp_cancel_sm_decode(const uint8_t *body, size_t body_length,
					  smpp_cancel_sm_t *cancel)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	cancel->service_type = smpp_priv_r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	cancel->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	cancel->source_addr_ton = smpp_priv_r_u8(&r);
	cancel->source_addr_npi = smpp_priv_r_u8(&r);
	cancel->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);
	cancel->dest_addr_ton = smpp_priv_r_u8(&r);
	cancel->dest_addr_npi = smpp_priv_r_u8(&r);
	cancel->destination_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);

	return r.error;
}

smpp_status_t
smpp_cancel_sm_encode(const smpp_cancel_sm_t *cancel, uint8_t *buffer,
					  size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, cancel->service_type, SMPP_SERVICE_TYPE_MAX);
	smpp_priv_w_cstring(&w, cancel->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, cancel->source_addr_ton);
	smpp_priv_w_u8(&w, cancel->source_addr_npi);
	smpp_priv_w_cstring(&w, cancel->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_u8(&w, cancel->dest_addr_ton);
	smpp_priv_w_u8(&w, cancel->dest_addr_npi);
	smpp_priv_w_cstring(&w, cancel->destination_addr, SMPP_ADDR_MAX);

	return w.error;
}

size_t
smpp_cancel_sm_encoded_length(const smpp_cancel_sm_t *cancel)
{
	return smpp_priv_safe_strlen(cancel->service_type) + 1 +
		   smpp_priv_safe_strlen(cancel->message_id) + 1 + 2 +
		   smpp_priv_safe_strlen(cancel->source_addr) + 1 + 2 +
		   smpp_priv_safe_strlen(cancel->destination_addr) + 1;
}

smpp_status_t
smpp_replace_sm_decode(const uint8_t *body, size_t body_length,
					   smpp_replace_sm_t *replace)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	replace->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	replace->source_addr_ton = smpp_priv_r_u8(&r);
	replace->source_addr_npi = smpp_priv_r_u8(&r);
	replace->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);
	replace->schedule_delivery_time = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	replace->validity_period = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	replace->registered_delivery = smpp_priv_r_u8(&r);
	replace->sm_default_msg_id = smpp_priv_r_u8(&r);
	replace->short_message_length = smpp_priv_r_u8(&r);
	replace->short_message =
		smpp_priv_r_octets(&r, replace->short_message_length);

	smpp_priv_r_tlvs(&r, &replace->tlvs, &replace->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_replace_sm_encode(const smpp_replace_sm_t *replace, uint8_t *buffer,
					   size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, replace->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, replace->source_addr_ton);
	smpp_priv_w_u8(&w, replace->source_addr_npi);
	smpp_priv_w_cstring(&w, replace->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_time_cstring(&w, replace->schedule_delivery_time);
	smpp_priv_w_time_cstring(&w, replace->validity_period);
	smpp_priv_w_u8(&w, replace->registered_delivery);
	smpp_priv_w_u8(&w, replace->sm_default_msg_id);
	smpp_priv_w_u8(&w, replace->short_message_length);
	smpp_priv_w_bytes(&w, replace->short_message,
					  replace->short_message_length);
	smpp_priv_w_bytes(&w, replace->tlvs, replace->tlvs_length);

	return w.error;
}

size_t
smpp_replace_sm_encoded_length(const smpp_replace_sm_t *replace)
{
	return smpp_priv_safe_strlen(replace->message_id) + 1 + 2 +
		   smpp_priv_safe_strlen(replace->source_addr) + 1 +
		   smpp_priv_safe_strlen(replace->schedule_delivery_time) + 1 +
		   smpp_priv_safe_strlen(replace->validity_period) + 1 + 3 +
		   replace->short_message_length + replace->tlvs_length;
}
