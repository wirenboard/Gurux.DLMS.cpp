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

#include "../include/GXDLMSConverter.h"
#include "../include/errorcodes.h"

const char* CGXDLMSConverter::GetErrorMessage(int error)
{
    switch(error)
    {
    case ERROR_CODES_INVALID_PARAMETER:
        return "Invalid parameter.";
    case ERROR_CODES_NOT_INITIALIZED:
        return "Server is not initialized.";
    case ERROR_CODES_OUTOFMEMORY:
        return "Not enough memory available.";
    case ERROR_CODES_NOT_REPLY:
        return "Packet is not a reply for a send packet.";
    case ERROR_CODES_FRAME_REJECTED:
        return "Meter rejects send packet.";
    case ERROR_CODES_INVALID_LOGICAL_NAME:
        return "Invalid Logical Name.";
    case ERROR_CODES_INVALID_CLIENT_ADDRESS:
        return "Client HDLC Address is not set.";
    case ERROR_CODES_INVALID_SERVER_ADDRESS:
        return "Server HDLC Address is not set.";
    case ERROR_CODES_INVALID_DATA_FORMAT:
        return "Not a HDLC frame.";
    case ERROR_CODES_INVALID_VERSION_NUMBER:
        return "Invalid DLMS version number.";
    case ERROR_CODES_CLIENT_ADDRESS_NO_NOT_MATCH:
        return "Client addresses do not match.";
    case ERROR_CODES_SERVER_ADDRESS_NO_NOT_MATCH:
        return "Server addresses do not match.";
    case ERROR_CODES_WRONG_CRC:
        return "CRC do not match.";
    case ERROR_CODES_INVALID_RESPONSE:
        return "Invalid response";
    case ERROR_CODES_INVALID_TAG:
        return "Invalid Tag.";
    case ERROR_CODES_ENCODING_FAILED:
        return "Encoding failed. Not enough data.";
    case ERROR_CODES_REJECTED_PERMAMENT:
        return "Rejected permament.";
    case ERROR_CODES_REJECTED_TRANSIENT:
        return "Rejected transient.";
    case ERROR_CODES_NO_REASON_GIVEN:
        return "No reason given.";
    case ERROR_CODES_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
        return "Application context name not supported.";
    case ERROR_CODES_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
        return "Authentication mechanism name not recognised.";
    case ERROR_CODES_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
        return "Authentication mechanism name required.";
    case ERROR_CODES_AUTHENTICATION_FAILURE:
        return "Authentication failure.";
    case ERROR_CODES_AUTHENTICATION_REQUIRED:
        return "Authentication required.";
    case ERROR_CODES_HARDWARE_FAULT:
        return "Access Error : Device reports a hardware fault.";
    case ERROR_CODES_TEMPORARY_FAILURE:
        return "Access Error : Device reports a temporary failure.";
    case ERROR_CODES_WRITE_DENIED:
        return "Access Error : Device reports Read-Write denied.";
    case ERROR_CODES_UNDEFINED_OBJ:
        return "Access Error : Device reports a undefined object.";
    case ERROR_CODES_INCONSISTENT_OBJ:
        return "Access Error : Device reports a inconsistent Class or Object.";
    case ERROR_CODES_UNAVAILABLE_OBJ:
        return "Access Error : Device reports a unavailable object.";
    case ERROR_CODES_UNMATCH_TYPE:
        return "Access Error : Device reports a unmatched type.";
    case ERROR_CODES_VIOLATED:
        return "Access Error : Device reports scope of access violated.";
    case ERROR_CODES_BLOCK_UNAVAILABLE:
        return "Access Error : Data Block Unavailable.";
    case ERROR_CODES_READ_ABORTED:
        return "Access Error : Long Get Or Read Aborted.";
    case ERROR_CODES_READ_IN_PROGRESS:
        return "Access Error : No Long Get Or Read In Progress.";
    case ERROR_CODES_WRITE_ABORTED:
        return "Access Error : Long Set Or Write Aborted.";
    case ERROR_CODES_WRITE_IN_PROGRESS:
        return "Access Error : No Long Set Or Write In Progress.";
    case ERROR_CODES_BLOCK_NUMBER_INVALID:
        return "Access Error : Data Block Number Invalid.";
    case ERROR_CODES_OTHER_REASON:
        return "Access Error : Other Reason.";
    case ERROR_CODES_UNKNOWN:
        return "Unknown error.";
    case ERROR_CODES_SEND_FAILED:
        return "Data send failed.";
    case ERROR_CODES_RECEIVE_FAILED:
        return "Data receive failed.";
    case ERROR_CODES_NOT_IMPLEMENTED:
        return "Not implemeted.";
    }
    return "Unknown error.";
}
const char* CGXDLMSConverter::ToString(BAUDRATE value)
{
    switch(value)
    {
    case 0:
        return "Baudrate300";
        break;
    case 1:
        return "Baudrate600";
        break;
    case 2:
        return "Baudrate1200";
        break;
    case 3:
        return "Baudrate2400";
        break;
    case 4:
        return "Baudrate4000";
        break;
    case 5:
        return "Baudrate9600";
        break;
    case 6:
        return "Baudrate19200";
        break;
    case 7:
        return "Baudrate38400";
        break;
    case 8:
        return "Baudrate57600";
        break;
    case 9:
        return "Baudrate115200";
        break;
    default:
        break;
    }
    return "Unknown baudrate";
}

const char* CGXDLMSConverter::ToString(GXDLMS_CLOCK_STATUS value)
{
    switch(value)
    {
    case 0:
        return "None";
        break;
    case 0x1:
        return "InvalidValue";
        break;
    case 0x2:
        return "DoubtfulValue";
        break;
    case 0x4:
        return "DifferentClockBase";
        break;
    case 0x8:
        return "Reserved1";
        break;
    case 0x10:
        return "Reserved2";
        break;
    case 0x20:
        return "Reserved3";
        break;
    case 0x40:
        return "Reserved4";
        break;
    case 0x80:
        return "DaylightSaveActive";
        break;
    default:
        break;
    }
    return "Unknown Clock status";
}

const char* CGXDLMSConverter::ToString(CLOCKBASE value)
{
    switch(value)
    {
    case 0:
        return "None";
        break;
    case 1:
        return "Crystal";
        break;
    case 2:
        return "Frequency50";
        break;
    case 3:
        return "Frequency60";
        break;
    case 4:
        return "GPS";
        break;
    case 5:
        return "Radio";
        break;
    default:
        break;
    }
    return "Unknown Clock base";
}

const char* CGXDLMSConverter::GetUnitAsString(int unit)
{
    switch(unit)
    {
    case 0:
        return "None";
    case 1:
        return "Year";
        break;
    case 2:
        return "Month";
        break;
    case 3:
        return "Week";
        break;
    case 4:
        return "Day";
        break;
    case 5:
        return "Hour";
        break;
    case 6:
        return "Minute";
        break;
    case 7:
        return "Second";
        break;
    case 8:
        return "PhaseAngle";
        break;
    case 9:
        return "Temperature";
        break;
    case 10:
        return "LocalCurrency";
        break;
    case 11:
        return "Length";
        break;
    case 12:
        return "Speed";
        break;
    case 13:
        return "Volume";
        break;
    case 14:
        return "CorrectedVolume";
        break;
    case 15:
        return "VolumeFlux";
        break;
    case 16:
        return "CorrectedVolumeFlux";
        break;
    case 17:
        return "VolumeFlux";
        break;
    case 18:
        return "CorrectedVolumeFlux";
        break;
    case 19:
        return "Volume";
        break;
    case 20:
        return "MassKg";
        break;
    case 21:
        return "Force";
        break;
    case 22:
        return "Energy";
        break;
    case 23:
        return "PressurePascal";
        break;
    case 24:
        return "PressureBar";
        break;
    case 25:
        return "Energy";
        break;
    case 26:
        return "ThermalPower";
        break;
    case 27:
        return "ActivePower";
        break;
    case 28:
        return "ApparentPower";
        break;
    case 29:
        return "ReactivePower";
        break;
    case 30:
        return "ActiveEnergy";
        break;
    case 31:
        return "ApparentEnergy";
        break;
    case 32:
        return "ReactiveEnergy";
        break;
    case 33:
        return "Current";
        break;
    case 34:
        return "ElectricalCharge";
        break;
    case 35:
        return "Voltage";
        break;
    case 36:
        return "Electrical field strength E V/m";
        break;
    case 37:
        return "Capacity C farad C/V = As/V";
        break;
    case 38:
        return "Resistance";
        break;
    case 39:
        return "Resistivity";
        break;
    case 40:
        return "Magnetic flux F weber Wb = Vs";
        break;
    case 41:
        return "Induction T tesla Wb/m2";
        break;
    case 42:
        return "Magnetic field strength H A/m";
        break;
    case 43:
        return "Inductivity L henry H = Wb/A";
        break;
    case 44:
        return "Frequency";
        break;
    case 45:
        return "ActiveEnergy";
        break;
    case 46:
        return "ReactiveEnergy";
        break;
    case 47:
        return "ApparentEnergy";
        break;
    case 48:
        return "V260*60s";
        break;
    case 49:
        return "A260*60s";
        break;
    case 50:
        return "Mass";
        break;
    case 51:
        return "ConductanceSiemens";
        break;
    case 254 :
        return "OtherUnit";
        break;
    case 255:
        return "NoUnit";
        break;
    }
    return "";
}


const char* CGXDLMSConverter::ToString(ADDRESS_STATE value)
{
    switch(value)
    {
    case 0:
        return "None";
        break;
    case 1:
        return "Assigned";
        break;
    default:
        break;
    }
    return "Unknown Clock base";
}

//Get Disconnect control control state as a std::string.
const char* CGXDLMSConverter::ToString(CONTROLSTATE value)
{
    switch(value)
    {
    case 0:
        return "Disconnected";
        break;
    case 1:
        return "Connected";
        break;
    case 2:
        return "ReadyForReconnection";
        break;
    default:
        break;
    }
    return "Unknown control state";
}

//Get Disconnect control control mode as a std::string.
const char* CGXDLMSConverter::ToString(CONTROLMODE value)
{
    switch(value)
    {
    case 0:
        return "None";
        break;
    case 1:
        return "Mode1";
        break;
    case 2:
        return "Mode2";
        break;
    case 3:
        return "Mode3";
        break;
    case 4:
        return "Mode4";
        break;
    case 5:
        return "Mode5";
        break;
    case 6:
        return "Mode6";
        break;
    default:
        break;
    }
    return "Unknown control mode";
}

//Get Security Setup security policy as a std::string.
const char* CGXDLMSConverter::ToString(SECURITY_POLICY value)
{
    switch(value)
    {
    case 0:
        return "Nothing";
        break;
    case 1:
        return "Authenticated";
        break;
    case 2:
        return "Encrypted";
        break;
    case 3:
        return "AuthenticatedEncrypted";
        break;
    default:
        break;
    }
    return "Unknown security policy";
}

//Get Security Setup security suite as a std::string.
const char* CGXDLMSConverter::ToString(SECURITY_SUITE value)
{
    switch(value)
    {
    case 0://SECURITY_SUITE_AES_GCM_128
        return "AES_GCM_128";
        break;
    default:
        break;
    }
    return "Unknown security suite";
}

const char* CGXDLMSConverter::ToString(GX_ASSOCIATION_STATUS value)
{
    switch(value)
    {
    case GX_ASSOCIATION_STATUS_NON_ASSOCIATED:
        return "NonAssociated";
        break;
    case GX_ASSOCIATION_STATUS_ASSOCIATION_PENDING:
        return "AssociationPending";
        break;
    case GX_ASSOCIATION_STATUS_ASSOCIATED:
        return "Associated";
        break;
    default:
        break;
    }
    return "Unknown association status";
}

const char* CGXDLMSConverter::ToString(OPTICAL_PROTOCOL_MODE value)
{
    switch(value)
    {
    case OPTICAL_PROTOCOL_MODE_DEFAULT:
        return "Default";
        break;
    case OPTICAL_PROTOCOL_MODE_NET:
        return "Net";
        break;
    case OPTICAL_PROTOCOL_MODE_UNKNOWN:
        return "Unknown";
        break;
    default:
        break;
    }
    return "Unknown ptotocol mode";
}

const char* CGXDLMSConverter::ToString(LOCAL_PORT_RESPONSE_TIME value)
{
    switch(value)
    {
    case LOCAL_PORT_RESPONSE_TIME_20_MS:
        return "ms20";
        break;
    case LOCAL_PORT_RESPONSE_TIME_200_MS:
        return "ms200";
        break;
    default:
        break;
    }
    return "Unknown response time";
}
