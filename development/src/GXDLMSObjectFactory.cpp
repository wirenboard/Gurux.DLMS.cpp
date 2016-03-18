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

#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSActionSchedule.h"
#include "../include/GXDLMSActivityCalendar.h"
#include "../include/GXDLMSAssociationLogicalName.h"
#include "../include/GXDLMSAssociationShortName.h"
#include "../include/GXDLMSAutoAnswer.h"
#include "../include/GXDLMSAutoConnect.h"
#include "../include/GXDLMSClock.h"
#include "../include/GXDLMSData.h"
#include "../include/GXDLMSRegister.h"
#include "../include/GXDLMSExtendedRegister.h"
#include "../include/GXDLMSDemandRegister.h"
#include "../include/GXDLMSRegisterMonitor.h"
#include "../include/GXDLMSProfileGeneric.h"
#include "../include/GXDLMSTcpUdpSetup.h"
#include "../include/GXDLMSGPRSSetup.h"
#include "../include/GXDLMSActionSchedule.h"
#include "../include/GXDLMSSpecialDaysTable.h"
#include "../include/GXDLMSImageTransfer.h"
#include "../include/GXDLMSDisconnectControl.h"
#include "../include/GXDLMSLimiter.h"
#include "../include/GXDLMSMBusClient.h"
#include "../include/GXDLMSScriptTable.h"
#include "../include/GXDLMSMBusSlavePortSetup.h"
#include "../include/GXDLMSSecuritySetup.h"
#include "../include/GXDLMSPppSetup.h"
#include "../include/GXDLMSMacAddressSetup.h"
#include "../include/GXDLMSHdlcSetup.h"
#include "../include/GXDLMSIECOpticalPortSetup.h"
#include "../include/GXDLMSIp4Setup.h"
#include "../include/GXDLMSRegisterActivation.h"
#include "../include/GXDLMSSchedule.h"
#include "../include/GXDLMSModemConfiguration.h"
#include "../include/GXDLMSSapAssignment.h"
#include "../include/GXDLMSMBusMasterPortSetup.h"
#include "../include/GXDLMSMessageHandler.h"
#include "../include/GXDLMSPushSetup.h"

// Reserved for internal use.
CGXDLMSObject* CGXDLMSObjectFactory::CreateObject(OBJECT_TYPE type)
{
    switch(type)
    {
    case OBJECT_TYPE_ACTION_SCHEDULE:
        return new CGXDLMSActionSchedule();
    case OBJECT_TYPE_ACTIVITY_CALENDAR:
        return new CGXDLMSActivityCalendar();
    case OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        return new CGXDLMSAssociationLogicalName();
    case OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        return new CGXDLMSAssociationShortName();
    case OBJECT_TYPE_AUTO_ANSWER:
        return new CGXDLMSAutoAnswer();
    case OBJECT_TYPE_AUTO_CONNECT:
        return new CGXDLMSAutoConnect();
    case OBJECT_TYPE_CLOCK:
        return new CGXDLMSClock();
    case OBJECT_TYPE_DATA:
        return new CGXDLMSData();
    case OBJECT_TYPE_DEMAND_REGISTER:
        return new CGXDLMSDemandRegister();
    case OBJECT_TYPE_MAC_ADDRESS_SETUP:
        return new CGXDLMSMacAddressSetup();
    case OBJECT_TYPE_EXTENDED_REGISTER:
        return new CGXDLMSExtendedRegister();
    case OBJECT_TYPE_GPRS_SETUP:
        return new CGXDLMSGPRSSetup();
    case OBJECT_TYPE_SECURITY_SETUP:
        return new CGXDLMSSecuritySetup();
    case OBJECT_TYPE_IEC_HDLC_SETUP:
        return new CGXDLMSIecHdlcSetup();
    case OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
        return new CGXDLMSIECOpticalPortSetup();
    case OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_IP4_SETUP:
        return new CGXDLMSIp4Setup();
    case OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        return new CGXDLMSMBusSlavePortSetup();
    case OBJECT_TYPE_IMAGE_TRANSFER:
        return new CGXDLMSImageTransfer();
    case OBJECT_TYPE_DISCONNECT_CONTROL:
        return new CGXDLMSDisconnectControl();
    case OBJECT_TYPE_LIMITER:
        return new CGXDLMSLimiter();
    case OBJECT_TYPE_MBUS_CLIENT:
        return new CGXDLMSMBusClient();
    case OBJECT_TYPE_MODEM_CONFIGURATION:
        return new CGXDLMSModemConfiguration();
    case OBJECT_TYPE_PPP_SETUP:
        return new CGXDLMSPppSetup();
    case OBJECT_TYPE_PROFILE_GENERIC:
        return new CGXDLMSProfileGeneric();
    case OBJECT_TYPE_REGISTER:
        return new CGXDLMSRegister();
    case OBJECT_TYPE_REGISTER_ACTIVATION:
        return new CGXDLMSRegisterActivation();
    case OBJECT_TYPE_REGISTER_MONITOR:
        return new CGXDLMSRegisterMonitor();
    case OBJECT_TYPE_REGISTER_TABLE:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_SAP_ASSIGNMENT:
        return new CGXDLMSSapAssignment();
    case OBJECT_TYPE_SCHEDULE:
        return new CGXDLMSSchedule();
    case OBJECT_TYPE_SCRIPT_TABLE:
        return new CGXDLMSScriptTable();
    case OBJECT_TYPE_SMTP_SETUP:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        return new CGXDLMSSpecialDaysTable();
    case OBJECT_TYPE_STATUS_MAPPING:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_TCP_UDP_SETUP:
        return new CGXDLMSTcpUdpSetup();
    case OBJECT_TYPE_UTILITY_TABLES:
        return new CGXDLMSCustomObject(type);
    case OBJECT_TYPE_MBUS_MASTER_PORT_SETUP:
        return new CGXDLMSMBusMasterPortSetup();
    case OBJECT_TYPE_MESSAGE_HANDLER:
        return new CGXDLMSMessageHandler();
    case OBJECT_TYPE_PUSH_SETUP:
        return new CGXDLMSPushSetup();
    default:
        return new CGXDLMSCustomObject(type);
    }
}

CGXDLMSObject* CGXDLMSObjectFactory::CreateObject(OBJECT_TYPE type, std::string ln)
{
    CGXDLMSObject* pObj = CreateObject(type);
    GXHelpers::SetLogicalName(ln.c_str(), pObj->m_LN);
    return pObj;
}

std::string CGXDLMSObjectFactory::ObjectTypeToString(OBJECT_TYPE type)
{
    switch(type)
    {
    case OBJECT_TYPE_ACTION_SCHEDULE:
        return "GXDLMSActionSchedule";
    case OBJECT_TYPE_ACTIVITY_CALENDAR:
        return "GXDLMSActivityCalendar";
    case OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        return "GXDLMSAssociationLogicalName";
    case OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        return "GXDLMSAssociationShortName";
    case OBJECT_TYPE_AUTO_ANSWER:
        return "GXDLMSAutoAnswer";
    case OBJECT_TYPE_AUTO_CONNECT:
        return "GXDLMSAutoConnect";
    case OBJECT_TYPE_CLOCK:
        return "GXDLMSClock";
    case OBJECT_TYPE_DATA:
        return "GXDLMSData";
    case OBJECT_TYPE_DEMAND_REGISTER:
        return "GXDLMSDemandRegister";
    case OBJECT_TYPE_MAC_ADDRESS_SETUP:
        return "GXDLMSMacAddressSetup";
    case OBJECT_TYPE_EXTENDED_REGISTER:
        return "GXDLMSExtendedRegister";
    case OBJECT_TYPE_GPRS_SETUP:
        return "GXDLMSGprsSetup";
    case OBJECT_TYPE_SECURITY_SETUP:
        return "GXDLMSSecuritySetup";
    case OBJECT_TYPE_IEC_HDLC_SETUP:
        return "GXDLMSHdlcSetup";
    case OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
        return "GXDLMSIECOpticalPortSetup";
    case OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
        return "GXDLMSIEC_TWISTED_PAIR_SETUP";
    case OBJECT_TYPE_IP4_SETUP:
        return "GXDLMSIp4Setup";
    case OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        return "GXDLMSMBusSlavePortSetup";
    case OBJECT_TYPE_IMAGE_TRANSFER:
        return "GXDLMSImageTransfer";
    case OBJECT_TYPE_DISCONNECT_CONTROL:
        return "GXDLMSDisconnectControl";
    case OBJECT_TYPE_LIMITER:
        return "GXDLMSLimiter";
    case OBJECT_TYPE_MBUS_CLIENT:
        return "GXDLMSMBusClient";
    case OBJECT_TYPE_MODEM_CONFIGURATION:
        return "GXDLMSModemConfiguration";
    case OBJECT_TYPE_PPP_SETUP:
        return "GXDLMSPppSetup";
    case OBJECT_TYPE_PROFILE_GENERIC:
        return "GXDLMSProfileGeneric";
    case OBJECT_TYPE_REGISTER:
        return "GXDLMSRegister";
    case OBJECT_TYPE_REGISTER_ACTIVATION:
        return "GXDLMSRegisterActivation";
    case OBJECT_TYPE_REGISTER_MONITOR:
        return "GXDLMSRegisterMonitor";
    case OBJECT_TYPE_REGISTER_TABLE:
        return "GXDLMSRegisterTable";
    case OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
        return "GXDLMSZigBeeSasStartup";
    case OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
        return "GXDLMSZigBeeSasJoin";
    case OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
        return "GXDLMSZigBeeSasApsFragmentation";
    case OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL:
        return "GXDLMSZigBeeNetworkControl";
    case OBJECT_TYPE_SAP_ASSIGNMENT:
        return "GXDLMSSapAssignment";
    case OBJECT_TYPE_SCHEDULE:
        return "GXDLMSSchedule";
    case OBJECT_TYPE_SCRIPT_TABLE:
        return "GXDLMSScriptTable";
    case OBJECT_TYPE_SMTP_SETUP:
        return "GXDLMSSMTPSetup";
    case OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        return "GXDLMSSpecialDaysTable";
    case OBJECT_TYPE_STATUS_MAPPING:
        return "GXDLMSStatusMapping";
    case OBJECT_TYPE_TCP_UDP_SETUP:
        return "GXDLMSTcpUdpSetup";
    case OBJECT_TYPE_UTILITY_TABLES:
        return "GXDLMSUtilityTables";
    case OBJECT_TYPE_MBUS_MASTER_PORT_SETUP:
        return "GXDLMSMBusMasterPortSetup";
    case OBJECT_TYPE_MESSAGE_HANDLER:
        return "GXDLMSMessageHandler";
    case OBJECT_TYPE_PUSH_SETUP:
        return "GXDLMSPushSetup";
    default:
        return "Manufacture spesific.";
    }
}
