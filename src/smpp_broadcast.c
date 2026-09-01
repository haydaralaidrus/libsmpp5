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

/* Spans spec section 4.4 "Message Broadcast Operations" (broadcast_sm) and
 * section 4.6 "Ancillary Broadcast Operations" (query_broadcast_sm,
 * cancel_broadcast_sm), grouped here by feature area rather than by the
 * spec's own section split, since a consumer implementing broadcast support
 * thinks in terms of the feature, not the spec's table of contents.
 * broadcast_sm_resp/query_broadcast_sm_resp reuse smpp_sm_resp_t (identical
 * shape); cancel_broadcast_sm_resp has no body (header only). */

#include "libsmpp5/smpp.h"
#include "smpp_internal.h"

smpp_status_t
smpp_broadcast_sm_decode(const uint8_t *body, size_t body_length,
						 smpp_broadcast_sm_t *broadcast)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	broadcast->service_type = smpp_priv_r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	broadcast->source_addr_ton = smpp_priv_r_u8(&r);
	broadcast->source_addr_npi = smpp_priv_r_u8(&r);
	broadcast->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);
	broadcast->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	broadcast->priority_flag = smpp_priv_r_u8(&r);
	broadcast->schedule_delivery_time = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	broadcast->validity_period = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	broadcast->replace_if_present_flag = smpp_priv_r_u8(&r);
	broadcast->data_coding = smpp_priv_r_u8(&r);
	broadcast->sm_default_msg_id = smpp_priv_r_u8(&r);

	smpp_priv_r_tlvs(&r, &broadcast->tlvs, &broadcast->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_broadcast_sm_encode(const smpp_broadcast_sm_t *broadcast, uint8_t *buffer,
						 size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, broadcast->service_type, SMPP_SERVICE_TYPE_MAX);
	smpp_priv_w_u8(&w, broadcast->source_addr_ton);
	smpp_priv_w_u8(&w, broadcast->source_addr_npi);
	smpp_priv_w_cstring(&w, broadcast->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_cstring(&w, broadcast->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, broadcast->priority_flag);
	smpp_priv_w_time_cstring(&w, broadcast->schedule_delivery_time);
	smpp_priv_w_time_cstring(&w, broadcast->validity_period);
	smpp_priv_w_u8(&w, broadcast->replace_if_present_flag);
	smpp_priv_w_u8(&w, broadcast->data_coding);
	smpp_priv_w_u8(&w, broadcast->sm_default_msg_id);
	smpp_priv_w_bytes(&w, broadcast->tlvs, broadcast->tlvs_length);

	return w.error;
}

size_t
smpp_broadcast_sm_encoded_length(const smpp_broadcast_sm_t *broadcast)
{
	return smpp_priv_safe_strlen(broadcast->service_type) + 1 + 2 +
		   smpp_priv_safe_strlen(broadcast->source_addr) + 1 +
		   smpp_priv_safe_strlen(broadcast->message_id) + 1 + 1 +
		   smpp_priv_safe_strlen(broadcast->schedule_delivery_time) + 1 +
		   smpp_priv_safe_strlen(broadcast->validity_period) + 1 + 3 +
		   broadcast->tlvs_length;
}

smpp_status_t
smpp_query_broadcast_sm_decode(const uint8_t *body, size_t body_length,
							   smpp_query_broadcast_sm_t *query)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	query->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	query->source_addr_ton = smpp_priv_r_u8(&r);
	query->source_addr_npi = smpp_priv_r_u8(&r);
	query->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);

	smpp_priv_r_tlvs(&r, &query->tlvs, &query->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_query_broadcast_sm_encode(const smpp_query_broadcast_sm_t *query,
							   uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, query->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, query->source_addr_ton);
	smpp_priv_w_u8(&w, query->source_addr_npi);
	smpp_priv_w_cstring(&w, query->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_bytes(&w, query->tlvs, query->tlvs_length);

	return w.error;
}

size_t
smpp_query_broadcast_sm_encoded_length(const smpp_query_broadcast_sm_t *query)
{
	return smpp_priv_safe_strlen(query->message_id) + 1 + 2 +
		   smpp_priv_safe_strlen(query->source_addr) + 1 + query->tlvs_length;
}

smpp_status_t
smpp_cancel_broadcast_sm_decode(const uint8_t *body, size_t body_length,
								smpp_cancel_broadcast_sm_t *cancel)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	cancel->service_type = smpp_priv_r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	cancel->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	cancel->source_addr_ton = smpp_priv_r_u8(&r);
	cancel->source_addr_npi = smpp_priv_r_u8(&r);
	cancel->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);

	smpp_priv_r_tlvs(&r, &cancel->tlvs, &cancel->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_cancel_broadcast_sm_encode(const smpp_cancel_broadcast_sm_t *cancel,
								uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, cancel->service_type, SMPP_SERVICE_TYPE_MAX);
	smpp_priv_w_cstring(&w, cancel->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_u8(&w, cancel->source_addr_ton);
	smpp_priv_w_u8(&w, cancel->source_addr_npi);
	smpp_priv_w_cstring(&w, cancel->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_bytes(&w, cancel->tlvs, cancel->tlvs_length);

	return w.error;
}

size_t
smpp_cancel_broadcast_sm_encoded_length(const smpp_cancel_broadcast_sm_t *cancel)
{
	return smpp_priv_safe_strlen(cancel->service_type) + 1 +
		   smpp_priv_safe_strlen(cancel->message_id) + 1 + 2 +
		   smpp_priv_safe_strlen(cancel->source_addr) + 1 + cancel->tlvs_length;
}
