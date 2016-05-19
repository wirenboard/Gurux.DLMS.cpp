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

#include "../include/GXDLMSVariant.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSActionSchedule.h"
#include <sstream>

void CGXDLMSActionSchedule::Init()
{
    m_ExecutedScriptSelector = 0;
    m_Type = SINGLE_ACTION_SCHEDULE_TYPE1;
}

/**
 Constructor.
*/
CGXDLMSActionSchedule::CGXDLMSActionSchedule() : CGXDLMSObject(DLMS_OBJECT_TYPE_ACTION_SCHEDULE)
{
    Init();
}

/**
 Constructor.

 @param ln Logical Name of the object.
*/
CGXDLMSActionSchedule::CGXDLMSActionSchedule(std::string ln) : CGXDLMSObject(DLMS_OBJECT_TYPE_ACTION_SCHEDULE, ln)
{
    Init();
}

/**
 Constructor.

 @param ln Logical Name of the object.
 @param sn Short Name of the object.
*/
CGXDLMSActionSchedule::CGXDLMSActionSchedule(int sn) : CGXDLMSObject(DLMS_OBJECT_TYPE_ACTION_SCHEDULE, sn)
{
    Init();
}

std::string CGXDLMSActionSchedule::GetExecutedScriptLogicalName()
{
    return m_ExecutedScriptLogicalName;
}
void CGXDLMSActionSchedule::SetExecutedScriptLogicalName(std::string value)
{
    m_ExecutedScriptLogicalName = value;
}

int CGXDLMSActionSchedule::GetExecutedScriptSelector()
{
    return m_ExecutedScriptSelector;
}
void CGXDLMSActionSchedule::SetExecutedScriptSelector(int value)
{
    m_ExecutedScriptSelector = value;
}

SINGLE_ACTION_SCHEDULE_TYPE CGXDLMSActionSchedule::GetType()
{
    return m_Type;
}
void CGXDLMSActionSchedule::SetType(SINGLE_ACTION_SCHEDULE_TYPE value)
{
    m_Type = value;
}

std::vector<CGXDateTime> CGXDLMSActionSchedule::GetExecutionTime()
{
    return m_ExecutionTime;
}
void CGXDLMSActionSchedule::SetExecutionTime(std::vector<CGXDateTime> value)
{
    m_ExecutionTime = value;
}

// Returns amount of attributes.
int CGXDLMSActionSchedule::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSActionSchedule::GetMethodCount()
{
    return 0;
}

void CGXDLMSActionSchedule::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ExecutedScriptLogicalName + " " + CGXDLMSVariant(m_ExecutedScriptSelector).ToString());
    values.push_back(CGXDLMSVariant(m_Type).ToString());
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for(std::vector<CGXDateTime>::iterator it = m_ExecutionTime.begin(); it != m_ExecutionTime.end(); ++it)
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
}

void CGXDLMSActionSchedule::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ExecutedScriptLogicalName is static and read only once.
    if (!IsRead(2))
    {
        attributes.push_back(2);
    }
    //Type is static and read only once.
    if (!IsRead(3))
    {
        attributes.push_back(3);
    }
    //ExecutionTime is static and read only once.
    if (!IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSActionSchedule::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;

}

// Returns value of given attribute.
int CGXDLMSActionSchedule::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    CGXByteBuffer data;
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
        int ret;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(2);
        CGXDLMSVariant ln = m_ExecutedScriptLogicalName;
        CGXDLMSVariant ss = m_ExecutedScriptSelector;
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, ss)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        e.SetValue(GetType());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        int ret;
        CGXByteBuffer bb;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount(GetExecutionTime().size(), bb);
        CGXDLMSVariant val;
        for (std::vector<CGXDateTime>::iterator it = m_ExecutionTime.begin(); it != m_ExecutionTime.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2); //Count
            val = *it;
            if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_TIME, val)) != 0 || //Time
                    (ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_DATE, val)) != 0) //Date
            {
                return ret;
            }
        }
        e.SetValue(bb);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSActionSchedule::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        SetExecutedScriptLogicalName(e.GetValue().Arr[0].ToString());
        SetExecutedScriptSelector(e.GetValue().Arr[1].ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 3)
    {
        SetType((SINGLE_ACTION_SCHEDULE_TYPE) e.GetValue().ToInteger());
        return DLMS_ERROR_CODE_OK;
    }
    else if (e.GetIndex() == 4)
    {
        m_ExecutionTime.clear();
        for (std::vector<CGXDLMSVariant>::iterator it = e.GetValue().Arr.begin();
                it != e.GetValue().Arr.end(); ++it)
        {
            CGXDLMSVariant time, date;
            CGXDLMSClient::ChangeType((*it).Arr[0], DLMS_DATA_TYPE_TIME, time);
            CGXDLMSClient::ChangeType((*it).Arr[1], DLMS_DATA_TYPE_DATE, date);
            struct tm val = time.dateTime.GetValue();
            struct tm val2 = date.dateTime.GetValue();
            val2.tm_hour = val.tm_hour;
            val2.tm_min = val.tm_min;
            val2.tm_sec = val.tm_sec;
            date.dateTime.SetValue(val2);
            date.dateTime.SetSkip((DATETIME_SKIPS) (time.dateTime.GetSkip() | date.dateTime.GetSkip()));
            m_ExecutionTime.push_back(date.dateTime);
        }
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}
