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

#ifndef LIBSMPP5_SMPP_TLV_H
#define LIBSMPP5_SMPP_TLV_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** TLV tags */
#define SMPP_TAG_DEST_ADDR_SUBUNIT 0x0005u
#define SMPP_TAG_DEST_NETWORK_TYPE 0x0006u
#define SMPP_TAG_DEST_BEARER_TYPE 0x0007u
#define SMPP_TAG_DEST_TELEMATICS_ID 0x0008u
#define SMPP_TAG_SOURCE_ADDR_SUBUNIT 0x000Du
#define SMPP_TAG_SOURCE_NETWORK_TYPE 0x000Eu
#define SMPP_TAG_SOURCE_BEARER_TYPE 0x000Fu
#define SMPP_TAG_SOURCE_TELEMATICS_ID 0x0010u
#define SMPP_TAG_QOS_TIME_TO_LIVE 0x0017u
#define SMPP_TAG_PAYLOAD_TYPE 0x0019u
#define SMPP_TAG_ADDITIONAL_STATUS_INFO_TEXT 0x001Du
#define SMPP_TAG_RECEIPTED_MESSAGE_ID 0x001Eu
#define SMPP_TAG_MS_MSG_WAIT_FACILITIES 0x0030u
#define SMPP_TAG_PRIVACY_INDICATOR 0x0201u
#define SMPP_TAG_SOURCE_SUBADDRESS 0x0202u
#define SMPP_TAG_DEST_SUBADDRESS 0x0203u
#define SMPP_TAG_USER_MESSAGE_REFERENCE 0x0204u
#define SMPP_TAG_USER_RESPONSE_CODE 0x0205u
#define SMPP_TAG_SOURCE_PORT 0x020Au
#define SMPP_TAG_DEST_PORT 0x020Bu
#define SMPP_TAG_SAR_MSG_REF_NUM 0x020Cu
#define SMPP_TAG_LANGUAGE_INDICATOR 0x020Du
#define SMPP_TAG_SAR_TOTAL_SEGMENTS 0x020Eu
#define SMPP_TAG_SAR_SEGMENT_SEQNUM 0x020Fu
#define SMPP_TAG_SC_INTERFACE_VERSION 0x0210u
#define SMPP_TAG_CALLBACK_NUM_PRES_IND 0x0302u
#define SMPP_TAG_CALLBACK_NUM_ATAG 0x0303u
#define SMPP_TAG_NUMBER_OF_MESSAGES 0x0304u
#define SMPP_TAG_CALLBACK_NUM 0x0381u
#define SMPP_TAG_DPF_RESULT 0x0420u
#define SMPP_TAG_SET_DPF 0x0421u
#define SMPP_TAG_MS_AVAILABILITY_STATUS 0x0422u
#define SMPP_TAG_NETWORK_ERROR_CODE 0x0423u
#define SMPP_TAG_MESSAGE_PAYLOAD 0x0424u
#define SMPP_TAG_DELIVERY_FAILURE_REASON 0x0425u
#define SMPP_TAG_MORE_MESSAGES_TO_SEND 0x0426u
#define SMPP_TAG_MESSAGE_STATE 0x0427u
#define SMPP_TAG_CONGESTION_STATE 0x0428u
#define SMPP_TAG_USSD_SERVICE_OP 0x0501u
#define SMPP_TAG_BROADCAST_CHANNEL_INDICATOR 0x0600u
#define SMPP_TAG_BROADCAST_CONTENT_TYPE 0x0601u
#define SMPP_TAG_BROADCAST_CONTENT_TYPE_INFO 0x0602u
#define SMPP_TAG_BROADCAST_MESSAGE_CLASS 0x0603u
#define SMPP_TAG_BROADCAST_REP_NUM 0x0604u
#define SMPP_TAG_BROADCAST_FREQUENCY_INTERVAL 0x0605u
#define SMPP_TAG_BROADCAST_AREA_IDENTIFIER 0x0606u
#define SMPP_TAG_BROADCAST_ERROR_STATUS 0x0607u
#define SMPP_TAG_BROADCAST_AREA_SUCCESS 0x0608u
#define SMPP_TAG_BROADCAST_END_TIME 0x0609u
#define SMPP_TAG_BROADCAST_SERVICE_GROUP 0x060Au
#define SMPP_TAG_BILLING_IDENTIFICATION 0x060Bu
#define SMPP_TAG_SOURCE_NETWORK_ID 0x060Du
#define SMPP_TAG_DEST_NETWORK_ID 0x060Eu
#define SMPP_TAG_SOURCE_NODE_ID 0x060Fu
#define SMPP_TAG_DEST_NODE_ID 0x0610u
#define SMPP_TAG_DEST_ADDR_NP_RESOLUTION 0x0611u
#define SMPP_TAG_DEST_ADDR_NP_INFORMATION 0x0612u
#define SMPP_TAG_DEST_ADDR_NP_COUNTRY 0x0613u
#define SMPP_TAG_DISPLAY_TIME 0x1201u
#define SMPP_TAG_SMS_SIGNAL 0x1203u
#define SMPP_TAG_MS_VALIDITY 0x1204u
#define SMPP_TAG_ALERT_ON_MESSAGE_DELIVERY 0x130Cu
#define SMPP_TAG_ITS_REPLY_TYPE 0x1380u
#define SMPP_TAG_ITS_SESSION_INFO 0x1383u

/**
 * \struct smpp_tlv_t
 * \brief A decoded tag-length-value entry.
 */
typedef struct smpp_tlv_t {
	uint16_t tag; /** Identifies the TLV; one of the SMPP_TAG_* values. */
	uint16_t length; /** Length of value in octets. */
	const uint8_t *value; /** TLV value, or NULL when length is 0. */
} smpp_tlv_t;

/**
 * \enum smpp_tlv_status_t
 * \brief Result of TLV decode/encode step. SMPP_TLV_END marks an end
 *        of iteration.
 */
typedef enum smpp_tlv_status_t {
	SMPP_TLV_OK = 0,
	SMPP_TLV_END,
	SMPP_TLV_MALFORMED,
	SMPP_TLV_ERR_BUFFER_TOO_SMALL
} smpp_tlv_status_t;

/**
 * \brief Decodes next TLV and advances the cursor.
 * \param cursor Cursor into the TLV stream, advanced past the decoded TLV.
 * \param end End of the TLV stream.
 * \param tlv Out param.
 * \return SMPP_TLV_OK, SMPP_TLV_END, or SMPP_TLV_MALFORMED.
 */
smpp_tlv_status_t
smpp_tlv_next(const uint8_t **cursor, const uint8_t *end, smpp_tlv_t *tlv);

/**
 * \brief Finds a TLV by tag.
 * \param tlvs TLV stream.
 * \param tlvs_length Octets in tlvs.
 * \param tag Tag to find.
 * \param tlv Out param.
 * \return 1 if found, 0 if not found or the stream is malformed.
 */
int
smpp_tlv_find(const uint8_t *tlvs, uint16_t tlvs_length, uint16_t tag,
			  smpp_tlv_t *tlv);

/**
 * \brief Encodes one TLV and advances the offset.
 * \param buffer Destination.
 * \param buffer_length Destination capacity.
 * \param offset Current write position; advanced past the written TLV.
 * \param tag Tag to write.
 * \param value Value octets.
 * \param value_length Octets in value.
 * \return SMPP_TLV_OK on success.
 */
smpp_tlv_status_t
smpp_tlv_write(uint8_t *buffer, size_t buffer_length, size_t *offset,
			   uint16_t tag, const uint8_t *value, uint16_t value_length);

/**
 * \brief Returns a string describing status.
 * \param status The status.
 * \return A string literal.
 */
const char *
smpp_tlv_strerror(smpp_tlv_status_t status);

#ifdef __cplusplus
}
#endif

#endif
