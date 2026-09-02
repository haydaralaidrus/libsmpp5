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
smpp_data_sm_decode(const uint8_t *body, size_t body_length,
					smpp_data_sm_t *data)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	data->service_type = smpp_priv_r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	data->source_addr_ton = smpp_priv_r_u8(&r);
	data->source_addr_npi = smpp_priv_r_u8(&r);
	data->source_addr = smpp_priv_r_cstring(&r, SMPP_LONG_ADDR_MAX);
	data->dest_addr_ton = smpp_priv_r_u8(&r);
	data->dest_addr_npi = smpp_priv_r_u8(&r);
	data->destination_addr = smpp_priv_r_cstring(&r, SMPP_LONG_ADDR_MAX);
	data->esm_class = smpp_priv_r_u8(&r);
	data->registered_delivery = smpp_priv_r_u8(&r);
	data->data_coding = smpp_priv_r_u8(&r);

	smpp_priv_r_tlvs(&r, &data->tlvs, &data->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_data_sm_encode(const smpp_data_sm_t *data, uint8_t *buffer,
					size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, data->service_type, SMPP_SERVICE_TYPE_MAX);
	smpp_priv_w_u8(&w, data->source_addr_ton);
	smpp_priv_w_u8(&w, data->source_addr_npi);
	smpp_priv_w_cstring(&w, data->source_addr, SMPP_LONG_ADDR_MAX);
	smpp_priv_w_u8(&w, data->dest_addr_ton);
	smpp_priv_w_u8(&w, data->dest_addr_npi);
	smpp_priv_w_cstring(&w, data->destination_addr, SMPP_LONG_ADDR_MAX);
	smpp_priv_w_u8(&w, data->esm_class);
	smpp_priv_w_u8(&w, data->registered_delivery);
	smpp_priv_w_u8(&w, data->data_coding);
	smpp_priv_w_bytes(&w, data->tlvs, data->tlvs_length);

	return w.error;
}

size_t
smpp_data_sm_encoded_length(const smpp_data_sm_t *data)
{
	return smpp_priv_safe_strlen(data->service_type) + 1 + 2 +
		   smpp_priv_safe_strlen(data->source_addr) + 1 + 2 +
		   smpp_priv_safe_strlen(data->destination_addr) + 1 + 3 +
		   data->tlvs_length;
}
