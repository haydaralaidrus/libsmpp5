#ifndef LIBSMPP5_PDU_H
#define LIBSMPP5_PDU_H

#include <stddef.h>
#include <stdint.h>

#define PDU_HEADER_LENGTH 16u

/**
 * \enum pdu_status_t
 * \brief Result of a decode/encode operation.
 */
typedef enum pdu_status_t {
	PDU_OK = 0,
	PDU_ERR_INCOMPLETE, /** Fewer octets in the buffer than the PDU needs. */
	PDU_ERR_INVALID_LENGTH, /** command_length smaller than PDU_HEADER_LENGTH. */
	PDU_ERR_BUFFER_TOO_SMALL, /** Destination buffer given is too small. */
} pdu_status_t;

/**
 * \struct pdu_header_t
 * \brief The mandatory 16-octet header.
 */
typedef struct pdu_header_t {
	uint32_t command_length; /** Size of the whole PDU including header. */
	uint32_t command_id; /** Identifies the SMPP operation. */
	uint32_t command_status; /** SMPP error code; 0 on requests and successful
	                             responses. */
	uint32_t sequence_number; /** Correlates a response with the request. */
} pdu_header_t;

/**
 * \struct pdu_t
 * \brief A generic SMPP PDU.
 */
typedef struct pdu_t {
	pdu_header_t header;
	const uint8_t *body; /** Body octets or NULL when body_length is 0. */
	uint32_t body_length;
} pdu_t;

/**
 * \brief Decode a single PDU from the front of buffer.
 * \param buffer Octets received so far.
 * \param buffer_length Octets available in buffer.
 * \param pdu Out param.
 */
pdu_status_t
pdu_decode(const uint8_t *buffer, size_t buffer_length, pdu_t *pdu);

/**
 * \brief Encode pdu into buffer, deriving command_length from body_length.
 * \param buffer Destination.
 */
pdu_status_t
pdu_encode(const pdu_t *pdu, uint8_t *buffer, size_t buffer_length);

/** \brief Octets needed to write pdu. */
size_t
pdu_encoded_length(const pdu_t *pdu);

#endif
