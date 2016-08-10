//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------
#ifndef ENUMS_H
#define ENUMS_H

#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <map>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <math.h>
#include <cmath>

#if _MSC_VER > 12
#define _CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

const unsigned char HDLC_FRAME_START_END = 0x7E;
const char AARQ_TAG = 0x60;
const char AARE_TAG = 0x61;

const unsigned char LLC_SEND_BYTES[3] = {0xE6, 0xE6, 0x00};
const unsigned char LLC_REPLY_BYTES[3] = {0xE6, 0xE7, 0x00};


enum DLMS_ASSOCIATION_RESULT
{
    DLMS_ASSOCIATION_RESULT_ACCEPTED = 0,
    DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED = 1,
    DLMS_ASSOCIATION_RESULT_TRANSIENT_REJECTED = 2
};

typedef enum
{
    DLMS_SOURCE_DIAGNOSTIC_NONE = 0,
    DLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN = 1,
    //Application context name not supported.
    DLMS_SOURCE_DIAGNOSTIC_NOT_SUPPORTED = 2,
    DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED = 11,
    DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED = 12,
    DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE =13,
    DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED =14,
} DLMS_SOURCE_DIAGNOSTIC;

/**
 *
 * Defines the baud rates.
 */
enum DLMS_BAUD_RATE
{
    /**
    Baudrate is 300.
    */
    DLMS_BAUD_RATE_300 = 0,
    /**
     Baudrate is 600.
    */
    DLMS_BAUD_RATE_600,
    /**
     Baudrate is 1200.
    */
    DLMS_BAUD_RATE_1200,
    /**
     Baudrate is 2400.
    */
    DLMS_BAUD_RATE_2400,
    /**
     Baudrate is 4800.
    */
    DLMS_BAUD_RATE_4800,
    /**
     Baudrate is 9600.
    */
    DLMS_BAUD_RATE_9600,
    /**
     Baudrate is 19200.
    */
    DLMS_BAUD_RATE_19200,
    /**
     Baudrate is 38400.
    */
    DLMS_BAUD_RATE_38400,
    /**
     Baudrate is 57600.
    */
    DLMS_BAUD_RATE_57600,
    /**
     Baudrate is 115200.
    */
    DLMS_BAUD_RATE_115200
};


/**
 Defines the protocol used by the meter on the port.
*/
enum DLMS_OPTICAL_PROTOCOL_MODE
{
    /**
     Protocol according to IEC 62056-21 (modes A…E),
    */
    DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT = 0,
    /**
     Protocol according to IEC 62056-46.
     Using this enumeration value all other attributes of this IC are not applicable.
    */
    DLMS_OPTICAL_PROTOCOL_MODE_NET = 1,
    /**
     Protocol not specified. Using this enumeration value,
     ProposedBaudrate is used for setting the communication speed on the port.
     All other attributes are not applicable.
    */
    DLMS_OPTICAL_PROTOCOL_MODE_UNKNOWN = 2
};

enum DLMS_LOCAL_PORT_RESPONSE_TIME
{
    /**
     Minimium time is 20 ms.
    */
    DLMS_LOCAL_PORT_RESPONSE_TIME_20_MS = 0,
    /**
     Minimium time is 200 ms.
    */
    DLMS_LOCAL_PORT_RESPONSE_TIME_200_MS = 1
};


/*
 * Configures the behaviour of the disconnect control object for all
triggers, i.e. the possible state transitions.
 */
enum DLMS_CONTROL_MODE
{
    /*
     * The disconnect control object is always in 'connected' state,
     */
    DLMS_CONTROL_MODE_NONE,
    /*
     * Disconnection: Remote (b, c), manual (f), local (g)
     * Reconnection: Remote (d), manual (e).
     */
    DLMS_CONTROL_MODE_MODE_1,
    /*
     * Disconnection: Remote (b, c), manual (f), local (g)
     * Reconnection: Remote (a), manual (e).
     */
    DLMS_CONTROL_MODE_MODE_2,
    /*
     * Disconnection: Remote (b, c), manual (-), local (g)
     * Reconnection: Remote (d), manual (e).
     */
    DLMS_CONTROL_MODE_MODE_3,
    /*
     * Disconnection: Remote (b, c), manual (-), local (g)
     * Reconnection: Remote (a), manual (e)
     */
    DLMS_CONTROL_MODE_MODE_4,
    /*
     * Disconnection: Remote (b, c), manual (f), local (g)
     * Reconnection: Remote (d), manual (e), local (h),
     */
    DLMS_CONTROL_MODE_MODE_5,
    /*
     * Disconnection: Remote (b, c), manual (-), local (g)
     * Reconnection: Remote (d), manual (e), local (h)
     */
    DLMS_CONTROL_MODE_MODE_6,
};

//Is logican name assosiated.
enum DLMS_DLMS_ASSOCIATION_STATUS
{
    DLMS_DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED,
    DLMS_DLMS_ASSOCIATION_STATUS_ASSOCIATION_PENDING,
    DLMS_DLMS_ASSOCIATION_STATUS_ASSOCIATED
};

typedef enum
{
    /// <summary>
    /// Not defined
    /// </summary>
    DLMS_CLOCK_BASE_NONE,
    /// <summary>
    /// Internal Crystal
    /// </summary>
    DLMS_CLOCK_BASE_CRYSTAL,
    /// <summary>
    /// Mains frequency 50 Hz,
    /// </summary>
    DLMS_CLOCK_BASE_FREQUENCY_50,
    /// <summary>
    /// Mains frequency 60 Hz,
    /// </summary>
    DLMS_CLOCK_BASE_FREQUENCY_60,
    /// <summary>
    /// Global Positioning System.
    /// </summary>
    DLMS_CLOCK_BASE_GPS,
    /// <summary>
    /// Radio controlled.
    /// </summary>
    DLMS_CLOCK_BASE_RADIO
} DLMS_CLOCK_BASE;

typedef enum
{
    /*
     * The output_state is set to false and the consumer is disconnected.
     */
    DLMS_CONTROL_STATE_DISCONNECTED,
    /*
     * The output_state is set to true and the consumer is connected.
     */
    DLMS_CONTROL_STATE_CONNECTED,
    /*
     * The output_state is set to false and the consumer is disconnected.
     */
    DLMS_CONTROL_STATE_READY_FOR_RECONNECTION
} DLMS_CONTROL_STATE;


// Defines whether or not the device has been assigned an address
// since last power up of the device.
typedef enum
{
    // Not assigned an address yet.
    DLMS_ADDRESS_STATE_NONE,
    // Assigned an address either by manual setting, or by automated method.
    DLMS_ADDRESS_STATE_ASSIGNED
} DLMS_ADDRESS_STATE;


typedef enum
{
    /**
     Size of execution_time = 1. Wildcard in date allowed.
    */
    DLMS_SINGLE_ACTION_SCHEDULE_TYPE1 = 0,
    /**
     Size of execution_time = n.
     All time values are the same, wildcards in date not allowed.
    */
    DLMS_SINGLE_ACTION_SCHEDULE_TYPE2 = 1,
    /**
     Size of execution_time = n.
     All time values are the same, wildcards in date are allowed,
    */
    DLMS_SINGLE_ACTION_SCHEDULE_TYPE3 = 2,
    /**
     Size of execution_time = n.
     Time values may be different, wildcards in date not allowed,
    */
    DLMS_SINGLE_ACTION_SCHEDULE_TYPE4 = 3,
    /**
     Size of execution_time = n.
     Time values may be different, wildcards in date are allowed
    */
    DLMS_SINGLE_ACTION_SCHEDULE_TYPE5 = 4
} DLMS_SINGLE_ACTION_SCHEDULE_TYPE;

typedef enum
{
    DLMS_IMAGE_TRANSFER_STATUS_NOT_INITIATED,
    DLMS_IMAGE_TRANSFER_STATUS_INITIATED,
    DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_INITIATED,
    DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_SUCCESSFUL,
    DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_FAILED,
    DLMS_IMAGE_TRANSFER_STATUS_ACTIVATION_INITIATED,
    DLMS_IMAGE_TRANSFER_STATUS_ACTIVATION_SUCCESSFUL,
    DLMS_IMAGE_TRANSFER_STATUS_ACTIVATION_FAILED
} DLMS_IMAGE_TRANSFER_STATUS;

typedef enum
{
    /**
     * Transport security is not used.
     */
    DLMS_SECURITY_NONE = 0,

    /**
     * Authentication security is used.
     */
    DLMS_SECURITY_AUTHENTICATION = 0x10,

    /**
     * Encryption security is used.
     */
    DLMS_SECURITY_ENCRYPTION = 0x20,

    /**
     * Authentication and Encryption security are used.
     */
    DLMS_SECURITY_AUTHENTICATION_ENCRYPTION = 0x30,
} DLMS_SECURITY;

typedef enum
{
    DLMS_COUNT_TYPE_TAG = 0x1,
    DLMS_COUNT_TYPE_DATA = 2,
    DLMS_COUNT_TYPE_PACKET = 3
} DLMS_COUNT_TYPE;

/**
* HDLC frame types.
*/
typedef enum
{
    /**
    * I-Frame. Information frames are used to transport user data.
    */
    HDLC_FRAME_TYPE_I_FRAME = 0x0,
    /**
     * S-frame. Supervisory Frames are used for flow and error control.
     * Rejected, RNR and RR.
     */
    HDLC_FRAME_TYPE_S_FRAME = 0x1,
    /**
     * U-frame. Unnumbered frames are used for link management. Example SNRM and
     * UA.
     */
    HDLC_FRAME_TYPE_U_FRAME = 0x3
} HDLC_FRAME_TYPE;

/**
*BER encoding enumeration values.
*/
typedef enum
{
    /**
    * End of Content.
    */
    BER_TYPE_EOC = 0x00,
    /**
    * Boolean.
    */
    BER_TYPE_BOOLEAN = 0x1,
    /**
    * Integer.
    */
    BER_TYPE_INTEGER = 0x2,
    /**
    * Bit String.
    */
    BER_TYPE_BIT_STRING = 0x3,
    /**
    * Octet string.
    */
    BER_TYPE_OCTET_STRING = 0x4,
    /**
    * Null value.
    */
    BER_TYPE_NULL = 0x5,
    /**
    * Object identifier.
    */
    BER_TYPE_OBJECT_IDENTIFIER = 0x6,
    /**
    * Object Descriptor.
    */
    BER_TYPE_OBJECT_DESCRIPTOR = 7,
    /**
    * External
    */
    BER_TYPE_EXTERNAL = 8,
    /**
    * Real (float).
    */
    BER_TYPE_REAL = 9,
    /**
    * Enumerated.
    */
    BER_TYPE_ENUMERATED = 10,
    /**
    * Utf8 String.
    */
    BER_TYPE_UTF8_STRING = 12,
    /**
    * Numeric string.
    */
    BER_TYPE_NUMERIC_STRING = 18,
    /**
    * Printable string.
    */
    BER_TYPE_PRINTABLE_STRING = 19,
    /**
    * Teletex string.
    */
    BER_TYPE_TELETEX_STRING = 20,
    /**
    * Videotex string.
    */
    BER_TYPE_VIDEOTEX_STRING = 21,
    /**
    * Ia5 string
    */
    BER_TYPE_IA5_STRING = 22,
    /**
    * Utc time.
    */
    BER_TYPE_UTC_TIME = 23,
    /**
    * Generalized time.
    */
    BER_TYPE_GENERALIZED_TIME = 24,
    /**
    * Graphic string.
    */
    BER_TYPE_GRAPHIC_STRING = 25,
    /**
    * Visible string.
    */
    BER_TYPE_VISIBLE_STRING = 26,
    /**
    * General string.
    */
    BER_TYPE_GENERAL_STRING = 27,
    /**
    * Universal string.
    */
    BER_TYPE_UNIVERSAL_STRING = 28,
    /**
    * Bmp string.
    */
    BER_TYPE_BMP_STRING = 30,
    /**
    * Application class.
    */
    BER_TYPE_APPLICATION = 0x40,
    /**
    * Context class.
    */
    BER_TYPE_CONTEXT = 0x80,
    /**
    * Private class.
    */
    BER_TYPE_PRIVATE = 0xc0,
    /**
    * Constructed.
    */
    BER_TYPE_CONSTRUCTED = 0x20
} BER_TYPE;

/**
 * APDU types.
 */
typedef enum
{
    /**
     * IMPLICIT BIT STRING {version1  = 0} DEFAULT {version1}
     */
    PDU_TYPE_PROTOCOL_VERSION = 0,

    /**
     * Application-context-name
     */
    PDU_TYPE_APPLICATION_CONTEXT_NAME = 1,

    /**
     * AP-title OPTIONAL
     */
    PDU_TYPE_CALLED_AP_TITLE = 2,

    /**
     * AE-qualifier OPTIONAL.
     */
    PDU_TYPE_CALLED_AE_QUALIFIER = 3,

    /**
     * AP-invocation-identifier OPTIONAL.
     */
    PDU_TYPE_CALLED_AP_INVOCATION_ID = 4,

    /**
     * AE-invocation-identifier OPTIONAL
     */
    PDU_TYPE_CALLED_AE_INVOCATION_ID = 5,

    /**
     * AP-title OPTIONAL
     */
    PDU_TYPE_CALLING_AP_TITLE = 6,

    /**
     * AE-qualifier OPTIONAL
     */
    PDU_TYPE_CALLING_AE_QUALIFIER = 7,

    /**
     * AP-invocation-identifier OPTIONAL
     */
    PDU_TYPE_CALLING_AP_INVOCATION_ID = 8,

    /**
     * AE-invocation-identifier OPTIONAL
     */
    PDU_TYPE_CALLING_AE_INVOCATION_ID = 9,

    /**
     * The following field shall not be present if only the kernel is used.
     */
    PDU_TYPE_SENDER_ACSE_REQUIREMENTS = 10,

    /**
     * The following field shall only be present if the authentication
     * functional unit is selected.
     */
    PDU_TYPE_MECHANISM_NAME = 11,

    /**
     * The following field shall only be present if the authentication
     * functional unit is selected.
     */
    PDU_TYPE_CALLING_AUTHENTICATION_VALUE = 12,

    /**
     * Implementation-data.
     */
    PDU_TYPE_IMPLEMENTATION_INFORMATION = 29,

    /**
     * Association-information OPTIONAL
     */
    PDU_TYPE_USER_INFORMATION = 30
} PDU_TYPE;

/**
 * HDLC control frame types.
 */
typedef enum
{
    /**
     * Receive is ready.
     */
    HDLC_CONTROL_FRAME_RECEIVE_READY = 0,
    /**
     * Receive is not ready.
     */
    HDLC_CONTROL_FRAME_RECEIVE_NOT_READY = 0x1,
    /**
     * Frame is rejected.
     */
    HDLC_CONTROL_FRAME_REJECT = 2,
    /**
     * Frame is selective rejected. Not all meters support this.
     */
    HDLC_CONTROL_FRAME_SELECTIVE_REJECT = 3
} HDLC_CONTROL_FRAME;

// Security policy Enforces authentication and/or encryption algorithm provided with security_suite.
//Note! This enumeration values are used in Security Setup version 0.
typedef enum
{
    //No security is used.
    DLMS_SECURITY_POLICY_NOTHING = 0,
    /**
     All messages to be authenticated.
    */
    DLMS_SECURITY_POLICY_AUTHENTICATED = 1,
    /**
     All messages to be encrypted.
    */
    DLMS_SECURITY_POLICY_ENCRYPTED = 2,
    /**
     All messages to be authenticated and encrypted.
    */
    DLMS_SECURITY_POLICY_AUTHENTICATED_ENCRYPTED = 3
} DLMS_SECURITY_POLICY;

// Security policy Enforces authentication and/or encryption algorithm provided with security_suite.
//Note! This enumeration values are used in Security Setup version 1.
typedef enum
{
    /**
     * Security is not used.
     */
    DLMS_SECURITY_POLICY1_NOTHING = 0,
    /**
     * Request is authenticated.
     */
    DLMS_SECURITY_POLICY1_AUTHENTICATED_REQUEST = 0x20,

    /**
     * Request is encrypted.
     */
    DLMS_SECURITY_POLICY1_ENCRYPTED_REQUEST = 0x10,

    /**
     * Request is digitally signed.
     */
    DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_REQUEST = 0x8,

    /**
     * Response is authenticated.
     */
    DLMS_SECURITY_POLICY1_AUTHENTICATED_RESPONSE = 0x4,

    /**
     * Response is encrypted.
     */
    DLMS_SECURITY_POLICY1_ENCRYPTED_RESPONSE = 0x2,

    /**
     * Response is digitally signed.
     */
    DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_RESPONSE = 0x1
} DLMS_SECURITY_POLICY1;


//Security suite Specifies authentication, encryption and key wrapping algorithm.
typedef enum
{
    /**
     AES-GCM-128 for authenticated encryption and AES-128 for key wrapping.
    */
    DLMS_SECURITY_SUITE_AES_GCM_128
} DLMS_SECURITY_SUITE;


typedef enum
{
    DLMS_SERVICE_TYPE_TCP = 0,
    DLMS_SERVICE_TYPE_UDP = 1,
    DLMS_SERVICE_TYPE_FTP = 2,
    DLMS_SERVICE_TYPE_SMTP = 3,
    DLMS_SERVICE_TYPE_SMS = 4
} DLMS_SERVICE_TYPE;

typedef enum
{
    DLMS_MESSAGE_TYPE_COSEM_APDU = 0,
    DLMS_MESSAGE_TYPE_COSEM_APDU_XML = 1,
    DLMS_MESSAGE_TYPE_MANUFACTURER_SPESIFIC = 128
} DLMS_MESSAGE_TYPE;

typedef enum
{
    DLMS_CLOCK_STATUS_OK = 0x0,
    DLMS_CLOCK_STATUS_INVALID_VALUE = 0x1,
    DLMS_CLOCK_STATUS_DOUBTFUL_VALUE = 0x2,
    DLMS_CLOCK_STATUS_DIFFERENT_CLOCK_BASE = 0x4,
    DLMS_CLOCK_STATUS_RESERVED1 = 0x8,
    DLMS_CLOCK_STATUS_RESERVED2 = 0x10,
    DLMS_CLOCK_STATUS_RESERVED3 = 0x20,
    DLMS_CLOCK_STATUS_RESERVED4 = 0x40,
    DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE = 0x80,
    //Skip clock status on write.
    DLMS_CLOCK_STATUS_SKIP = 0xFF
} DLMS_CLOCK_STATUS;

/**
 * Authentication enumerates the authentication levels.
 */
typedef enum
{
    /**
     * No authentication is used.
     */
    DLMS_AUTHENTICATION_NONE,

    /**
     * Low authentication is used.
     */
    DLMS_AUTHENTICATION_LOW,

    /**
     * High authentication is used. Because DLMS/COSEM specification does not
     * specify details of the HLS mechanism we have implemented Indian standard.
     * Texas Instruments also uses this.
     */
    DLMS_AUTHENTICATION_HIGH,

    /**
     * High authentication is used. Password is hashed with MD5.
     */
    DLMS_AUTHENTICATION_HIGH_MD5,

    /**
     * High authentication is used. Password is hashed with SHA1.
     */
    DLMS_AUTHENTICATION_HIGH_SHA1,

    /**
     * High authentication is used. Password is hashed with GMAC.
     */
    DLMS_AUTHENTICATION_HIGH_GMAC
} DLMS_AUTHENTICATION;

/**
 * Used priority.
 */
typedef enum
{
    // Normal priority.
    DLMS_PRIORITY_NORMAL = 0,

    // High priority.
    DLMS_PRIORITY_HIGH = 1
} DLMS_PRIORITY;

typedef enum
{
    DLMS_SERVICE_CLASS_UN_CONFIRMED = 0,
    DLMS_SERVICE_CLASS_CONFIRMED = 1
} DLMS_SERVICE_CLASS;

//Interface type that is used.
typedef enum
{
    //General
    DLMS_INTERFACE_TYPE_HDLC = 0,
    // IEC 62056-47 COSEM transport layers for IPv4 networks
    DLMS_INTERFACE_TYPE_WRAPPER = 0x1
} DLMS_INTERFACE_TYPE;

typedef enum
{
    DLMS_DATA_REQUEST_TYPES_NONE = 0x0,
    DLMS_DATA_REQUEST_TYPES_FRAME = 0x1,
    DLMS_DATA_REQUEST_TYPES_BLOCK = 0x2
} DLMS_DATA_REQUEST_TYPES;

typedef enum
{
    DLMS_OBJECT_TYPE_ALL = -1,
    DLMS_OBJECT_TYPE_NONE = 0,
    DLMS_OBJECT_TYPE_DATA = 1,
    DLMS_OBJECT_TYPE_REGISTER = 3,
    DLMS_OBJECT_TYPE_EXTENDED_REGISTER = 4,
    DLMS_OBJECT_TYPE_DEMAND_REGISTER = 5,
    DLMS_OBJECT_TYPE_REGISTER_ACTIVATION = 6,
    DLMS_OBJECT_TYPE_PROFILE_GENERIC = 7,
    DLMS_OBJECT_TYPE_CLOCK = 8,
    DLMS_OBJECT_TYPE_SCRIPT_TABLE = 9,
    DLMS_OBJECT_TYPE_SCHEDULE = 10,
    DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE = 11,
    DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME = 12,
    DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME = 15,
    DLMS_OBJECT_TYPE_SAP_ASSIGNMENT = 17,
    DLMS_OBJECT_TYPE_IMAGE_TRANSFER = 18,
    DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP = 19,
    DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR = 20,
    DLMS_OBJECT_TYPE_REGISTER_MONITOR = 21,
    DLMS_OBJECT_TYPE_ACTION_SCHEDULE = 22,
    DLMS_OBJECT_TYPE_IEC_HDLC_SETUP = 23,
    DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP = 24,
    DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP = 25,
    DLMS_OBJECT_TYPE_UTILITY_TABLES = 26,
    DLMS_OBJECT_TYPE_MODEM_CONFIGURATION = 27,
    DLMS_OBJECT_TYPE_AUTO_ANSWER = 28,
    DLMS_OBJECT_TYPE_AUTO_CONNECT = 29,
    DLMS_OBJECT_TYPE_TCP_UDP_SETUP = 41,
    DLMS_OBJECT_TYPE_IP4_SETUP = 42,
    DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP = 43,
    DLMS_OBJECT_TYPE_PPP_SETUP = 44,
    DLMS_OBJECT_TYPE_GPRS_SETUP = 45,
    DLMS_OBJECT_TYPE_SMTP_SETUP = 46,
    DLMS_OBJECT_TYPE_REGISTER_TABLE = 61,
    DLMS_OBJECT_TYPE_STATUS_MAPPING = 63,
    DLMS_OBJECT_TYPE_DLMS_SECURITY_SETUP = 64,
    DLMS_OBJECT_TYPE_DISCONNECT_CONTROL = 70,
    DLMS_OBJECT_TYPE_LIMITER = 71,
    DLMS_OBJECT_TYPE_MBUS_CLIENT = 72,
    DLMS_OBJECT_TYPE_PUSH_SETUP = 40,
    DLMS_OBJECT_TYPE_MESSAGE_HANDLER = 60,
    DLMS_OBJECT_TYPE_PARAMETER_MONITOR = 65,
    DLMS_OBJECT_TYPE_WIRELESS_MODE_Q_CHANNEL = 73,
    DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP = 74,

    /**
     * Configure a ZigBee PRO device with information necessary to create or
     * join the network.
     */
    DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP = 101,

    /**
     * Configure the behavior of a ZigBee PRO device on joining or loss of
     * connection to the network.
     */
    DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN = 102,
    /**
     * Configure the fragmentation feature of ZigBee PRO transport layer.
     */
    DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION = 103,
    DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL = 104
} DLMS_OBJECT_TYPE;

typedef enum
{
    DLMS_COMMAND_NONE,
    DLMS_COMMAND_READ_REQUEST = 0x5,
    DLMS_COMMAND_READ_RESPONSE = 0xC,
    DLMS_COMMAND_WRITE_REQUEST = 0x6,
    DLMS_COMMAND_WRITE_RESPONSE = 0xD,
    DLMS_COMMAND_GET_REQUEST = 0xC0,
    DLMS_COMMAND_GET_RESPONSE = 0xC4,
    DLMS_COMMAND_SET_REQUEST = 0xC1,
    DLMS_COMMAND_SET_RESPONSE = 0xC5,
    DLMS_COMMAND_METHOD_REQUEST = 0xC3,
    DLMS_COMMAND_METHOD_RESPONSE = 0xC7,
    DLMS_COMMAND_REJECTED = 0x97,
    // SNRM request.
    DLMS_COMMAND_SNRM = 0x93,
    // UA request.
    DLMS_COMMAND_UA = 0x73,
    DLMS_COMMAND_AARQ = 0x60,
    DLMS_COMMAND_AARE = 0x61,
    /**
     * Disconnect request for HDLC framing.
     */
    DLMS_COMMAND_DISC = 0x53,
    DLMS_COMMAND_DISCONNECT_REQUEST = 0x62,
    DLMS_COMMAND_DISCONNECT_RESPONSE = 0x63,
    // Exception response.
    DLMS_COMMAND_EXCEPTION_RESPONSE = 0xD8,

    // General block transfer.
    DLMS_COMMAND_GENERAL_BLOCK_TRANSFER = 0xE0,
    // Data Notification request.
    DLMS_COMMAND_DATA_NOTIFICATION = 0x0F,

    //Glo messages
    DLMS_COMMAND_GLO_GET_REQUEST = 0xC8,
    DLMS_COMMAND_GLO_GET_RESPONSE = 0xCC,
    DLMS_COMMAND_GLO_SET_REQUEST = 0xC9,
    DLMS_COMMAND_GLO_SET_RESPONSE = 0xCD,
    /**
     * Glo general ciphering.
     */
    DLMS_COMMAND_GLO_GENERAL_CIPHERING = 0xDB,

    /**
    * Glo event notification request.
    */
    DLMS_COMMAND_GLO_EVENT_NOTIFICATION_REQUEST = 0xCA,

    DLMS_COMMAND_GLO_METHOD_REQUEST = 0xCB,
    DLMS_COMMAND_GLO_METHOD_RESPONSE = 0xCF
} DLMS_COMMAND;

typedef enum
{
    HDLC_INFO_MAX_INFO_TX = 0x5,
    HDLC_INFO_MAX_INFO_RX = 0x6,
    HDLC_INFO_WINDOW_SIZE_TX = 0x7,
    HDLC_INFO_WINDOW_SIZE_RX = 0x8
} HDLC_INFO;

typedef enum
{
    DLMS_DATA_TYPE_NONE = 0,
    DLMS_DATA_TYPE_BOOLEAN  = 3,
    DLMS_DATA_TYPE_BIT_STRING = 4,
    DLMS_DATA_TYPE_INT32 = 5,
    DLMS_DATA_TYPE_UINT32 = 6,
    DLMS_DATA_TYPE_OCTET_STRING = 9,
    DLMS_DATA_TYPE_STRING = 10,
    DLMS_DATA_TYPE_BINARY_CODED_DESIMAL = 13,
    DLMS_DATA_TYPE_STRING_UTF8 = 12,
    DLMS_DATA_TYPE_INT8 = 15,
    DLMS_DATA_TYPE_INT16 = 16,
    DLMS_DATA_TYPE_UINT8 = 17,
    DLMS_DATA_TYPE_UINT16 = 18,
    DLMS_DATA_TYPE_INT64 = 20,
    DLMS_DATA_TYPE_UINT64 = 21,
    DLMS_DATA_TYPE_ENUM = 22,
    DLMS_DATA_TYPE_FLOAT32 = 23,
    DLMS_DATA_TYPE_FLOAT64 = 24,
    DLMS_DATA_TYPE_DATETIME = 25,
    DLMS_DATA_TYPE_DATE = 26,
    DLMS_DATA_TYPE_TIME = 27,
    DLMS_DATA_TYPE_ARRAY = 1,
    DLMS_DATA_TYPE_STRUCTURE = 2,
    DLMS_DATA_TYPE_COMPACT_ARRAY = 19
} DLMS_DATA_TYPE;

typedef enum
{
    /// <summary>
    /// No access.
    /// </summary>
    DLMS_ACCESS_MODE_NONE = 0,
    /// <summary>
    /// The client is allowed only reading from the server.
    /// </summary>
    DLMS_ACCESS_MODE_READ = 1,
    /// <summary>
    /// The client is allowed only writing to the server.
    /// </summary>
    DLMS_ACCESS_MODE_WRITE = 2,
    /// <summary>
    /// The client is allowed both reading from the server and writing to it.
    /// </summary>
    DLMS_ACCESS_MODE_READ_WRITE = 3,
    DLMS_ACCESS_MODE_AUTHENTICATED_READ = 4,
    DLMS_ACCESS_MODE_AUTHENTICATED_WRITE = 5,
    DLMS_ACCESS_MODE_AUTHENTICATED_READ_WRITE = 6
} DLMS_ACCESS_MODE;

typedef enum
{
    /// <summary>
    /// No access.
    /// </summary>
    DLMS_METHOD_ACCESS_MODE_NONE = 0,
    /// <summary>
    /// Access.
    /// </summary>
    DLMS_METHOD_ACCESS_MODE_ACCESS = 1,
    /// <summary>
    /// Authenticated Access.
    /// </summary>
    DLMS_METHOD_ACCESS_MODE_AUTHENTICATED_ACCESS = 2
} DLMS_METHOD_ACCESS_MODE;

typedef enum
{
    DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED = 0,
    DLMS_ASSOCIATION_STATUS_ASSOCIATION_PENDING = 1,
    DLMS_ASSOCIATION_STATUS_ASSOCIATED = 2
} DLMS_ASSOCIATION_STATUS;

#endif //ENUMS_H
