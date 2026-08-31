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

#include "smpp.h"

typedef struct writer_t {
	uint8_t *buf;
	size_t len;
	size_t pos;
	smpp_status_t error;
} writer_t;

typedef struct reader_t {
	const uint8_t *buf;
	size_t len;
	size_t pos;
	smpp_status_t error;
} reader_t;

/* Return string length, or 0 if s is NULL */
static size_t
safe_strlen(const char *s);

/* Write one byte to the buffer */
static void
w_u8(writer_t *w, uint8_t v);

/* Write n raw bytes to the buffer */
static void
w_bytes(writer_t *w, const uint8_t *data, size_t n);

/* Write a null-terminated string, capped at max_len */
static void
w_cstring(writer_t *w, const char *s, size_t max_len);

/* Write an SMPP time string (must be empty or 16 chars) */
static void
w_time_cstring(writer_t *w, const char *s);

/* Read one byte from the buffer */
static uint8_t
r_u8(reader_t *r);

/* Read n raw bytes and return a pointer to them */
static const uint8_t *
r_octets(reader_t *r, size_t n);

/* Read a null-terminated string, up to max_len bytes */
static const char *
r_cstring(reader_t *r, size_t max_len);

/* Read the remaining bytes as TLV data */
static void
r_tlvs(reader_t *r, const uint8_t **tlvs, uint16_t *tlvs_length);

smpp_status_t
smpp_bind_decode(const uint8_t *body, size_t body_length, smpp_bind_t *bind)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	bind->system_id = r_cstring(&r, SMPP_SYSTEM_ID_MAX);
	bind->password = r_cstring(&r, SMPP_PASSWORD_MAX);
	bind->system_type = r_cstring(&r, SMPP_SYSTEM_TYPE_MAX);
	bind->interface_version = r_u8(&r);
	bind->addr_ton = r_u8(&r);
	bind->addr_npi = r_u8(&r);
	bind->address_range = r_cstring(&r, SMPP_ADDRESS_RANGE_MAX);

	return r.error;
}

smpp_status_t
smpp_bind_encode(const smpp_bind_t *bind, uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	w_cstring(&w, bind->system_id, SMPP_SYSTEM_ID_MAX);
	w_cstring(&w, bind->password, SMPP_PASSWORD_MAX);
	w_cstring(&w, bind->system_type, SMPP_SYSTEM_TYPE_MAX);
	w_u8(&w, bind->interface_version);
	w_u8(&w, bind->addr_ton);
	w_u8(&w, bind->addr_npi);
	w_cstring(&w, bind->address_range, SMPP_ADDRESS_RANGE_MAX);

	return w.error;
}

size_t
smpp_bind_encoded_length(const smpp_bind_t *bind)
{
	return safe_strlen(bind->system_id) + 1 + safe_strlen(bind->password) + 1 +
		   safe_strlen(bind->system_type) + 1 + 3 +
		   safe_strlen(bind->address_range) + 1;
}

smpp_status_t
smpp_bind_resp_decode(const uint8_t *body, size_t body_length,
					  smpp_bind_resp_t *resp)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	resp->system_id = r_cstring(&r, SMPP_SYSTEM_ID_MAX);
	r_tlvs(&r, &resp->tlvs, &resp->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_bind_resp_encode(const smpp_bind_resp_t *resp, uint8_t *buffer,
					  size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	w_cstring(&w, resp->system_id, SMPP_SYSTEM_ID_MAX);
	w_bytes(&w, resp->tlvs, resp->tlvs_length);

	return w.error;
}

size_t
smpp_bind_resp_encoded_length(const smpp_bind_resp_t *resp)
{
	return safe_strlen(resp->system_id) + 1 + resp->tlvs_length;
}

smpp_status_t
smpp_sm_decode(const uint8_t *body, size_t body_length, smpp_sm_t *sm)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	sm->service_type = r_cstring(&r, SMPP_SERVICE_TYPE_MAX);
	sm->source_addr_ton = r_u8(&r);
	sm->source_addr_npi = r_u8(&r);
	sm->source_addr = r_cstring(&r, SMPP_ADDR_MAX);
	sm->dest_addr_ton = r_u8(&r);
	sm->dest_addr_npi = r_u8(&r);
	sm->destination_addr = r_cstring(&r, SMPP_ADDR_MAX);
	sm->esm_class = r_u8(&r);
	sm->protocol_id = r_u8(&r);
	sm->priority_flag = r_u8(&r);
	sm->schedule_delivery_time = r_cstring(&r, SMPP_TIME_MAX);
	sm->validity_period = r_cstring(&r, SMPP_TIME_MAX);
	sm->registered_delivery = r_u8(&r);
	sm->replace_if_present_flag = r_u8(&r);
	sm->data_coding = r_u8(&r);
	sm->sm_default_msg_id = r_u8(&r);
	sm->short_message_length = r_u8(&r);
	sm->short_message = r_octets(&r, sm->short_message_length);

	r_tlvs(&r, &sm->tlvs, &sm->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_sm_encode(const smpp_sm_t *sm, uint8_t *buffer, size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	w_cstring(&w, sm->service_type, SMPP_SERVICE_TYPE_MAX);
	w_u8(&w, sm->source_addr_ton);
	w_u8(&w, sm->source_addr_npi);
	w_cstring(&w, sm->source_addr, SMPP_ADDR_MAX);
	w_u8(&w, sm->dest_addr_ton);
	w_u8(&w, sm->dest_addr_npi);
	w_cstring(&w, sm->destination_addr, SMPP_ADDR_MAX);
	w_u8(&w, sm->esm_class);
	w_u8(&w, sm->protocol_id);
	w_u8(&w, sm->priority_flag);
	w_time_cstring(&w, sm->schedule_delivery_time);
	w_time_cstring(&w, sm->validity_period);
	w_u8(&w, sm->registered_delivery);
	w_u8(&w, sm->replace_if_present_flag);
	w_u8(&w, sm->data_coding);
	w_u8(&w, sm->sm_default_msg_id);
	w_u8(&w, sm->short_message_length);
	w_bytes(&w, sm->short_message, sm->short_message_length);
	w_bytes(&w, sm->tlvs, sm->tlvs_length);

	return w.error;
}

size_t
smpp_sm_encoded_length(const smpp_sm_t *sm)
{
	return safe_strlen(sm->service_type) + 1 + 2 +
		   safe_strlen(sm->source_addr) + 1 + 2 +
		   safe_strlen(sm->destination_addr) + 1 + 3 +
		   safe_strlen(sm->schedule_delivery_time) + 1 +
		   safe_strlen(sm->validity_period) + 1 + 5 + sm->short_message_length +
		   sm->tlvs_length;
}

smpp_status_t
smpp_sm_resp_decode(const uint8_t *body, size_t body_length,
					smpp_sm_resp_t *resp)
{
	reader_t r = { body, body_length, 0, SMPP_OK };

	resp->message_id = r_cstring(&r, SMPP_MESSAGE_ID_MAX);
	r_tlvs(&r, &resp->tlvs, &resp->tlvs_length);

	return r.error;
}

smpp_status_t
smpp_sm_resp_encode(const smpp_sm_resp_t *resp, uint8_t *buffer,
					size_t buffer_length)
{
	writer_t w = { buffer, buffer_length, 0, SMPP_OK };

	w_cstring(&w, resp->message_id, SMPP_MESSAGE_ID_MAX);
	w_bytes(&w, resp->tlvs, resp->tlvs_length);

	return w.error;
}

size_t
smpp_sm_resp_encoded_length(const smpp_sm_resp_t *resp)
{
	return safe_strlen(resp->message_id) + 1 + resp->tlvs_length;
}

static size_t
safe_strlen(const char *s)
{
	return s ? strlen(s) : 0;
}

static void
w_u8(writer_t *w, uint8_t v)
{
	if (w->error)
		return;

	if (w->pos + 1 > w->len) {
		w->error = SMPP_ERR_BUFFER_TOO_SMALL;
		return;
	}

	w->buf[w->pos++] = v;
}

static void
w_bytes(writer_t *w, const uint8_t *data, size_t n)
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

static void
w_cstring(writer_t *w, const char *s, size_t max_len)
{
	if (w->error)
		return;

	size_t len = safe_strlen(s);
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

static void
w_time_cstring(writer_t *w, const char *s)
{
	if (w->error)
		return;

	size_t len = safe_strlen(s);
	if (len != 0 && len != 16) {
		w->error = SMPP_ERR_INVALID;
		return;
	}

	w_cstring(w, s, SMPP_TIME_MAX);
}

static uint8_t
r_u8(reader_t *r)
{
	if (r->error)
		return 0;

	if (r->pos + 1 > r->len) {
		r->error = SMPP_ERR_INVALID;
		return 0;
	}

	return r->buf[r->pos++];
}

static const uint8_t *
r_octets(reader_t *r, size_t n)
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

static const char *
r_cstring(reader_t *r, size_t max_len)
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

static void
r_tlvs(reader_t *r, const uint8_t **tlvs, uint16_t *tlvs_length)
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
