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

const char *
smpp_strerror(smpp_status_t status)
{
	/* No default */
	switch (status) {
	case SMPP_OK:
		return "success";
	case SMPP_ERR_INVALID:
		return "invalid field value";
	case SMPP_ERR_BUFFER_TOO_SMALL:
		return "destination buffer too small";
	}

	return "unknown smpp_status_t";
}

const char *
smpp_esme_strerror(uint32_t command_status)
{
	switch (command_status) {
	case SMPP_ESME_ROK:
		return "success";
	case SMPP_ESME_RINVMSGLEN:
		return "message length is invalid";
	case SMPP_ESME_RINVCMDLEN:
		return "command length is invalid";
	case SMPP_ESME_RINVCMDID:
		return "invalid command ID";
	case SMPP_ESME_RINVBNDSTS:
		return "incorrect bind status for the given command";
	case SMPP_ESME_RALYBND:
		return "ESME already in bound state";
	case SMPP_ESME_RINVPRTFLG:
		return "invalid priority flag";
	case SMPP_ESME_RINVREGDLVFLG:
		return "invalid registered delivery flag";
	case SMPP_ESME_RSYSERR:
		return "system error";
	case SMPP_ESME_RINVSRCADR:
		return "invalid source address";
	case SMPP_ESME_RINVDSTADR:
		return "invalid destination address";
	case SMPP_ESME_RINVMSGID:
		return "invalid message ID";
	case SMPP_ESME_RBINDFAIL:
		return "bind failed";
	case SMPP_ESME_RINVPASWD:
		return "invalid password";
	case SMPP_ESME_RINVSYSID:
		return "invalid system ID";
	case SMPP_ESME_RCANCELFAIL:
		return "cancel_sm failed";
	case SMPP_ESME_RREPLACEFAIL:
		return "replace_sm failed";
	case SMPP_ESME_RMSGQFUL:
		return "message queue full";
	case SMPP_ESME_RINVSERTYP:
		return "invalid service type";
	case SMPP_ESME_RINVNUMDESTS:
		return "invalid number of destinations";
	case SMPP_ESME_RINVDLNAME:
		return "invalid distribution list name";
	case SMPP_ESME_RINVDESTFLAG:
		return "invalid destination flag in submit_multi";
	case SMPP_ESME_RINVSUBREP:
		return "submit-with-replace not supported or inappropriate here";
	case SMPP_ESME_RINVESMCLASS:
		return "invalid esm_class field data";
	case SMPP_ESME_RCNTSUBDL:
		return "cannot submit to distribution list";
	case SMPP_ESME_RSUBMITFAIL:
		return "submit_sm, data_sm, or submit_multi failed";
	case SMPP_ESME_RINVSRCTON:
		return "invalid source address TON";
	case SMPP_ESME_RINVSRCNPI:
		return "invalid source address NPI";
	case SMPP_ESME_RINVDSTTON:
		return "invalid destination address TON";
	case SMPP_ESME_RINVDSTNPI:
		return "invalid destination address NPI";
	case SMPP_ESME_RINVSYSTYP:
		return "invalid system_type field";
	case SMPP_ESME_RINVREPFLAG:
		return "invalid replace_if_present flag";
	case SMPP_ESME_RINVNUMMSGS:
		return "invalid number of messages";
	case SMPP_ESME_RTHROTTLED:
		return "throttled: message rate limit exceeded";
	case SMPP_ESME_RINVSCHED:
		return "invalid scheduled delivery time";
	case SMPP_ESME_RINVEXPIRY:
		return "invalid validity period (expiry time)";
	case SMPP_ESME_RINVDFTMSGID:
		return "predefined message ID is invalid or not found";
	case SMPP_ESME_RX_T_APPN:
		return "ESME receiver temporary application error";
	case SMPP_ESME_RX_P_APPN:
		return "ESME receiver permanent application error";
	case SMPP_ESME_RX_R_APPN:
		return "ESME receiver reject-message error";
	case SMPP_ESME_RQUERYFAIL:
		return "query_sm failed";
	case SMPP_ESME_RINVTLVSTREAM:
		return "malformed TLV stream in the PDU body";
	case SMPP_ESME_RTLVNOTALLWD:
		return "TLV not allowed in this context";
	case SMPP_ESME_RINVTLVLEN:
		return "invalid TLV length";
	case SMPP_ESME_RMISSINGTLV:
		return "expected TLV missing";
	case SMPP_ESME_RINVTLVVAL:
		return "invalid TLV value";
	case SMPP_ESME_RDELIVERYFAILURE:
		return "transaction delivery failure";
	case SMPP_ESME_RUNKNOWNERR:
		return "unknown error";
	case SMPP_ESME_RSERTYPUNAUTH:
		return "ESME not authorised for this service_type";
	case SMPP_ESME_RPROHIBITED:
		return "ESME prohibited from this operation";
	case SMPP_ESME_RSERTYPUNAVAIL:
		return "service_type is unavailable";
	case SMPP_ESME_RSERTYPDENIED:
		return "service_type is denied";
	case SMPP_ESME_RINVDCS:
		return "invalid data coding scheme";
	case SMPP_ESME_RINVSRCADDRSUBUNIT:
		return "invalid source address sub-unit";
	case SMPP_ESME_RINVDSTADDRSUBUNIT:
		return "invalid destination address sub-unit";
	case SMPP_ESME_RINVBCASTFREQINT:
		return "invalid broadcast frequency interval";
	case SMPP_ESME_RINVBCASTALIAS_NAME:
		return "invalid broadcast alias name";
	case SMPP_ESME_RINVBCASTAREAFMT:
		return "invalid broadcast area format";
	case SMPP_ESME_RINVNUMBCAST_AREAS:
		return "invalid number of broadcast areas";
	case SMPP_ESME_RINVBCASTCNTTYPE:
		return "invalid broadcast content type";
	case SMPP_ESME_RINVBCASTMSGCLASS:
		return "invalid broadcast message class";
	case SMPP_ESME_RBCASTFAIL:
		return "broadcast_sm failed";
	case SMPP_ESME_RBCASTQUERYFAIL:
		return "query_broadcast_sm failed";
	case SMPP_ESME_RBCASTCANCELFAIL:
		return "cancel_broadcast_sm failed";
	case SMPP_ESME_RINVBCAST_REP:
		return "invalid number of repeated broadcasts";
	case SMPP_ESME_RINVBCASTSRVGRP:
		return "invalid broadcast service group";
	case SMPP_ESME_RINVBCASTCHANIND:
		return "invalid broadcast channel indicator";
	default:
		return "unrecognised or vendor-specific SMPP error code";
	}
}
