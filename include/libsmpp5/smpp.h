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

#ifndef LIBSMPP5_SMPP_H
#define LIBSMPP5_SMPP_H

#include <stddef.h>
#include <stdint.h>

/**
 * \file smpp.h
 * \brief SMPP command/status constants and operation-body codecs.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** SMPP requests */
#define SMPP_CMD_BIND_RECEIVER (0x00000001u)
#define SMPP_CMD_BIND_TRANSMITTER (0x00000002u)
#define SMPP_CMD_QUERY_SM (0x00000003u)
#define SMPP_CMD_SUBMIT_SM (0x00000004u)
#define SMPP_CMD_DELIVER_SM (0x00000005u)
#define SMPP_CMD_UNBIND (0x00000006u)
#define SMPP_CMD_REPLACE_SM (0x00000007u)
#define SMPP_CMD_CANCEL_SM (0x00000008u)
#define SMPP_CMD_BIND_TRANSCEIVER (0x00000009u)
#define SMPP_CMD_OUTBIND (0x0000000Bu)
#define SMPP_CMD_ENQUIRE_LINK (0x00000015u)
#define SMPP_CMD_SUBMIT_MULTI (0x00000021u)
#define SMPP_CMD_ALERT_NOTIFICATION (0x00000102u)
#define SMPP_CMD_DATA_SM (0x00000103u)
#define SMPP_CMD_BROADCAST_SM (0x00000111u)
#define SMPP_CMD_QUERY_BROADCAST_SM (0x00000112u)
#define SMPP_CMD_CANCEL_BROADCAST_SM (0x00000113u)

/** SMPP responses */
#define SMPP_CMD_GENERIC_NACK (0x80000000u)
#define SMPP_CMD_BIND_RECEIVER_RESP (0x80000001u)
#define SMPP_CMD_BIND_TRANSMITTER_RESP (0x80000002u)
#define SMPP_CMD_QUERY_SM_RESP (0x80000003u)
#define SMPP_CMD_SUBMIT_SM_RESP (0x80000004u)
#define SMPP_CMD_DELIVER_SM_RESP (0x80000005u)
#define SMPP_CMD_UNBIND_RESP (0x80000006u)
#define SMPP_CMD_REPLACE_SM_RESP (0x80000007u)
#define SMPP_CMD_CANCEL_SM_RESP (0x80000008u)
#define SMPP_CMD_BIND_TRANSCEIVER_RESP (0x80000009u)
#define SMPP_CMD_ENQUIRE_LINK_RESP (0x80000015u)
#define SMPP_CMD_SUBMIT_MULTI_RESP (0x80000021u)
#define SMPP_CMD_DATA_SM_RESP (0x80000103u)
#define SMPP_CMD_BROADCAST_SM_RESP (0x80000111u)
#define SMPP_CMD_QUERY_BROADCAST_SM_RESP (0x80000112u)
#define SMPP_CMD_CANCEL_BROADCAST_SM_RESP (0x80000113u)

/** SMPP command status */
#define SMPP_ESME_ROK (0x00000000u)
#define SMPP_ESME_RINVMSGLEN (0x00000001u)
#define SMPP_ESME_RINVCMDLEN (0x00000002u)
#define SMPP_ESME_RINVCMDID (0x00000003u)
#define SMPP_ESME_RINVBNDSTS (0x00000004u)
#define SMPP_ESME_RALYBND (0x00000005u)
#define SMPP_ESME_RINVPRTFLG (0x00000006u)
#define SMPP_ESME_RINVREGDLVFLG (0x00000007u)
#define SMPP_ESME_RSYSERR (0x00000008u)
#define SMPP_ESME_RINVSRCADR (0x0000000Au)
#define SMPP_ESME_RINVDSTADR (0x0000000Bu)
#define SMPP_ESME_RINVMSGID (0x0000000Cu)
#define SMPP_ESME_RBINDFAIL (0x0000000Du)
#define SMPP_ESME_RINVPASWD (0x0000000Eu)
#define SMPP_ESME_RINVSYSID (0x0000000Fu)
#define SMPP_ESME_RCANCELFAIL (0x00000011u)
#define SMPP_ESME_RREPLACEFAIL (0x00000013u)
#define SMPP_ESME_RMSGQFUL (0x00000014u)
#define SMPP_ESME_RINVSERTYP (0x00000015u)
#define SMPP_ESME_RINVNUMDESTS (0x00000033u)
#define SMPP_ESME_RINVDLNAME (0x00000034u)
#define SMPP_ESME_RINVDESTFLAG (0x00000040u)
#define SMPP_ESME_RINVSUBREP (0x00000042u)
#define SMPP_ESME_RINVESMCLASS (0x00000043u)
#define SMPP_ESME_RCNTSUBDL (0x00000044u)
#define SMPP_ESME_RSUBMITFAIL (0x00000045u)
#define SMPP_ESME_RINVSRCTON (0x00000048u)
#define SMPP_ESME_RINVSRCNPI (0x00000049u)
#define SMPP_ESME_RINVDSTTON (0x00000050u)
#define SMPP_ESME_RINVDSTNPI (0x00000051u)
#define SMPP_ESME_RINVSYSTYP (0x00000053u)
#define SMPP_ESME_RINVREPFLAG (0x00000054u)
#define SMPP_ESME_RINVNUMMSGS (0x00000055u)
#define SMPP_ESME_RTHROTTLED (0x00000058u)
#define SMPP_ESME_RINVSCHED (0x00000061u)
#define SMPP_ESME_RINVEXPIRY (0x00000062u)
#define SMPP_ESME_RINVDFTMSGID (0x00000063u)
#define SMPP_ESME_RX_T_APPN (0x00000064u)
#define SMPP_ESME_RX_P_APPN (0x00000065u)
#define SMPP_ESME_RX_R_APPN (0x00000066u)
#define SMPP_ESME_RQUERYFAIL (0x00000067u)
#define SMPP_ESME_RINVTLVSTREAM (0x000000C0u)
#define SMPP_ESME_RTLVNOTALLWD (0x000000C1u)
#define SMPP_ESME_RINVTLVLEN (0x000000C2u)
#define SMPP_ESME_RMISSINGTLV (0x000000C3u)
#define SMPP_ESME_RINVTLVVAL (0x000000C4u)
#define SMPP_ESME_RDELIVERYFAILURE (0x000000FEu)
#define SMPP_ESME_RUNKNOWNERR (0x000000FFu)
#define SMPP_ESME_RSERTYPUNAUTH (0x00000100u)
#define SMPP_ESME_RPROHIBITED (0x00000101u)
#define SMPP_ESME_RSERTYPUNAVAIL (0x00000102u)
#define SMPP_ESME_RSERTYPDENIED (0x00000103u)
#define SMPP_ESME_RINVDCS (0x00000104u)
#define SMPP_ESME_RINVSRCADDRSUBUNIT (0x00000105u)
#define SMPP_ESME_RINVDSTADDRSUBUNIT (0x00000106u)
#define SMPP_ESME_RINVBCASTFREQINT (0x00000107u)
#define SMPP_ESME_RINVBCASTALIAS_NAME (0x00000108u)
#define SMPP_ESME_RINVBCASTAREAFMT (0x00000109u)
#define SMPP_ESME_RINVNUMBCAST_AREAS (0x0000010Au)
#define SMPP_ESME_RINVBCASTCNTTYPE (0x0000010Bu)
#define SMPP_ESME_RINVBCASTMSGCLASS (0x0000010Cu)
#define SMPP_ESME_RBCASTFAIL (0x0000010Du)
#define SMPP_ESME_RBCASTQUERYFAIL (0x0000010Eu)
#define SMPP_ESME_RBCASTCANCELFAIL (0x0000010Fu)
#define SMPP_ESME_RINVBCAST_REP (0x00000110u)
#define SMPP_ESME_RINVBCASTSRVGRP (0x00000111u)
#define SMPP_ESME_RINVBCASTCHANIND (0x00000112u)

/** Type of number (TON) */
#define SMPP_TON_UNKNOWN (0x00u)
#define SMPP_TON_INTERNATIONAL (0x01u)
#define SMPP_TON_NATIONAL (0x02u)
#define SMPP_TON_NETWORK_SPECIFIC (0x03u)
#define SMPP_TON_SUBSCRIBER_NUMBER (0x04u)
#define SMPP_TON_ALPHANUMERIC (0x05u)
#define SMPP_TON_ABBREVIATED (0x06u)

/** Numbering plan indicator (NPI) */
#define SMPP_NPI_UNKNOWN (0x00u)
#define SMPP_NPI_ISDN (0x01u)
#define SMPP_NPI_DATA (0x03u)
#define SMPP_NPI_TELEX (0x04u)
#define SMPP_NPI_LAND_MOBILE (0x06u)
#define SMPP_NPI_NATIONAL (0x08u)
#define SMPP_NPI_PRIVATE (0x09u)
#define SMPP_NPI_ERMES (0x0Au)
#define SMPP_NPI_INTERNET (0x0Eu)
#define SMPP_NPI_WAP_CLIENT_ID (0x12u)

/** Interface version */
#define SMPP_INTERFACE_VERSION_3_4 (0x34u)
#define SMPP_INTERFACE_VERSION_5_0 (0x50u)

/** Data coding */
#define SMPP_DATA_CODING_DEFAULT (0x00u)
#define SMPP_DATA_CODING_IA5 (0x01u)
#define SMPP_DATA_CODING_OCTET_UNSPECIFIED (0x02u)
#define SMPP_DATA_CODING_LATIN1 (0x03u)
#define SMPP_DATA_CODING_JIS (0x05u)
#define SMPP_DATA_CODING_CYRILLIC (0x06u)
#define SMPP_DATA_CODING_LATIN_HEBREW (0x07u)
#define SMPP_DATA_CODING_UCS2 (0x08u)

/** ESM class */
#define SMPP_ESM_CLASS_DEFAULT (0x00u)
#define SMPP_ESM_CLASS_DATAGRAM (0x01u)
#define SMPP_ESM_CLASS_FORWARD (0x02u)
#define SMPP_ESM_CLASS_STORE_AND_FORWARD (0x03u)
#define SMPP_ESM_CLASS_MC_DELIVERY_RECEIPT (0x04u)
#define SMPP_ESM_CLASS_INTERMEDIATE_NOTIFICATION (0x20u)
#define SMPP_ESM_CLASS_UDHI (0x40u)
#define SMPP_ESM_CLASS_REPLY_PATH (0x80u)

/** Registered delivery */
#define SMPP_REGISTERED_DELIVERY_NONE (0x00u)
#define SMPP_REGISTERED_DELIVERY_SUCCESS_OR_FAILURE (0x01u)
#define SMPP_REGISTERED_DELIVERY_FAILURE (0x02u)
#define SMPP_REGISTERED_DELIVERY_SUCCESS (0x03u)
#define SMPP_REGISTERED_DELIVERY_SME_DELIVERY_ACK (0x04u)
#define SMPP_REGISTERED_DELIVERY_SME_USER_ACK (0x08u)
#define SMPP_REGISTERED_DELIVERY_INTERMEDIATE_NOTIFICATION (0x10u)

/** Replace if present flag */
#define SMPP_REPLACE_IF_PRESENT_NO (0x00u)
#define SMPP_REPLACE_IF_PRESENT_YES (0x01u)

/** dest_flag (submit_multi destination discriminator) */
#define SMPP_DEST_FLAG_SME_ADDRESS (0x01u)
#define SMPP_DEST_FLAG_DISTRIBUTION_LIST (0x02u)

/** message_state (query_sm_resp, and via TLV, query_broadcast_sm_resp) */
#define SMPP_MESSAGE_STATE_SCHEDULED (0x00u)
#define SMPP_MESSAGE_STATE_ENROUTE (0x01u)
#define SMPP_MESSAGE_STATE_DELIVERED (0x02u)
#define SMPP_MESSAGE_STATE_EXPIRED (0x03u)
#define SMPP_MESSAGE_STATE_DELETED (0x04u)
#define SMPP_MESSAGE_STATE_UNDELIVERABLE (0x05u)
#define SMPP_MESSAGE_STATE_ACCEPTED (0x06u)
#define SMPP_MESSAGE_STATE_UNKNOWN (0x07u)
#define SMPP_MESSAGE_STATE_REJECTED (0x08u)
#define SMPP_MESSAGE_STATE_SKIPPED (0x09u)

/** Field length limits */
#define SMPP_SYSTEM_ID_MAX (16u)
#define SMPP_PASSWORD_MAX (9u)
#define SMPP_SYSTEM_TYPE_MAX (13u)
#define SMPP_ADDRESS_RANGE_MAX (41u)
#define SMPP_SERVICE_TYPE_MAX (6u)
#define SMPP_ADDR_MAX (21u)
#define SMPP_LONG_ADDR_MAX (65u)
#define SMPP_DL_NAME_MAX (21u)
#define SMPP_TIME_MAX (17u)
#define SMPP_MESSAGE_ID_MAX (65u)

/**
 * \enum smpp_status_t
 * \brief Result of an SMPP operation.
 */
typedef enum smpp_status_t {
	SMPP_OK = 0,
	SMPP_ERR_INVALID,
	SMPP_ERR_BUFFER_TOO_SMALL,
} smpp_status_t;

/**
 * \struct smpp_bind_t
 * \brief bind_transmitter, bind_receiver, & bind_transceiver.
 */
typedef struct smpp_bind_t {
	const char *system_id;
	const char *password;
	const char *system_type;
	uint8_t interface_version;
	uint8_t addr_ton;
	uint8_t addr_npi;
	const char *address_range;
} smpp_bind_t;

/**
 * \brief Decodes bind requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param bind Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_bind_decode(const uint8_t *body, size_t body_length, smpp_bind_t *bind);

/**
 * \brief Encodes bind requests.
 * \param bind Bind to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_bind_encode(const smpp_bind_t *bind, uint8_t *buffer,
				 size_t buffer_length);

/**
 * \brief Octets needed to encode bind.
 * \param bind The bind.
 * \return Octets needed.
 */
size_t
smpp_bind_encoded_length(const smpp_bind_t *bind);

/**
 * \typedef smpp_bind_transmitter_t
 * \brief bind_transmitter; identical in shape to smpp_bind_t.
 */
typedef smpp_bind_t smpp_bind_transmitter_t;

/**
 * \brief Decodes bind_transmitter requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param bind Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transmitter_decode(const uint8_t *body, size_t body_length,
							 smpp_bind_transmitter_t *bind)
{
	return smpp_bind_decode(body, body_length, bind);
}

/**
 * \brief Encodes bind_transmitter requests.
 * \param bind Bind to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transmitter_encode(const smpp_bind_transmitter_t *bind,
							 uint8_t *buffer, size_t buffer_length)
{
	return smpp_bind_encode(bind, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode bind.
 * \param bind The bind.
 * \return Octets needed.
 */
static inline size_t
smpp_bind_transmitter_encoded_length(const smpp_bind_transmitter_t *bind)
{
	return smpp_bind_encoded_length(bind);
}

/**
 * \typedef smpp_bind_receiver_t
 * \brief bind_receiver; identical in shape to smpp_bind_t.
 */
typedef smpp_bind_t smpp_bind_receiver_t;

/**
 * \brief Decodes bind_receiver requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param bind Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_receiver_decode(const uint8_t *body, size_t body_length,
						  smpp_bind_receiver_t *bind)
{
	return smpp_bind_decode(body, body_length, bind);
}

/**
 * \brief Encodes bind_receiver requests.
 * \param bind Bind to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_receiver_encode(const smpp_bind_receiver_t *bind, uint8_t *buffer,
						  size_t buffer_length)
{
	return smpp_bind_encode(bind, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode bind.
 * \param bind The bind.
 * \return Octets needed.
 */
static inline size_t
smpp_bind_receiver_encoded_length(const smpp_bind_receiver_t *bind)
{
	return smpp_bind_encoded_length(bind);
}

/**
 * \typedef smpp_bind_transceiver_t
 * \brief bind_transceiver; identical in shape to smpp_bind_t.
 */
typedef smpp_bind_t smpp_bind_transceiver_t;

/**
 * \brief Decodes bind_transceiver requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param bind Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transceiver_decode(const uint8_t *body, size_t body_length,
							 smpp_bind_transceiver_t *bind)
{
	return smpp_bind_decode(body, body_length, bind);
}

/**
 * \brief Encodes bind_transceiver requests.
 * \param bind Bind to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transceiver_encode(const smpp_bind_transceiver_t *bind,
							 uint8_t *buffer, size_t buffer_length)
{
	return smpp_bind_encode(bind, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode bind.
 * \param bind The bind.
 * \return Octets needed.
 */
static inline size_t
smpp_bind_transceiver_encoded_length(const smpp_bind_transceiver_t *bind)
{
	return smpp_bind_encoded_length(bind);
}

/**
 * \struct smpp_bind_resp_t
 * \brief bind_transmitter_resp, bind_receiver_resp, & bind_transceiver_resp.
 */
typedef struct smpp_bind_resp_t {
	const char *system_id;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_bind_resp_t;

/**
 * \brief Decodes bind_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_bind_resp_decode(const uint8_t *body, size_t body_length,
					  smpp_bind_resp_t *resp);

/**
 * \brief Encodes bind_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_bind_resp_encode(const smpp_bind_resp_t *resp, uint8_t *buffer,
					  size_t buffer_length);

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
size_t
smpp_bind_resp_encoded_length(const smpp_bind_resp_t *resp);

/**
 * \typedef smpp_bind_transmitter_resp_t
 * \brief bind_transmitter_resp; identical in shape to smpp_bind_resp_t.
 */
typedef smpp_bind_resp_t smpp_bind_transmitter_resp_t;

/**
 * \brief Decodes bind_transmitter_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transmitter_resp_decode(const uint8_t *body, size_t body_length,
								  smpp_bind_transmitter_resp_t *resp)
{
	return smpp_bind_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes bind_transmitter_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transmitter_resp_encode(const smpp_bind_transmitter_resp_t *resp,
								  uint8_t *buffer, size_t buffer_length)
{
	return smpp_bind_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_bind_transmitter_resp_encoded_length(
	const smpp_bind_transmitter_resp_t *resp)
{
	return smpp_bind_resp_encoded_length(resp);
}

/**
 * \typedef smpp_bind_receiver_resp_t
 * \brief bind_receiver_resp; identical in shape to smpp_bind_resp_t.
 */
typedef smpp_bind_resp_t smpp_bind_receiver_resp_t;

/**
 * \brief Decodes bind_receiver_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_receiver_resp_decode(const uint8_t *body, size_t body_length,
							   smpp_bind_receiver_resp_t *resp)
{
	return smpp_bind_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes bind_receiver_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_receiver_resp_encode(const smpp_bind_receiver_resp_t *resp,
							   uint8_t *buffer, size_t buffer_length)
{
	return smpp_bind_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_bind_receiver_resp_encoded_length(const smpp_bind_receiver_resp_t *resp)
{
	return smpp_bind_resp_encoded_length(resp);
}

/**
 * \typedef smpp_bind_transceiver_resp_t
 * \brief bind_transceiver_resp; identical in shape to smpp_bind_resp_t.
 */
typedef smpp_bind_resp_t smpp_bind_transceiver_resp_t;

/**
 * \brief Decodes bind_transceiver_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transceiver_resp_decode(const uint8_t *body, size_t body_length,
								  smpp_bind_transceiver_resp_t *resp)
{
	return smpp_bind_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes bind_transceiver_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_bind_transceiver_resp_encode(const smpp_bind_transceiver_resp_t *resp,
								  uint8_t *buffer, size_t buffer_length)
{
	return smpp_bind_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_bind_transceiver_resp_encoded_length(
	const smpp_bind_transceiver_resp_t *resp)
{
	return smpp_bind_resp_encoded_length(resp);
}

/**
 * \struct smpp_outbind_t
 * \brief outbind. MC-originated invitation for ESME to bind.
 */
typedef struct smpp_outbind_t {
	const char *system_id;
	const char *password;
} smpp_outbind_t;

/**
 * \brief Decodes outbind requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param outbind Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_outbind_decode(const uint8_t *body, size_t body_length,
					smpp_outbind_t *outbind);

/**
 * \brief Encodes outbind requests.
 * \param outbind Outbind to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_outbind_encode(const smpp_outbind_t *outbind, uint8_t *buffer,
					size_t buffer_length);

/**
 * \brief Octets needed to encode outbind.
 * \param outbind The outbind.
 * \return Octets needed.
 */
size_t
smpp_outbind_encoded_length(const smpp_outbind_t *outbind);

/**
 * \struct smpp_sm_t
 * \brief submit_sm & deliver_sm.
 */
typedef struct smpp_sm_t {
	const char *service_type;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	uint8_t dest_addr_ton;
	uint8_t dest_addr_npi;
	const char *destination_addr;
	uint8_t esm_class;
	uint8_t protocol_id;
	uint8_t priority_flag;
	const char *schedule_delivery_time;
	const char *validity_period;
	uint8_t registered_delivery;
	uint8_t replace_if_present_flag;
	uint8_t data_coding;
	uint8_t sm_default_msg_id;
	const uint8_t *short_message;
	uint8_t short_message_length;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_sm_t;

/**
 * \brief Decodes sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param sm Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_sm_decode(const uint8_t *body, size_t body_length, smpp_sm_t *sm);

/**
 * \brief Encodes sm requests.
 * \param sm Message to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_sm_encode(const smpp_sm_t *sm, uint8_t *buffer, size_t buffer_length);

/**
 * \brief Octets needed to encode sm.
 * \param sm The message.
 * \return Octets needed.
 */
size_t
smpp_sm_encoded_length(const smpp_sm_t *sm);

/**
 * \typedef smpp_submit_sm_t
 * \brief submit_sm; identical in shape to smpp_sm_t.
 */
typedef smpp_sm_t smpp_submit_sm_t;

/**
 * \brief Decodes submit_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param sm Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_submit_sm_decode(const uint8_t *body, size_t body_length,
					  smpp_submit_sm_t *sm)
{
	return smpp_sm_decode(body, body_length, sm);
}

/**
 * \brief Encodes submit_sm requests.
 * \param sm Message to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_submit_sm_encode(const smpp_submit_sm_t *sm, uint8_t *buffer,
					  size_t buffer_length)
{
	return smpp_sm_encode(sm, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode sm.
 * \param sm The message.
 * \return Octets needed.
 */
static inline size_t
smpp_submit_sm_encoded_length(const smpp_submit_sm_t *sm)
{
	return smpp_sm_encoded_length(sm);
}

/**
 * \typedef smpp_deliver_sm_t
 * \brief deliver_sm; identical in shape to smpp_sm_t.
 */
typedef smpp_sm_t smpp_deliver_sm_t;

/**
 * \brief Decodes deliver_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param sm Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_deliver_sm_decode(const uint8_t *body, size_t body_length,
					   smpp_deliver_sm_t *sm)
{
	return smpp_sm_decode(body, body_length, sm);
}

/**
 * \brief Encodes deliver_sm requests.
 * \param sm Message to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_deliver_sm_encode(const smpp_deliver_sm_t *sm, uint8_t *buffer,
					   size_t buffer_length)
{
	return smpp_sm_encode(sm, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode sm.
 * \param sm The message.
 * \return Octets needed.
 */
static inline size_t
smpp_deliver_sm_encoded_length(const smpp_deliver_sm_t *sm)
{
	return smpp_sm_encoded_length(sm);
}

/**
 * \struct smpp_sm_resp_t
 * \brief submit_sm_resp & deliver_sm_resp.
 */
typedef struct smpp_sm_resp_t {
	const char *message_id;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_sm_resp_t;

/**
 * \brief Decodes sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_sm_resp_decode(const uint8_t *body, size_t body_length,
					smpp_sm_resp_t *resp);

/**
 * \brief Encodes sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_sm_resp_encode(const smpp_sm_resp_t *resp, uint8_t *buffer,
					size_t buffer_length);

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
size_t
smpp_sm_resp_encoded_length(const smpp_sm_resp_t *resp);

/**
 * \typedef smpp_submit_sm_resp_t
 * \brief submit_sm_resp; identical in shape to smpp_sm_resp_t.
 */
typedef smpp_sm_resp_t smpp_submit_sm_resp_t;

/**
 * \brief Decodes submit_sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_submit_sm_resp_decode(const uint8_t *body, size_t body_length,
						   smpp_submit_sm_resp_t *resp)
{
	return smpp_sm_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes submit_sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_submit_sm_resp_encode(const smpp_submit_sm_resp_t *resp, uint8_t *buffer,
						   size_t buffer_length)
{
	return smpp_sm_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_submit_sm_resp_encoded_length(const smpp_submit_sm_resp_t *resp)
{
	return smpp_sm_resp_encoded_length(resp);
}

/**
 * \typedef smpp_deliver_sm_resp_t
 * \brief deliver_sm_resp; identical in shape to smpp_sm_resp_t.
 */
typedef smpp_sm_resp_t smpp_deliver_sm_resp_t;

/**
 * \brief Decodes deliver_sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_deliver_sm_resp_decode(const uint8_t *body, size_t body_length,
							smpp_deliver_sm_resp_t *resp)
{
	return smpp_sm_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes deliver_sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_deliver_sm_resp_encode(const smpp_deliver_sm_resp_t *resp, uint8_t *buffer,
							size_t buffer_length)
{
	return smpp_sm_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_deliver_sm_resp_encoded_length(const smpp_deliver_sm_resp_t *resp)
{
	return smpp_sm_resp_encoded_length(resp);
}

/**
 * \struct smpp_alert_notification_t
 * \brief alert_notification, MC-originated.
 */
typedef struct smpp_alert_notification_t {
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	uint8_t esme_addr_ton;
	uint8_t esme_addr_npi;
	const char *esme_addr;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_alert_notification_t;

/**
 * \brief Decodes alert_notification requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param alert Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_alert_notification_decode(const uint8_t *body, size_t body_length,
							   smpp_alert_notification_t *alert);

/**
 * \brief Encodes alert_notification requests.
 * \param alert Alert to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_alert_notification_encode(const smpp_alert_notification_t *alert,
							   uint8_t *buffer, size_t buffer_length);

/**
 * \brief Octets needed to encode alert.
 * \param alert The alert.
 * \return Octets needed.
 */
size_t
smpp_alert_notification_encoded_length(const smpp_alert_notification_t *alert);

/**
 * \struct smpp_query_sm_t
 * \brief query_sm.
 */
typedef struct smpp_query_sm_t {
	const char *message_id;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
} smpp_query_sm_t;

/**
 * \brief Decodes query_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param query Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_query_sm_decode(const uint8_t *body, size_t body_length,
					 smpp_query_sm_t *query);

/**
 * \brief Encodes query_sm requests.
 * \param query Query to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_query_sm_encode(const smpp_query_sm_t *query, uint8_t *buffer,
					 size_t buffer_length);

/**
 * \brief Octets needed to encode query.
 * \param query The query.
 * \return Octets needed.
 */
size_t
smpp_query_sm_encoded_length(const smpp_query_sm_t *query);

/**
 * \struct smpp_query_sm_resp_t
 * \brief query_sm_resp.
 */
typedef struct smpp_query_sm_resp_t {
	const char *message_id;
	const char *final_date;
	uint8_t message_state;
	uint8_t error_code;
} smpp_query_sm_resp_t;

/**
 * \brief Decodes query_sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_query_sm_resp_decode(const uint8_t *body, size_t body_length,
						  smpp_query_sm_resp_t *resp);

/**
 * \brief Encodes query_sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_query_sm_resp_encode(const smpp_query_sm_resp_t *resp, uint8_t *buffer,
						  size_t buffer_length);

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
size_t
smpp_query_sm_resp_encoded_length(const smpp_query_sm_resp_t *resp);

/**
 * \struct smpp_cancel_sm_t
 * \brief cancel_sm.
 */
typedef struct smpp_cancel_sm_t {
	const char *service_type;
	const char *message_id;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	uint8_t dest_addr_ton;
	uint8_t dest_addr_npi;
	const char *destination_addr;
} smpp_cancel_sm_t;

/**
 * \brief Decodes cancel_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param cancel Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_cancel_sm_decode(const uint8_t *body, size_t body_length,
					  smpp_cancel_sm_t *cancel);

/**
 * \brief Encodes cancel_sm requests.
 * \param cancel Cancel to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_cancel_sm_encode(const smpp_cancel_sm_t *cancel, uint8_t *buffer,
					  size_t buffer_length);

/**
 * \brief Octets needed to encode cancel.
 * \param cancel The cancel.
 * \return Octets needed.
 */
size_t
smpp_cancel_sm_encoded_length(const smpp_cancel_sm_t *cancel);

/**
 * \struct smpp_replace_sm_t
 * \brief replace_sm.
 */
typedef struct smpp_replace_sm_t {
	const char *message_id;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	const char *schedule_delivery_time;
	const char *validity_period;
	uint8_t registered_delivery;
	uint8_t sm_default_msg_id;
	const uint8_t *short_message;
	uint8_t short_message_length;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_replace_sm_t;

/**
 * \brief Decodes replace_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param replace Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_replace_sm_decode(const uint8_t *body, size_t body_length,
					   smpp_replace_sm_t *replace);

/**
 * \brief Encodes replace_sm requests.
 * \param replace Replace to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_replace_sm_encode(const smpp_replace_sm_t *replace, uint8_t *buffer,
					   size_t buffer_length);

/**
 * \brief Octets needed to encode replace.
 * \param replace The replace.
 * \return Octets needed.
 */
size_t
smpp_replace_sm_encoded_length(const smpp_replace_sm_t *replace);

/**
 * \struct smpp_data_sm_t
 * \brief data_sm.
 */
typedef struct smpp_data_sm_t {
	const char *service_type;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	uint8_t dest_addr_ton;
	uint8_t dest_addr_npi;
	const char *destination_addr;
	uint8_t esm_class;
	uint8_t registered_delivery;
	uint8_t data_coding;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_data_sm_t;

/**
 * \brief Decodes data_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param data Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_data_sm_decode(const uint8_t *body, size_t body_length,
					smpp_data_sm_t *data);

/**
 * \brief Encodes data_sm requests.
 * \param data Data to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_data_sm_encode(const smpp_data_sm_t *data, uint8_t *buffer,
					size_t buffer_length);

/**
 * \brief Octets needed to encode data.
 * \param data The data.
 * \return Octets needed.
 */
size_t
smpp_data_sm_encoded_length(const smpp_data_sm_t *data);

/**
 * \typedef smpp_data_sm_resp_t
 * \brief data_sm_resp; identical in shape to smpp_sm_resp_t.
 */
typedef smpp_sm_resp_t smpp_data_sm_resp_t;

/**
 * \brief Decodes data_sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_data_sm_resp_decode(const uint8_t *body, size_t body_length,
						 smpp_data_sm_resp_t *resp)
{
	return smpp_sm_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes data_sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_data_sm_resp_encode(const smpp_data_sm_resp_t *resp, uint8_t *buffer,
						 size_t buffer_length)
{
	return smpp_sm_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_data_sm_resp_encoded_length(const smpp_data_sm_resp_t *resp)
{
	return smpp_sm_resp_encoded_length(resp);
}

/**
 * \struct smpp_broadcast_sm_t
 * \brief broadcast_sm.
 */
typedef struct smpp_broadcast_sm_t {
	const char *service_type;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	const char *message_id;
	uint8_t priority_flag;
	const char *schedule_delivery_time;
	const char *validity_period;
	uint8_t replace_if_present_flag;
	uint8_t data_coding;
	uint8_t sm_default_msg_id;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_broadcast_sm_t;

/**
 * \brief Decodes broadcast_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param broadcast Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_broadcast_sm_decode(const uint8_t *body, size_t body_length,
						 smpp_broadcast_sm_t *broadcast);

/**
 * \brief Encodes broadcast_sm requests.
 * \param broadcast Broadcast to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_broadcast_sm_encode(const smpp_broadcast_sm_t *broadcast, uint8_t *buffer,
						 size_t buffer_length);

/**
 * \brief Octets needed to encode broadcast.
 * \param broadcast The broadcast.
 * \return Octets needed.
 */
size_t
smpp_broadcast_sm_encoded_length(const smpp_broadcast_sm_t *broadcast);

/**
 * \typedef smpp_broadcast_sm_resp_t
 * \brief broadcast_sm_resp; identical in shape to smpp_sm_resp_t.
 */
typedef smpp_sm_resp_t smpp_broadcast_sm_resp_t;

/**
 * \brief Decodes broadcast_sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_broadcast_sm_resp_decode(const uint8_t *body, size_t body_length,
							  smpp_broadcast_sm_resp_t *resp)
{
	return smpp_sm_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes broadcast_sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_broadcast_sm_resp_encode(const smpp_broadcast_sm_resp_t *resp,
							  uint8_t *buffer, size_t buffer_length)
{
	return smpp_sm_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_broadcast_sm_resp_encoded_length(const smpp_broadcast_sm_resp_t *resp)
{
	return smpp_sm_resp_encoded_length(resp);
}

/**
 * \struct smpp_query_broadcast_sm_t
 * \brief query_broadcast_sm.
 */
typedef struct smpp_query_broadcast_sm_t {
	const char *message_id;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_query_broadcast_sm_t;

/**
 * \brief Decodes query_broadcast_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param query Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_query_broadcast_sm_decode(const uint8_t *body, size_t body_length,
							   smpp_query_broadcast_sm_t *query);

/**
 * \brief Encodes query_broadcast_sm requests.
 * \param query Query to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_query_broadcast_sm_encode(const smpp_query_broadcast_sm_t *query,
							   uint8_t *buffer, size_t buffer_length);

/**
 * \brief Octets needed to encode query.
 * \param query The query.
 * \return Octets needed.
 */
size_t
smpp_query_broadcast_sm_encoded_length(const smpp_query_broadcast_sm_t *query);

/**
 * \typedef smpp_query_broadcast_sm_resp_t
 * \brief query_broadcast_sm_resp; identical in shape to smpp_sm_resp_t.
 */
typedef smpp_sm_resp_t smpp_query_broadcast_sm_resp_t;

/**
 * \brief Decodes query_broadcast_sm_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_query_broadcast_sm_resp_decode(const uint8_t *body, size_t body_length,
									smpp_query_broadcast_sm_resp_t *resp)
{
	return smpp_sm_resp_decode(body, body_length, resp);
}

/**
 * \brief Encodes query_broadcast_sm_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
static inline smpp_status_t
smpp_query_broadcast_sm_resp_encode(const smpp_query_broadcast_sm_resp_t *resp,
									uint8_t *buffer, size_t buffer_length)
{
	return smpp_sm_resp_encode(resp, buffer, buffer_length);
}

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
static inline size_t
smpp_query_broadcast_sm_resp_encoded_length(
	const smpp_query_broadcast_sm_resp_t *resp)
{
	return smpp_sm_resp_encoded_length(resp);
}

/**
 * \struct smpp_cancel_broadcast_sm_t
 * \brief cancel_broadcast_sm.
 */
typedef struct smpp_cancel_broadcast_sm_t {
	const char *service_type;
	const char *message_id;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_cancel_broadcast_sm_t;

/**
 * \brief Decodes cancel_broadcast_sm requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param cancel Out param.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_cancel_broadcast_sm_decode(const uint8_t *body, size_t body_length,
								smpp_cancel_broadcast_sm_t *cancel);

/**
 * \brief Encodes cancel_broadcast_sm requests.
 * \param cancel Cancel to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_cancel_broadcast_sm_encode(const smpp_cancel_broadcast_sm_t *cancel,
								uint8_t *buffer, size_t buffer_length);

/**
 * \brief Octets needed to encode cancel.
 * \param cancel The cancel.
 * \return Octets needed.
 */
size_t
smpp_cancel_broadcast_sm_encoded_length(
	const smpp_cancel_broadcast_sm_t *cancel);

/**
 * \enum smpp_list_status_t
 * \brief Result of a dest_address/unsuccess_sme decode/encode step.
 */
typedef enum smpp_list_status_t {
	SMPP_LIST_OK = 0,
	SMPP_LIST_END,
	SMPP_LIST_MALFORMED,
	SMPP_LIST_ERR_BUFFER_TOO_SMALL,
} smpp_list_status_t;

/**
 * \struct smpp_dest_address_t
 * \brief One decoded dest_address entry from submit_multi.
 */
typedef struct smpp_dest_address_t {
	uint8_t dest_flag;
	uint8_t dest_addr_ton;
	uint8_t dest_addr_npi;
	const char *destination_addr;
	const char *dl_name;
} smpp_dest_address_t;

/**
 * \brief Decodes the next dest_address entry and advances the cursor.
 * \param cursor Cursor into the dest_address block, advanced past the
 *        decoded entry.
 * \param end End of the dest_address block.
 * \param dest Out param.
 * \return SMPP_LIST_OK, SMPP_LIST_END, or SMPP_LIST_MALFORMED.
 */
smpp_list_status_t
smpp_dest_address_next(const uint8_t **cursor, const uint8_t *end,
					   smpp_dest_address_t *dest);

/**
 * \brief Encodes one dest_address entry and advances the offset.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \param offset Current write position; advanced past the written entry.
 * \param dest Entry to encode.
 * \return SMPP_LIST_OK on success, SMPP_LIST_ERR_BUFFER_TOO_SMALL otherwise.
 */
smpp_list_status_t
smpp_dest_address_write(uint8_t *buffer, size_t buffer_length, size_t *offset,
						const smpp_dest_address_t *dest);

/**
 * \struct smpp_unsuccess_sme_t
 * \brief One decoded unsuccess_sme entry from submit_multi_resp.
 */
typedef struct smpp_unsuccess_sme_t {
	uint8_t dest_addr_ton;
	uint8_t dest_addr_npi;
	const char *destination_addr;
	uint32_t error_status_code;
} smpp_unsuccess_sme_t;

/**
 * \brief Decodes the next unsuccess_sme entry and advances the cursor.
 * \param cursor Cursor into the unsuccess_sme block, advanced past the
 *        decoded entry.
 * \param end End of the unsuccess_sme block.
 * \param sme Out param.
 * \return SMPP_LIST_OK, SMPP_LIST_END, or SMPP_LIST_MALFORMED.
 */
smpp_list_status_t
smpp_unsuccess_sme_next(const uint8_t **cursor, const uint8_t *end,
						smpp_unsuccess_sme_t *sme);

/**
 * \brief Encodes one unsuccess_sme entry and advances the offset.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \param offset Current write position; advanced past the written entry.
 * \param sme Entry to encode.
 * \return SMPP_LIST_OK on success, SMPP_LIST_ERR_BUFFER_TOO_SMALL otherwise.
 */
smpp_list_status_t
smpp_unsuccess_sme_write(uint8_t *buffer, size_t buffer_length, size_t *offset,
						 const smpp_unsuccess_sme_t *sme);

/**
 * \struct smpp_submit_multi_t
 * \brief submit_multi. dest_addresses is a pre-validated span covering
 *        exactly dest_count entries; iterate it with smpp_dest_address_next.
 */
typedef struct smpp_submit_multi_t {
	const char *service_type;
	uint8_t source_addr_ton;
	uint8_t source_addr_npi;
	const char *source_addr;
	uint8_t dest_count;
	const uint8_t *dest_addresses;
	uint16_t dest_addresses_length;
	uint8_t esm_class;
	uint8_t protocol_id;
	uint8_t priority_flag;
	const char *schedule_delivery_time;
	const char *validity_period;
	uint8_t registered_delivery;
	uint8_t replace_if_present_flag;
	uint8_t data_coding;
	uint8_t sm_default_msg_id;
	const uint8_t *short_message;
	uint8_t short_message_length;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_submit_multi_t;

/**
 * \brief Decodes submit_multi requests.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param multi Out param.
 * \return SMPP_OK on success, SMPP_ERR_INVALID if dest_count entries can't
 *         all be validated (truncated or malformed dest_address block).
 */
smpp_status_t
smpp_submit_multi_decode(const uint8_t *body, size_t body_length,
						 smpp_submit_multi_t *multi);

/**
 * \brief Encodes submit_multi requests.
 * \param multi Multi to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_submit_multi_encode(const smpp_submit_multi_t *multi, uint8_t *buffer,
						 size_t buffer_length);

/**
 * \brief Octets needed to encode multi.
 * \param multi The multi.
 * \return Octets needed.
 */
size_t
smpp_submit_multi_encoded_length(const smpp_submit_multi_t *multi);

/**
 * \struct smpp_submit_multi_resp_t
 * \brief submit_multi_resp. unsuccess_smes is a pre-validated span covering
 *        exactly unsuccess_count entries; iterate it with
 *        smpp_unsuccess_sme_next.
 */
typedef struct smpp_submit_multi_resp_t {
	const char *message_id;
	uint8_t unsuccess_count;
	const uint8_t *unsuccess_smes;
	uint16_t unsuccess_smes_length;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_submit_multi_resp_t;

/**
 * \brief Decodes submit_multi_resp responses.
 * \param body Body octets.
 * \param body_length Octets available in body.
 * \param resp Out param.
 * \return SMPP_OK on success, SMPP_ERR_INVALID if unsuccess_count entries
 *         can't all be validated (truncated or malformed unsuccess_sme
 *         block).
 */
smpp_status_t
smpp_submit_multi_resp_decode(const uint8_t *body, size_t body_length,
							  smpp_submit_multi_resp_t *resp);

/**
 * \brief Encodes submit_multi_resp responses.
 * \param resp Response to encode.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \return SMPP_OK on success.
 */
smpp_status_t
smpp_submit_multi_resp_encode(const smpp_submit_multi_resp_t *resp,
							  uint8_t *buffer, size_t buffer_length);

/**
 * \brief Octets needed to encode resp.
 * \param resp The response.
 * \return Octets needed.
 */
size_t
smpp_submit_multi_resp_encoded_length(const smpp_submit_multi_resp_t *resp);

/**
 * \brief Returns a string describing status.
 * \param status The status.
 * \return A string literal.
 */
const char *
smpp_strerror(smpp_status_t status);

/**
 * \brief Returns a string describing command_status value.
 * \param command_status The command_status value from a PDU header.
 * \return A string literal.
 */
const char *
smpp_esme_strerror(uint32_t command_status);

#ifdef __cplusplus
}
#endif

#endif
