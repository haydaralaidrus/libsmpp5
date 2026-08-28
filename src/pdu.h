#ifndef LIBSMPP5_PDU_H
#define LIBSMPP5_PDU_H

#include <stdint.h>

#define PDU_HEADER_LENGTH (sizeof(uint8_t) * 16)
#define PDU_NULL_1 ((uint8_t)0x00) /** 1-octet NULL */
#define PDU_NULL_2 ((uint16_t)0x0000) /** 2-octet NULL */
#define PDU_NULL_4 ((uint32_t)0x00000000) /** 4-octet NULL */

/**
 * \struct pdu_header_t
 */
typedef struct pdu_header_t {
	uint8_t cmdlen[4]; /** Overall size of PDU including header and body. */
	uint8_t cmdid[4]; /** Identifies the PDU. */
	uint8_t cmdstatus[4]; /** Used to carry SMPP error code. */
	uint8_t seqnum[4]; /** Used to uniquely identify a SMPP PDU in the
	                       context of a SMPP session. */
} pdu_header_t;

/**
 * \struct pdu_body_t
 */
typedef struct pdu_body_t {
	/** TODO: standard parameters & tlv */
} pdu_body_t;

/**
 * \struct pdu_t
 * \brief Generic Protocol Data Unit (PDU) containing SMPP command.
 */
typedef struct pdu_t {
	pdu_header_t header;
} pdu_t;

#endif
