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
smpp_alert_notification_decode(const uint8_t *body, size_t body_length,
							   smpp_alert_notification_t *alert)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	alert->source_addr_ton = smpp_priv_r_u8(&r);
	alert->source_addr_npi = smpp_priv_r_u8(&r);
	alert->source_addr = smpp_priv_r_cstring(&r, SMPP_LONG_ADDR_MAX);
	alert->esme_addr_ton = smpp_priv_r_u8(&r);
	alert->esme_addr_npi = smpp_priv_r_u8(&r);
	alert->esme_addr = smpp_priv_r_cstring(&r, SMPP_LONG_ADDR_MAX);
	smpp_priv_r_tlvs(&r, &alert->tlvs, &alert->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_alert_notification_encode(const smpp_alert_notification_t *alert,
							   uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	smpp_priv_w_u8(&w, alert->source_addr_ton);
	smpp_priv_w_u8(&w, alert->source_addr_npi);
	smpp_priv_w_cstring(&w, alert->source_addr, SMPP_LONG_ADDR_MAX);
	smpp_priv_w_u8(&w, alert->esme_addr_ton);
	smpp_priv_w_u8(&w, alert->esme_addr_npi);
	smpp_priv_w_cstring(&w, alert->esme_addr, SMPP_LONG_ADDR_MAX);
	smpp_priv_w_bytes(&w, alert->tlvs, alert->tlvs_length);

	return w.error;
}

size_t
smpp_alert_notification_encoded_length(const smpp_alert_notification_t *alert)
{
	return 2 + smpp_priv_safe_strlen(alert->source_addr) + 1 + 2 +
		   smpp_priv_safe_strlen(alert->esme_addr) + 1 + alert->tlvs_length;
}
