#include "pdu.h"

#include <string.h>

/** reads 4 bytes at p as a big-endian uint32_t */
static uint32_t
read_u32be(const uint8_t *p);

/** writes into p as 4 big-endian bytes */
static void
write_u32be(uint8_t *p, uint32_t value);

pdu_status_t
pdu_decode(const uint8_t *buffer, size_t buffer_length, pdu_t *pdu)
{
	if (buffer_length < PDU_HEADER_LENGTH)
		return PDU_ERR_INCOMPLETE;

	uint32_t command_length = read_u32be(buffer);
	if (command_length < PDU_HEADER_LENGTH)
		return PDU_ERR_INVALID_LENGTH;

	if (buffer_length < (size_t)command_length)
		return PDU_ERR_INCOMPLETE;

	pdu->header.command_length = command_length;
	pdu->header.command_id = read_u32be(buffer + 4);
	pdu->header.command_status = read_u32be(buffer + 8);
	pdu->header.sequence_number = read_u32be(buffer + 12);

	pdu->body_length = command_length - PDU_HEADER_LENGTH;
	pdu->body = pdu->body_length > 0 ? buffer + PDU_HEADER_LENGTH : NULL;

	return PDU_OK;
}

pdu_status_t
pdu_encode(const pdu_t *pdu, uint8_t *buffer, size_t buffer_length)
{
	size_t total_length = pdu_encoded_length(pdu);
	if (buffer_length < total_length)
		return PDU_ERR_BUFFER_TOO_SMALL;

	write_u32be(buffer, (uint32_t)total_length);
	write_u32be(buffer + 4, pdu->header.command_id);
	write_u32be(buffer + 8, pdu->header.command_status);
	write_u32be(buffer + 12, pdu->header.sequence_number);

	if (pdu->body_length > 0)
		memcpy(buffer + PDU_HEADER_LENGTH, pdu->body, pdu->body_length);

	return PDU_OK;
}

size_t
pdu_encoded_length(const pdu_t *pdu)
{
	return (size_t)PDU_HEADER_LENGTH + pdu->body_length;
}

static uint32_t
read_u32be(const uint8_t *p)
{
	return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) |
		   ((uint32_t)p[2] << 8) | (uint32_t)p[3];
}

static void
write_u32be(uint8_t *p, uint32_t value)
{
	p[0] = (uint8_t)(value >> 24);
	p[1] = (uint8_t)(value >> 16);
	p[2] = (uint8_t)(value >> 8);
	p[3] = (uint8_t)value;
}
