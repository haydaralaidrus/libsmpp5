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

#include "libsmpp5/smpp.h"
#include "smpp_internal.h"

smpp_status_t
smpp_sm_decode(const uint8_t *body, size_t body_length, smpp_sm_t *sm)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	sm->service_type = smpp_priv_r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	sm->source_addr_ton = smpp_priv_r_u8(&r);
	sm->source_addr_npi = smpp_priv_r_u8(&r);
	sm->source_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);
	sm->dest_addr_ton = smpp_priv_r_u8(&r);
	sm->dest_addr_npi = smpp_priv_r_u8(&r);
	sm->destination_addr = smpp_priv_r_cstring(&r, SMPP_ADDR_MAX);
	sm->esm_class = smpp_priv_r_u8(&r);
	sm->protocol_id = smpp_priv_r_u8(&r);
	sm->priority_flag = smpp_priv_r_u8(&r);
	sm->schedule_delivery_time = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	sm->validity_period = smpp_priv_r_cstring(&r, SMPP_TIME_MAX);
	sm->registered_delivery = smpp_priv_r_u8(&r);
	sm->replace_if_present_flag = smpp_priv_r_u8(&r);
	sm->data_coding = smpp_priv_r_u8(&r);
	sm->sm_default_msg_id = smpp_priv_r_u8(&r);
	sm->short_message_length = smpp_priv_r_u8(&r);
	sm->short_message = smpp_priv_r_octets(&r, sm->short_message_length);

	smpp_priv_r_tlvs(&r, &sm->tlvs, &sm->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_sm_encode(const smpp_sm_t *sm, uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, sm->service_type, SMPP_SERVICE_TYPE_MAX);
	smpp_priv_w_u8(&w, sm->source_addr_ton);
	smpp_priv_w_u8(&w, sm->source_addr_npi);
	smpp_priv_w_cstring(&w, sm->source_addr, SMPP_ADDR_MAX);
	smpp_priv_w_u8(&w, sm->dest_addr_ton);
	smpp_priv_w_u8(&w, sm->dest_addr_npi);
	smpp_priv_w_cstring(&w, sm->destination_addr, SMPP_ADDR_MAX);
	smpp_priv_w_u8(&w, sm->esm_class);
	smpp_priv_w_u8(&w, sm->protocol_id);
	smpp_priv_w_u8(&w, sm->priority_flag);
	smpp_priv_w_time_cstring(&w, sm->schedule_delivery_time);
	smpp_priv_w_time_cstring(&w, sm->validity_period);
	smpp_priv_w_u8(&w, sm->registered_delivery);
	smpp_priv_w_u8(&w, sm->replace_if_present_flag);
	smpp_priv_w_u8(&w, sm->data_coding);
	smpp_priv_w_u8(&w, sm->sm_default_msg_id);
	smpp_priv_w_u8(&w, sm->short_message_length);
	smpp_priv_w_bytes(&w, sm->short_message, sm->short_message_length);
	smpp_priv_w_bytes(&w, sm->tlvs, sm->tlvs_length);

	return w.error;
}

size_t
smpp_sm_encoded_length(const smpp_sm_t *sm)
{
	return smpp_priv_safe_strlen(sm->service_type) + 1 + 2 +
		   smpp_priv_safe_strlen(sm->source_addr) + 1 + 2 +
		   smpp_priv_safe_strlen(sm->destination_addr) + 1 + 3 +
		   smpp_priv_safe_strlen(sm->schedule_delivery_time) + 1 +
		   smpp_priv_safe_strlen(sm->validity_period) + 1 + 5 +
		   sm->short_message_length + sm->tlvs_length;
}

smpp_status_t
smpp_sm_resp_decode(const uint8_t *body, size_t body_length,
					smpp_sm_resp_t *resp)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	resp->message_id = smpp_priv_r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	smpp_priv_r_tlvs(&r, &resp->tlvs, &resp->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_sm_resp_encode(const smpp_sm_resp_t *resp, uint8_t *buffer,
					size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, resp->message_id, SMPP_MESSAGE_ID_MAX);
	smpp_priv_w_bytes(&w, resp->tlvs, resp->tlvs_length);

	return w.error;
}

size_t
smpp_sm_resp_encoded_length(const smpp_sm_resp_t *resp)
{
	return smpp_priv_safe_strlen(resp->message_id) + 1 + resp->tlvs_length;
}
