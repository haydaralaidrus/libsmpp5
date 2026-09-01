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
smpp_bind_decode(const uint8_t *body, size_t body_length, smpp_bind_t *bind)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	bind->system_id = smpp_priv_r_cstring(&r, SMPP_SYSTEM_ID_MAX);
	bind->password = smpp_priv_r_cstring(&r, SMPP_PASSWORD_MAX);
	bind->system_type = smpp_priv_r_cstring(&r, SMPP_SYSTEM_TYPE_MAX);
	bind->interface_version = smpp_priv_r_u8(&r);
	bind->addr_ton = smpp_priv_r_u8(&r);
	bind->addr_npi = smpp_priv_r_u8(&r);
	bind->address_range = smpp_priv_r_cstring(&r, SMPP_ADDRESS_RANGE_MAX);

	return r.error;
}

smpp_status_t
smpp_bind_encode(const smpp_bind_t *bind, uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, bind->system_id, SMPP_SYSTEM_ID_MAX);
	smpp_priv_w_cstring(&w, bind->password, SMPP_PASSWORD_MAX);
	smpp_priv_w_cstring(&w, bind->system_type, SMPP_SYSTEM_TYPE_MAX);
	smpp_priv_w_u8(&w, bind->interface_version);
	smpp_priv_w_u8(&w, bind->addr_ton);
	smpp_priv_w_u8(&w, bind->addr_npi);
	smpp_priv_w_cstring(&w, bind->address_range, SMPP_ADDRESS_RANGE_MAX);

	return w.error;
}

size_t
smpp_bind_encoded_length(const smpp_bind_t *bind)
{
	return smpp_priv_safe_strlen(bind->system_id) + 1 +
		   smpp_priv_safe_strlen(bind->password) + 1 +
		   smpp_priv_safe_strlen(bind->system_type) + 1 + 3 +
		   smpp_priv_safe_strlen(bind->address_range) + 1;
}

smpp_status_t
smpp_bind_resp_decode(const uint8_t *body, size_t body_length,
					  smpp_bind_resp_t *resp)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	resp->system_id = smpp_priv_r_cstring(&r, SMPP_SYSTEM_ID_MAX);
	smpp_priv_r_tlvs(&r, &resp->tlvs, &resp->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_bind_resp_encode(const smpp_bind_resp_t *resp, uint8_t *buffer,
					  size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, resp->system_id, SMPP_SYSTEM_ID_MAX);
	smpp_priv_w_bytes(&w, resp->tlvs, resp->tlvs_length);

	return w.error;
}

size_t
smpp_bind_resp_encoded_length(const smpp_bind_resp_t *resp)
{
	return smpp_priv_safe_strlen(resp->system_id) + 1 + resp->tlvs_length;
}

smpp_status_t
smpp_outbind_decode(const uint8_t *body, size_t body_length,
					smpp_outbind_t *outbind)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	outbind->system_id = smpp_priv_r_cstring(&r, SMPP_SYSTEM_ID_MAX);
	outbind->password = smpp_priv_r_cstring(&r, SMPP_PASSWORD_MAX);

	return r.error;
}

smpp_status_t
smpp_outbind_encode(const smpp_outbind_t *outbind, uint8_t *buffer,
					size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_cstring(&w, outbind->system_id, SMPP_SYSTEM_ID_MAX);
	smpp_priv_w_cstring(&w, outbind->password, SMPP_PASSWORD_MAX);

	return w.error;
}

size_t
smpp_outbind_encoded_length(const smpp_outbind_t *outbind)
{
	return smpp_priv_safe_strlen(outbind->system_id) + 1 +
		   smpp_priv_safe_strlen(outbind->password) + 1;
}
