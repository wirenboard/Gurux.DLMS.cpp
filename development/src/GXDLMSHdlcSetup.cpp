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

#include "../include/GXDLMSHdlcSetup.h"
#include "../include/GXDLMSConverter.h"

//Constructor.
CGXDLMSIecHdlcSetup::CGXDLMSIecHdlcSetup() : CGXDLMSObject(OBJECT_TYPE_IEC_HDLC_SETUP)
{
    m_CommunicationSpeed = BAUDRATE_9600;
    m_WindowSizeTransmit = m_WindowSizeReceive = 1;
    m_MaximumInfoLengthTransmit = m_MaximumInfoLengthReceive = 128;
}

//SN Constructor.
CGXDLMSIecHdlcSetup::CGXDLMSIecHdlcSetup(unsigned short sn) : CGXDLMSObject(OBJECT_TYPE_IEC_HDLC_SETUP, sn)
{
    m_CommunicationSpeed = BAUDRATE_9600;
    m_WindowSizeTransmit = m_WindowSizeReceive = 1;
    m_MaximumInfoLengthTransmit = m_MaximumInfoLengthReceive = 128;
}

//LN Constructor.
CGXDLMSIecHdlcSetup::CGXDLMSIecHdlcSetup(std::string ln) : CGXDLMSObject(OBJECT_TYPE_IEC_HDLC_SETUP, ln)
{
    m_CommunicationSpeed = BAUDRATE_9600;
    m_WindowSizeTransmit = m_WindowSizeReceive = 1;
    m_MaximumInfoLengthTransmit = m_MaximumInfoLengthReceive = 128;
}

BAUDRATE CGXDLMSIecHdlcSetup::GetCommunicationSpeed()
{
    return m_CommunicationSpeed;
}
void CGXDLMSIecHdlcSetup::SetCommunicationSpeed(BAUDRATE value)
{
    m_CommunicationSpeed = value;
}

int CGXDLMSIecHdlcSetup::GetWindowSizeTransmit()
{
    return m_WindowSizeTransmit;
}
void CGXDLMSIecHdlcSetup::SetWindowSizeTransmit(int value)
{
    m_WindowSizeTransmit = value;
}

int CGXDLMSIecHdlcSetup::GetWindowSizeReceive()
{
    return m_WindowSizeReceive;
}
void CGXDLMSIecHdlcSetup::SetWindowSizeReceive(int value)
{
    m_WindowSizeReceive = value;
}

int CGXDLMSIecHdlcSetup::GetMaximumInfoLengthTransmit()
{
    return m_MaximumInfoLengthTransmit;
}
void CGXDLMSIecHdlcSetup::SetMaximumInfoLengthTransmit(int value)
{
    m_MaximumInfoLengthTransmit = value;
}

int CGXDLMSIecHdlcSetup::GetMaximumInfoLengthReceive()
{
    return m_MaximumInfoLengthReceive;
}
void CGXDLMSIecHdlcSetup::SetMaximumInfoLengthReceive(int value)
{
    m_MaximumInfoLengthReceive = value;
}

int CGXDLMSIecHdlcSetup::GetInterCharachterTimeout()
{
    return m_InterCharachterTimeout;
}
void CGXDLMSIecHdlcSetup::SetInterCharachterTimeout(int value)
{
    m_InterCharachterTimeout = value;
}

int CGXDLMSIecHdlcSetup::GetInactivityTimeout()
{
    return m_InactivityTimeout;
}
void CGXDLMSIecHdlcSetup::SetInactivityTimeout(int value)
{
    m_InactivityTimeout = value;
}

int CGXDLMSIecHdlcSetup::GetDeviceAddress()
{
    return m_DeviceAddress;
}
void CGXDLMSIecHdlcSetup::SetDeviceAddress(int value)
{
    m_DeviceAddress = value;
}

// Returns amount of attributes.
int CGXDLMSIecHdlcSetup::GetAttributeCount()
{
    return 9;
}

// Returns amount of methods.
int CGXDLMSIecHdlcSetup::GetMethodCount()
{
    return 0;
}

void CGXDLMSIecHdlcSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(CGXDLMSConverter::ToString(m_CommunicationSpeed));
    values.push_back(CGXDLMSVariant(m_WindowSizeTransmit).ToString());
    values.push_back(CGXDLMSVariant(m_WindowSizeReceive).ToString());
    values.push_back(CGXDLMSVariant(m_MaximumInfoLengthTransmit).ToString());
    values.push_back(CGXDLMSVariant(m_MaximumInfoLengthReceive).ToString());
    values.push_back(CGXDLMSVariant(m_InterCharachterTimeout).ToString());
    values.push_back(CGXDLMSVariant(m_InactivityTimeout).ToString());
    values.push_back(CGXDLMSVariant(m_DeviceAddress).ToString());
}

void CGXDLMSIecHdlcSetup::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CommunicationSpeed
    if (!IsRead(2))
    {
        attributes.push_back(2);
    }
    //WindowSizeTransmit
    if (!IsRead(3))
    {
        attributes.push_back(3);
    }
    //WindowSizeReceive
    if (!IsRead(4))
    {
        attributes.push_back(4);
    }
    //MaximumInfoLengthTransmit
    if (!IsRead(5))
    {
        attributes.push_back(5);
    }
    //MaximumInfoLengthReceive
    if (!IsRead(6))
    {
        attributes.push_back(6);
    }
    //InterCharachterTimeout
    if (!IsRead(7))
    {
        attributes.push_back(7);
    }
    //InactivityTimeout
    if (!IsRead(8))
    {
        attributes.push_back(8);
    }
    //DeviceAddress
    if (!IsRead(9))
    {
        attributes.push_back(9);
    }
}

int CGXDLMSIecHdlcSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ENUM;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 8)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 9)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}

// Returns value of given attribute.
int CGXDLMSIecHdlcSetup::GetValue(int index, int selector, CGXDLMSVariant& parameters, CGXDLMSVariant& value)
{
    if (index == 1)
    {
        return GetLogicalName(this, value);
    }
    else if (index == 2)
    {
        value = m_CommunicationSpeed;
    }
    else if (index == 3)
    {
        value = m_WindowSizeTransmit;
    }
    else if (index == 4)
    {
        value = m_WindowSizeReceive;
    }
    else if (index == 5)
    {
        value = m_MaximumInfoLengthTransmit;
    }
    else if (index == 6)
    {
        value = m_MaximumInfoLengthReceive;
    }
    else if (index == 7)
    {
        value = m_InterCharachterTimeout;
    }
    else if (index == 8)
    {
        value = m_InactivityTimeout;
    }
    else if (index == 9)
    {
        value = m_DeviceAddress;
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}

// Set value of given attribute.
int CGXDLMSIecHdlcSetup::SetValue(CGXDLMSSettings* settings, int index, CGXDLMSVariant& value)
{
    if (index == 1)
    {
        return SetLogicalName(this, value);
    }
    else if (index == 2)
    {
        m_CommunicationSpeed = (BAUDRATE) value.ToInteger();
    }
    else if (index == 3)
    {
        m_WindowSizeTransmit = value.ToInteger();
    }
    else if (index == 4)
    {
        m_WindowSizeReceive = value.ToInteger();
    }
    else if (index == 5)
    {
        m_MaximumInfoLengthTransmit = value.ToInteger();
    }
    else if (index == 6)
    {
        m_MaximumInfoLengthReceive = value.ToInteger();
    }
    else if (index == 7)
    {
        m_InterCharachterTimeout = value.ToInteger();
    }
    else if (index == 8)
    {
        m_InactivityTimeout = value.ToInteger();
    }
    else if (index == 9)
    {
        m_DeviceAddress = value.ToInteger();
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}