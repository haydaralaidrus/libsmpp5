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

#ifndef LIBSMPP5_SMPP_INTERNAL_H
#define LIBSMPP5_SMPP_INTERNAL_H

#include "libsmpp5/smpp.h"

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

/* Returns string length, or 0 if s is NULL */
size_t
smpp_priv_safe_strlen(const char *s);

/* Writes one byte to the buffer */
void
smpp_priv_w_u8(writer_t *w, uint8_t v);

/* Writes v as 4 big-endian bytes */
void
smpp_priv_w_u32(writer_t *w, uint32_t v);

/* Writes n raw bytes to the buffer */
void
smpp_priv_w_bytes(writer_t *w, const uint8_t *data, size_t n);

/* Writes a null-terminated string, capped at max_len */
void
smpp_priv_w_cstring(writer_t *w, const char *s, size_t max_len);

/* Writes an SMPP time string (must be empty or 16 chars) */
void
smpp_priv_w_time_cstring(writer_t *w, const char *s);

/* Reads one byte from the buffer */
uint8_t
smpp_priv_r_u8(reader_t *r);

/* Reads 4 bytes as a big-endian uint32_t */
uint32_t
smpp_priv_r_u32(reader_t *r);

/* Reads n raw bytes and return a pointer to them */
const uint8_t *
smpp_priv_r_octets(reader_t *r, size_t n);

/* Reads a null-terminated string, up to max_len bytes */
const char *
smpp_priv_r_cstring(reader_t *r, size_t max_len);

/* Reads the remaining bytes as TLV data */
void
smpp_priv_r_tlvs(reader_t *r, const uint8_t **tlvs, uint16_t *tlvs_length);

#endif
