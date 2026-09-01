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

#include "smpp_internal.h"

size_t
smpp_priv_safe_strlen(const char *s)
{
	return s ? strlen(s) : 0;
}

void
smpp_priv_w_u8(writer_t *w, uint8_t v)
{
	if (w->error)
		return;

	if (w->pos + 1 > w->len) {
		w->error = SMPP_ERR_BUFFER_TOO_SMALL;
		return;
	}

	w->buf[w->pos++] = v;
}

void
smpp_priv_w_bytes(writer_t *w, const uint8_t *data, size_t n)
{
	if (w->error || n == 0)
		return;

	if (w->pos + n > w->len) {
		w->error = SMPP_ERR_BUFFER_TOO_SMALL;
		return;
	}

	memcpy(w->buf + w->pos, data, n);
	w->pos += n;
}

void
smpp_priv_w_cstring(writer_t *w, const char *s, size_t max_len)
{
	if (w->error)
		return;

	size_t len = smpp_priv_safe_strlen(s);
	if (len + 1 > max_len) {
		w->error = SMPP_ERR_INVALID;
		return;
	}

	if (w->pos + len + 1 > w->len) {
		w->error = SMPP_ERR_BUFFER_TOO_SMALL;
		return;
	}

	if (len > 0)
		memcpy(w->buf + w->pos, s, len);

	w->buf[w->pos + len] = '\0';
	w->pos += len + 1;
}

void
smpp_priv_w_time_cstring(writer_t *w, const char *s)
{
	if (w->error)
		return;

	size_t len = smpp_priv_safe_strlen(s);
	if (len != 0 && len != 16) {
		w->error = SMPP_ERR_INVALID;
		return;
	}

	smpp_priv_w_cstring(w, s, SMPP_TIME_MAX);
}

uint8_t
smpp_priv_r_u8(reader_t *r)
{
	if (r->error)
		return 0;

	if (r->pos + 1 > r->len) {
		r->error = SMPP_ERR_INVALID;
		return 0;
	}

	return r->buf[r->pos++];
}

const uint8_t *
smpp_priv_r_octets(reader_t *r, size_t n)
{
	if (r->error || n == 0)
		return NULL;

	if (r->len - r->pos < n) {
		r->error = SMPP_ERR_INVALID;
		return NULL;
	}

	const uint8_t *result = r->buf + r->pos;
	r->pos += n;
	return result;
}

const char *
smpp_priv_r_cstring(reader_t *r, size_t max_len)
{
	if (r->error)
		return NULL;

	size_t window = r->len - r->pos;
	if (window > max_len)
		window = max_len;

	for (size_t i = 0; i < window; i++) {
		if (r->buf[r->pos + i] == 0x00) {
			const char *result = (const char *)(r->buf + r->pos);
			r->pos += i + 1;
			return result;
		}
	}

	r->error = SMPP_ERR_INVALID;
	return NULL;
}

void
smpp_priv_r_tlvs(reader_t *r, const uint8_t **tlvs, uint16_t *tlvs_length)
{
	if (r->error) {
		*tlvs = NULL;
		*tlvs_length = 0;
		return;
	}

	size_t remaining = r->len - r->pos;
	if (remaining > UINT16_MAX) {
		r->error = SMPP_ERR_INVALID;
		*tlvs = NULL;
		*tlvs_length = 0;
		return;
	}

	*tlvs = remaining > 0 ? r->buf + r->pos : NULL;
	*tlvs_length = (uint16_t)remaining;
	r->pos = r->len;
}
