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

#include "../include/GXDLMSSettings.h"

CGXDLMSSettings::CGXDLMSSettings(bool isServer)
{
    m_BlockIndex = 1;
    m_Connected = false;
    m_DlmsVersionNumber = DLMS_VERSION;
    m_Server = isServer;
    ResetFrameSequence();
    m_InvokeID = 1;
    m_Priority = GXDLMS_PRIORITY_HIGH;
    m_ServiceClass = GXDLMS_SERVICECLASS_UN_CONFIRMED;
    m_ClientAddress = 0;
    m_ServerAddress = 0;
    m_InterfaceType = GXDLMS_INTERFACETYPE_HDLC;
    m_Authentication = GXDLMS_AUTHENTICATION_NONE;
    m_MaxReceivePDUSize = 0xFFFF;
}

CGXByteBuffer& CGXDLMSSettings::GetCtoSChallenge()
{
    return m_CtoSChallenge;
}


void CGXDLMSSettings::SetCtoSChallenge(CGXByteBuffer value)
{
    if (!m_CustomChallenges || m_CtoSChallenge.GetSize() == 0)
    {
        m_CtoSChallenge = value;
    }
}

CGXByteBuffer& CGXDLMSSettings::GetStoCChallenge()
{
    return m_StoCChallenge;
}

void CGXDLMSSettings::SetStoCChallenge(CGXByteBuffer value)
{
    if (!m_CustomChallenges || m_StoCChallenge.GetSize() == 0)
    {
        m_StoCChallenge = value;
    }
}

GXDLMS_AUTHENTICATION CGXDLMSSettings::GetAuthentication()
{
    return m_Authentication;
}

void CGXDLMSSettings::SetAuthentication(GXDLMS_AUTHENTICATION value)
{
    m_Authentication = value;
}

CGXByteBuffer& CGXDLMSSettings::GetPassword()
{
    return m_Password;
}

void CGXDLMSSettings::SetPassword(CGXByteBuffer value)
{
    m_Password = value;
}

unsigned char CGXDLMSSettings::GetDlmsVersionNumber()
{
    return m_DlmsVersionNumber;
}

void CGXDLMSSettings::SetDlmsVersionNumber(unsigned char value)
{
    m_DlmsVersionNumber = value;
}

void CGXDLMSSettings::ResetFrameSequence()
{
    if (m_Server)
    {
        m_SenderFrame = SERVER_START_SENDER_FRAME_SEQUENCE;
        m_ReceiverFrame = SERVER_START_RECEIVER_FRAME_SEQUENCE;
    }
    else
    {
        m_SenderFrame = CLIENT_START_SENDER_FRAME_SEQUENCE;
        m_ReceiverFrame = CLIENT_START_RCEIVER_FRAME_SEQUENCE;
    }
}

bool CGXDLMSSettings::CheckFrame(unsigned char frame)
{
    // If U frame.
    if ((frame & 0x3) == 3)
    {
        ResetFrameSequence();
        return true;
    }
    // If S -frame
    if ((frame & 0x3) == 1)
    {
        if ((frame & 0xE0) == ((m_ReceiverFrame) & 0xE0))
        {
            m_ReceiverFrame = frame;
            return true;
        }
        return true;
    }

    // If I frame sent.
    if ((m_SenderFrame & 0x1) == 0)
    {
        if ((frame & 0xE0) == ((m_ReceiverFrame + 0x20) & 0xE0)
                && (frame & 0xE) == ((m_ReceiverFrame + 2) & 0xE))
        {
            m_ReceiverFrame = frame;
            return true;
        }
    }
    else if (frame == m_ReceiverFrame
             || ((frame & 0xE0) == (m_ReceiverFrame & 0xE0)
                 && (frame & 0xE) == ((m_ReceiverFrame + 2) & 0xE)))
    {
        // If S-frame sent.
        m_ReceiverFrame = frame;
        return true;
    }
    return true;
}

// Increase receiver sequence.
//
// @param value
//            Frame value.
// Increased receiver frame sequence.
static unsigned char IncreaseReceiverSequence(unsigned char value)
{
    return ((value + 0x20) | 0x10 | (value & 0xE));
}

// Increase sender sequence.
//
// @param value
//            Frame value.
// Increased sender frame sequence.
static unsigned char IncreaseSendSequence(unsigned char value)
{
    return (unsigned char) ((value & 0xF0) | ((value + 0x2) & 0xE));
}

unsigned char CGXDLMSSettings::GetNextSend()
{
    m_SenderFrame = IncreaseReceiverSequence(IncreaseSendSequence((unsigned char) m_SenderFrame));
    return (unsigned char) m_SenderFrame;
}

unsigned char CGXDLMSSettings::GetReceiverReady()
{
    m_SenderFrame = IncreaseReceiverSequence((unsigned char) (m_SenderFrame | 1));
    return (unsigned char) (m_SenderFrame & 0xF1);
}

unsigned char CGXDLMSSettings::GetKeepAlive()
{
    m_SenderFrame = (unsigned char) (m_SenderFrame | 1);
    return (unsigned char) (m_SenderFrame & 0xF1);
}

CGXDLMSLNSettings& CGXDLMSSettings::GetLnSettings()
{
    return m_LNSettings;
}

CGXDLMSSNSettings CGXDLMSSettings::GetSnSettings()
{
    return m_SNSettings;
}

unsigned long CGXDLMSSettings::GetBlockIndex()
{
    return m_BlockIndex;
}

void CGXDLMSSettings::SetBlockIndex(unsigned long value)
{
    m_BlockIndex = value;
}

void CGXDLMSSettings::ResetBlockIndex()
{
    m_BlockIndex = 1;
}

void CGXDLMSSettings::IncreaseBlockIndex()
{
    m_BlockIndex += 1;
}

bool CGXDLMSSettings::IsServer()
{
    return m_Server;
}

CGXDLMSLimits& CGXDLMSSettings::GetLimits()
{
    return m_Limits;
}

GXDLMS_INTERFACETYPE CGXDLMSSettings::GetInterfaceType()
{
    return m_InterfaceType;
}

void CGXDLMSSettings::SetInterfaceType(GXDLMS_INTERFACETYPE value)
{
    m_InterfaceType = value;
}

int CGXDLMSSettings::GetClientAddress()
{
    return m_ClientAddress;
}

void CGXDLMSSettings::SetClientAddress(int value)
{
    m_ClientAddress = value;
}

int CGXDLMSSettings::GetServerAddress()
{
    return m_ServerAddress;
}

// Server address.
void CGXDLMSSettings::SetServerAddress(int value)
{
    m_ServerAddress = value;
}

// DLMS version number.
unsigned char CGXDLMSSettings::GetDLMSVersion()
{
    return m_DlmsVersionNumber;
}

// DLMS version number.
void CGXDLMSSettings::SetDLMSVersion(unsigned char value)
{
    m_DlmsVersionNumber = value;
}

// Maximum PDU size.
int CGXDLMSSettings::GetMaxReceivePDUSize()
{
    return m_MaxReceivePDUSize;
}

void CGXDLMSSettings::SetMaxReceivePDUSize(int value)
{
    m_MaxReceivePDUSize = value;
}

bool CGXDLMSSettings::GetUseLogicalNameReferencing()
{
    return m_UseLogicalNameReferencing;
}

void CGXDLMSSettings::SetUseLogicalNameReferencing(bool value)
{
    m_UseLogicalNameReferencing = value;
}

GXDLMS_PRIORITY CGXDLMSSettings::GetPriority()
{
    return m_Priority;
}

void CGXDLMSSettings::SetPriority(GXDLMS_PRIORITY value)
{
    m_Priority = value;
}

GXDLMS_SERVICECLASS CGXDLMSSettings::GetServiceClass()
{
    return m_ServiceClass;
}

void CGXDLMSSettings::SetServiceClass(GXDLMS_SERVICECLASS value)
{
    m_ServiceClass = value;
}

int CGXDLMSSettings::GetInvokeID()
{
    return m_InvokeID;
}

void CGXDLMSSettings::SetInvokeID(int value)
{
    m_InvokeID = value;
}

CGXDLMSObjectCollection& CGXDLMSSettings::GetObjects()
{
    return m_Objects;
}

bool CGXDLMSSettings::IsCustomChallenges()
{
    return m_CustomChallenges;
}

void CGXDLMSSettings::SetCustomChallenges(bool value)
{
    m_CustomChallenges = value;
}

bool CGXDLMSSettings::IsConnected()
{
    return m_Connected;
}

void CGXDLMSSettings::SetConnected(bool value)
{
    m_Connected = value;
}

