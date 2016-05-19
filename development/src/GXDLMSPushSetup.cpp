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

#include "../include/GXDLMSPushSetup.h"
#include "../include/GXDLMSClient.h"
#include <sstream>

//Constructor.
CGXDLMSPushSetup::CGXDLMSPushSetup() : CGXDLMSObject(DLMS_OBJECT_TYPE_PUSH_SETUP)
{
    m_RandomisationStartInterval = m_NumberOfRetries = m_RepetitionDelay = 0;
}

//SN Constructor.
CGXDLMSPushSetup::CGXDLMSPushSetup(unsigned short sn) : CGXDLMSObject(DLMS_OBJECT_TYPE_PUSH_SETUP, sn)
{
    m_RandomisationStartInterval = m_NumberOfRetries = m_RepetitionDelay = 0;
}

//LN Constructor.
CGXDLMSPushSetup::CGXDLMSPushSetup(std::string ln) : CGXDLMSObject(DLMS_OBJECT_TYPE_PUSH_SETUP, ln)
{
    m_RandomisationStartInterval = m_NumberOfRetries = m_RepetitionDelay = 0;
}

// Returns amount of attributes.
int CGXDLMSPushSetup::GetAttributeCount()
{
    return 7;
}

// Returns amount of methods.
int CGXDLMSPushSetup::GetMethodCount()
{
    return 1;
}

void CGXDLMSPushSetup::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for(std::vector<CGXDLMSPushObject>::iterator it = m_PushObjectList.begin(); it != m_PushObjectList.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());

    sb.str(std::string());
    sb << m_SendDestinationAndMethod.GetService();
    sb << " ";
    sb << m_SendDestinationAndMethod.GetDestination().c_str();
    sb << " ";
    sb << m_SendDestinationAndMethod.GetMessage();
    values.push_back(sb.str());

    sb.str(std::string());
    sb << '[';
    empty = true;
    for(std::vector<std::pair<CGXDateTime, CGXDateTime> >::iterator it = m_CommunicationWindow.begin(); it != m_CommunicationWindow.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        std::string str = it->first.ToString();
        sb.write(str.c_str(), str.size());
        sb << " ";
        str = it->second.ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());

    values.push_back(CGXDLMSVariant(m_RandomisationStartInterval).ToString());
    values.push_back(CGXDLMSVariant(m_NumberOfRetries).ToString());
    values.push_back(CGXDLMSVariant(m_RepetitionDelay).ToString());
}

void CGXDLMSPushSetup::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //PushObjectList
    if (CanRead(2))
    {
        attributes.push_back(2);
    }

    //SendDestinationAndMethod
    if (CanRead(3))
    {
        attributes.push_back(3);
    }
    //CommunicationWindow
    if (CanRead(4))
    {
        attributes.push_back(4);
    }
    //RandomisationStartInterval
    if (CanRead(5))
    {
        attributes.push_back(5);
    }
    //NumberOfRetries
    if (CanRead(6))
    {
        attributes.push_back(6);
    }
    //RepetitionDelay
    if (CanRead(7))
    {
        attributes.push_back(7);
    }
}

int CGXDLMSPushSetup::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 5)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else if (index == 6)
    {
        type = DLMS_DATA_TYPE_UINT8;
    }
    else if (index == 7)
    {
        type = DLMS_DATA_TYPE_UINT16;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns value of given attribute.
int CGXDLMSPushSetup::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    if (e.GetIndex() == 1)
    {
        int ret;
        CGXDLMSVariant tmp;
        if ((ret = GetLogicalName(this, tmp)) != 0)
        {
            return ret;
        }
        e.SetValue(tmp);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 2)
    {
        //TODO: e.SetValue(m_PushObjectList;
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        //TODO: e.SetValue(m_SendDestinationAndMethod;
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        //TODO: e.SetValue(m_CommunicationWindow;
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        e.SetValue(m_RandomisationStartInterval);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.SetValue(m_NumberOfRetries);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(m_RepetitionDelay);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSPushSetup::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_PushObjectList.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for(std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                CGXDLMSPushObject obj;
                obj.SetType((DLMS_OBJECT_TYPE) it->Arr[0].ToInteger());
                CGXDLMSVariant tmp;
                CGXDLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_BIT_STRING, tmp);
                obj.SetLogicalName(tmp.ToString());
                obj.SetAttributeIndex(it->Arr[2].ToInteger());
                obj.SetDataIndex(it->Arr[3].ToInteger());
                m_PushObjectList.push_back(obj);
            }
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            m_SendDestinationAndMethod.SetService((DLMS_SERVICE_TYPE) e.GetValue().Arr[0].ToInteger());
            std::string str;
            str.append(reinterpret_cast< char const* >(e.GetValue().Arr[1].byteArr), e.GetValue().Arr[1].size);
            m_SendDestinationAndMethod.SetDestination(str);
            m_SendDestinationAndMethod.SetMessage((DLMS_MESSAGE_TYPE) e.GetValue().Arr[2].ToInteger());
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_CommunicationWindow.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            int ret;
            for(std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                CGXDLMSVariant tmp;
                if ((ret = CGXDLMSClient::ChangeType(it->Arr[0], DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
                {
                    return ret;
                }
                CGXDateTime start = tmp.dateTime;
                if ((ret = CGXDLMSClient::ChangeType(it->Arr[1], DLMS_DATA_TYPE_DATETIME, tmp)) != 0)
                {
                    return ret;
                }
                CGXDateTime end = tmp.dateTime;
                m_CommunicationWindow.push_back(std::pair<CGXDateTime, CGXDateTime>(start, end));
            }
        }
    }
    else if (e.GetIndex() == 5)
    {
        m_RandomisationStartInterval = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 6)
    {
        m_NumberOfRetries = e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 7)
    {
        m_RepetitionDelay = e.GetValue().ToInteger();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
