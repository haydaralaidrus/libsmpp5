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

#define SMPP_ESM_CLASS_DEFAULT (0x00u)
#define SMPP_ESM_CLASS_DATAGRAM (0x01u)
#define SMPP_ESM_CLASS_FORWARD (0x02u)
#define SMPP_ESM_CLASS_STORE_AND_FORWARD (0x03u)
#define SMPP_ESM_CLASS_MC_DELIVERY_RECEIPT (0x04u)
#define SMPP_ESM_CLASS_INTERMEDIATE_NOTIFICATION (0x20u)
#define SMPP_ESM_CLASS_UDHI (0x40u)
#define SMPP_ESM_CLASS_REPLY_PATH (0x80u)

#define SMPP_REGISTERED_DELIVERY_NONE (0x00u)
#define SMPP_REGISTERED_DELIVERY_SUCCESS_OR_FAILURE (0x01u)
#define SMPP_REGISTERED_DELIVERY_FAILURE (0x02u)
#define SMPP_REGISTERED_DELIVERY_SUCCESS (0x03u)
#define SMPP_REGISTERED_DELIVERY_SME_DELIVERY_ACK (0x04u)
#define SMPP_REGISTERED_DELIVERY_SME_USER_ACK (0x08u)
#define SMPP_REGISTERED_DELIVERY_INTERMEDIATE_NOTIFICATION (0x10u)

#define SMPP_REPLACE_IF_PRESENT_NO (0x00u)
#define SMPP_REPLACE_IF_PRESENT_YES (0x01u)

#define SMPP_SYSTEM_ID_MAX (16u)
#define SMPP_PASSWORD_MAX (9u)
#define SMPP_SYSTEM_TYPE_MAX (13u)
#define SMPP_ADDRESS_RANGE_MAX (41u)
#define SMPP_SERVICE_TYPE_MAX (6u)
#define SMPP_ADDR_MAX (21u)
#define SMPP_TIME_MAX (17u)
#define SMPP_MESSAGE_ID_MAX (65u)

typedef enum smpp_status_t {
	SMPP_OK = 0,
	SMPP_ERR_INVALID,
	SMPP_ERR_BUFFER_TOO_SMALL,
} smpp_status_t;

typedef struct smpp_bind_t {
	const char *system_id;
	const char *password;
	const char *system_type;
	uint8_t interface_version;
	uint8_t addr_ton;
	uint8_t addr_npi;
	const char *address_range;
} smpp_bind_t;

smpp_status_t
smpp_bind_decode(const uint8_t *body, size_t body_length, smpp_bind_t *bind);

smpp_status_t
smpp_bind_encode(const smpp_bind_t *bind, uint8_t *buffer,
				 size_t buffer_length);

size_t
smpp_bind_encoded_length(const smpp_bind_t *bind);

typedef struct smpp_bind_resp_t {
	const char *system_id;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_bind_resp_t;

smpp_status_t
smpp_bind_resp_decode(const uint8_t *body, size_t body_length,
					  smpp_bind_resp_t *resp);

smpp_status_t
smpp_bind_resp_encode(const smpp_bind_resp_t *resp, uint8_t *buffer,
					  size_t buffer_length);

size_t
smpp_bind_resp_encoded_length(const smpp_bind_resp_t *resp);

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

smpp_status_t
smpp_sm_decode(const uint8_t *body, size_t body_length, smpp_sm_t *sm);

smpp_status_t
smpp_sm_encode(const smpp_sm_t *sm, uint8_t *buffer, size_t buffer_length);

size_t
smpp_sm_encoded_length(const smpp_sm_t *sm);

typedef struct smpp_sm_resp_t {
	const char *message_id;
	const uint8_t *tlvs;
	uint16_t tlvs_length;
} smpp_sm_resp_t;

smpp_status_t
smpp_sm_resp_decode(const uint8_t *body, size_t body_length,
					smpp_sm_resp_t *resp);

smpp_status_t
smpp_sm_resp_encode(const smpp_sm_resp_t *resp, uint8_t *buffer,
					size_t buffer_length);

size_t
smpp_sm_resp_encoded_length(const smpp_sm_resp_t *resp);

#endif
