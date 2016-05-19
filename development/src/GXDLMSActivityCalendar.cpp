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
#include "../include/GXDLMSActivityCalendar.h"
#include <sstream>

//Constructor.
CGXDLMSActivityCalendar::CGXDLMSActivityCalendar() : CGXDLMSObject(DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR, "0.0.13.0.0.255")
{
}

//SN Constructor.
CGXDLMSActivityCalendar::CGXDLMSActivityCalendar(unsigned short sn) : CGXDLMSObject(DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR, sn)
{

}

//LN Constructor.
CGXDLMSActivityCalendar::CGXDLMSActivityCalendar(std::string ln) : CGXDLMSObject(DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR, ln)
{

}

std::string CGXDLMSActivityCalendar::GetCalendarNameActive()
{
    return m_CalendarNameActive;
}
void CGXDLMSActivityCalendar::SetCalendarNameActive(std::string value)
{
    m_CalendarNameActive = value;
}

std::vector<CGXDLMSSeasonProfile> CGXDLMSActivityCalendar::GetSeasonProfileActive()
{
    return m_SeasonProfileActive;
}
void CGXDLMSActivityCalendar::SetSeasonProfileActive(std::vector<CGXDLMSSeasonProfile> value)
{
    m_SeasonProfileActive = value;
}

std::vector<CGXDLMSWeekProfile> CGXDLMSActivityCalendar::GetWeekProfileTableActive()
{
    return m_WeekProfileTableActive;
}
void CGXDLMSActivityCalendar::SetWeekProfileTableActive(std::vector<CGXDLMSWeekProfile> value)
{
    m_WeekProfileTableActive = value;
}

std::vector<CGXDLMSDayProfile> CGXDLMSActivityCalendar::GetDayProfileTableActive()
{
    return m_DayProfileTableActive;
}
void CGXDLMSActivityCalendar::SetDayProfileTableActive(std::vector<CGXDLMSDayProfile> value)
{
    m_DayProfileTableActive = value;
}

std::string CGXDLMSActivityCalendar::GetCalendarNamePassive()
{
    return m_CalendarNamePassive;
}
void CGXDLMSActivityCalendar::SetCalendarNamePassive(std::string value)
{
    m_CalendarNamePassive = value;
}

std::vector<CGXDLMSSeasonProfile> CGXDLMSActivityCalendar::GetSeasonProfilePassive()
{
    return m_SeasonProfilePassive;
}

void CGXDLMSActivityCalendar::SetSeasonProfilePassive(std::vector<CGXDLMSSeasonProfile> value)
{
    m_SeasonProfilePassive = value;
}

std::vector<CGXDLMSWeekProfile> CGXDLMSActivityCalendar::GetWeekProfileTablePassive()
{
    return m_WeekProfileTablePassive;
}
void CGXDLMSActivityCalendar::SetWeekProfileTablePassive(std::vector<CGXDLMSWeekProfile> value)
{
    m_WeekProfileTablePassive = value;
}

std::vector<CGXDLMSDayProfile> CGXDLMSActivityCalendar::GetDayProfileTablePassive()
{
    return m_DayProfileTablePassive;
}
void CGXDLMSActivityCalendar::SetDayProfileTablePassive(std::vector<CGXDLMSDayProfile> value)
{
    m_DayProfileTablePassive = value;
}

CGXDateTime& CGXDLMSActivityCalendar::GetTime()
{
    return m_Time;
}
void CGXDLMSActivityCalendar::SetTime(CGXDateTime& value)
{
    m_Time = value;
}

// Returns amount of attributes.
int CGXDLMSActivityCalendar::GetAttributeCount()
{
    return 10;
}

// Returns amount of methods.
int CGXDLMSActivityCalendar::GetMethodCount()
{
    return 1;
}

void CGXDLMSActivityCalendar::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    values.push_back(m_CalendarNameActive);
    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for(std::vector<CGXDLMSSeasonProfile>::iterator it = m_SeasonProfileActive.begin(); it != m_SeasonProfileActive.end(); ++it)
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

    sb << '[';
    empty = true;
    for(std::vector<CGXDLMSWeekProfile>::iterator it = m_WeekProfileTableActive.begin(); it != m_WeekProfileTableActive.end(); ++it)
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

    sb << '[';
    empty = true;
    for(std::vector<CGXDLMSDayProfile>::iterator it = m_DayProfileTableActive.begin(); it != m_DayProfileTableActive.end(); ++it)
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

    values.push_back(m_CalendarNamePassive);
    sb << '[';
    empty = true;
    for(std::vector<CGXDLMSSeasonProfile>::iterator it = m_SeasonProfilePassive.begin(); it != m_SeasonProfilePassive.end(); ++it)
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

    sb << '[';
    empty = true;
    for(std::vector<CGXDLMSWeekProfile>::iterator it = m_WeekProfileTablePassive.begin(); it != m_WeekProfileTablePassive.end(); ++it)
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

    sb << '[';
    empty = true;
    for(std::vector<CGXDLMSDayProfile>::iterator it = m_DayProfileTablePassive.begin(); it != m_DayProfileTablePassive.end(); ++it)
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
    values.push_back(m_Time.ToString());
}

void CGXDLMSActivityCalendar::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //CalendarNameActive
    if (CanRead(2))
    {
        attributes.push_back(2);
    }
    //SeasonProfileActive
    if (CanRead(3))
    {
        attributes.push_back(3);
    }

    //WeekProfileTableActive
    if (CanRead(4))
    {
        attributes.push_back(4);
    }
    //DayProfileTableActive
    if (CanRead(5))
    {
        attributes.push_back(5);
    }
    //CalendarNamePassive
    if (CanRead(6))
    {
        attributes.push_back(6);
    }
    //SeasonProfilePassive
    if (CanRead(7))
    {
        attributes.push_back(7);
    }
    //WeekProfileTablePassive
    if (CanRead(8))
    {
        attributes.push_back(8);
    }
    //DayProfileTablePassive
    if (CanRead(9))
    {
        attributes.push_back(9);
    }
    //Time.
    if (CanRead(10))
    {
        attributes.push_back(10);
    }
}

int CGXDLMSActivityCalendar::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;

    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 9)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 10)
    {
        type = DLMS_DATA_TYPE_DATETIME;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSActivityCalendar::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
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
        e.GetValue().Add(&m_CalendarNameActive[0], m_CalendarNameActive.size());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 3)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int cnt = m_SeasonProfileActive.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant tmp;
        for (std::vector<CGXDLMSSeasonProfile>::iterator it = m_SeasonProfileActive.begin(); it != m_SeasonProfileActive.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            tmp = it->GetName();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
            tmp = it->GetStart();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
            tmp.Clear();
            tmp.Add((*it).GetWeekName());
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int cnt = m_WeekProfileTableActive.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant tmp;
        for (std::vector<CGXDLMSWeekProfile>::iterator it = m_WeekProfileTableActive.begin(); it != m_WeekProfileTableActive.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            data.SetUInt8(8);
            tmp = it->GetName();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
            tmp = it->GetMonday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetTuesday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetWednesday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetThursday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetFriday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetSaturday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetSunday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int cnt = m_DayProfileTableActive.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        for (std::vector<CGXDLMSDayProfile>::iterator it = m_DayProfileTableActive.begin(); it != m_DayProfileTableActive.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            CGXDLMSVariant tmp = it->GetDayId();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            //Add count
            std::vector<CGXDLMSDayProfileAction>& schedules = (*it).GetDaySchedules();
            GXHelpers::SetObjectCount(schedules.size(), data);
            CGXDLMSVariant time, ln, selector;
            for (std::vector<CGXDLMSDayProfileAction>::iterator action = schedules.begin(); action != schedules.end(); ++action)
            {
                time = action->GetStartTime();
                ln = action->GetScriptLogicalName();
                selector = action->GetScriptSelector();
                GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, time);
                GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln);
                GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector);
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        e.GetValue().Add(m_CalendarNamePassive);
        return DLMS_ERROR_CODE_OK;
    }
    //
    if (e.GetIndex() == 7)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int cnt = m_SeasonProfilePassive.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant tmp;
        for (std::vector<CGXDLMSSeasonProfile>::iterator it = m_SeasonProfilePassive.begin(); it != m_SeasonProfilePassive.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(3);
            tmp = it->GetName();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
            tmp = it->GetStart();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
            tmp = it->GetWeekName();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;

    }
    if (e.GetIndex() == 8)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int cnt = m_WeekProfileTablePassive.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant tmp;
        for (std::vector<CGXDLMSWeekProfile>::iterator it = m_WeekProfileTablePassive.begin(); it != m_WeekProfileTablePassive.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            data.SetUInt8(8);
            tmp = it->GetName();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
            tmp = it->GetMonday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetTuesday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetWednesday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetThursday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetFriday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetSaturday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
            tmp = it->GetSunday();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, tmp);
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        int cnt = m_DayProfileTablePassive.size();
        //Add count
        GXHelpers::SetObjectCount(cnt, data);
        CGXDLMSVariant id, time, ln, selector;
        for (std::vector<CGXDLMSDayProfile>::iterator it = m_DayProfileTablePassive.begin(); it != m_DayProfileTablePassive.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            id = it->GetDayId();
            GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, id);
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            //Add count
            GXHelpers::SetObjectCount((*it).GetDaySchedules().size(), data);
            for (std::vector<CGXDLMSDayProfileAction>::iterator action = (*it).GetDaySchedules().begin();
                    action != (*it).GetDaySchedules().end(); ++action)
            {
                time = action->GetStartTime();
                ln = action->GetScriptLogicalName();
                selector = action->GetScriptSelector();
                GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, time);
                GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln);
                GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, selector);
            }
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 10)
    {
        e.SetValue(GetTime());
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSActivityCalendar::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetCalendarNameActive(tmp.strVal);
    }
    else if (e.GetIndex() == 3)
    {
        m_SeasonProfileActive.clear();
        for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSSeasonProfile it;
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType((*item).Arr[0], DLMS_DATA_TYPE_STRING, tmp);
            it.SetName(tmp.strVal);
            CGXDLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
            it.SetStart(tmp.dateTime);
            CGXDLMSClient::ChangeType((*item).Arr[2], DLMS_DATA_TYPE_STRING, tmp);
            it.SetWeekName(tmp.strVal);
            m_SeasonProfileActive.push_back(it);
        }
    }
    else if (e.GetIndex() == 4)
    {
        m_WeekProfileTableActive.clear();
        for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSVariant tmp;
            CGXDLMSWeekProfile it;
            CGXDLMSClient::ChangeType((*item).Arr[0], DLMS_DATA_TYPE_STRING, tmp);
            it.SetName(tmp.strVal);
            it.SetMonday((*item).Arr[1].lVal);
            it.SetTuesday((*item).Arr[1].lVal);
            it.SetWednesday((*item).Arr[1].lVal);
            it.SetThursday((*item).Arr[1].lVal);
            it.SetFriday((*item).Arr[1].lVal);
            it.SetSaturday((*item).Arr[1].lVal);
            it.SetSunday((*item).Arr[1].lVal);
            m_WeekProfileTableActive.push_back(it);
        }
    }
    else if (e.GetIndex() == 5)
    {
        m_DayProfileTableActive.clear();
        for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSDayProfile it;
            it.SetDayId(item->Arr[0].ToInteger());
            for(std::vector<CGXDLMSVariant>::iterator it2 = (*item).Arr[1].Arr.begin(); it2 != (*item).Arr[1].Arr.end(); ++it2)
            {
                CGXDLMSDayProfileAction ac;
                CGXDLMSVariant tmp;
                CGXDLMSClient::ChangeType(it2->Arr[0], DLMS_DATA_TYPE_TIME, tmp);
                ac.SetStartTime(tmp.dateTime);
                CGXDLMSClient::ChangeType(it2->Arr[1], DLMS_DATA_TYPE_STRING, tmp);
                ac.SetScriptLogicalName(tmp.strVal);
                ac.SetScriptSelector(it2->Arr[2].ToInteger());
                it.GetDaySchedules().push_back(ac);
            }
            m_DayProfileTableActive.push_back(it);
        }
    }
    else if (e.GetIndex() == 6)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_STRING, tmp);
        SetCalendarNamePassive(tmp.strVal);
    }
    else if (e.GetIndex() == 7)
    {
        for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSSeasonProfile it;
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType((*item).Arr[0], DLMS_DATA_TYPE_STRING, tmp);
            it.SetName(tmp.strVal);
            CGXDLMSClient::ChangeType((*item).Arr[1], DLMS_DATA_TYPE_DATETIME, tmp);
            it.SetStart(tmp.dateTime);
            CGXDLMSClient::ChangeType((*item).Arr[2], DLMS_DATA_TYPE_STRING, tmp);
            it.SetWeekName(tmp.strVal);
            m_SeasonProfilePassive.push_back(it);
        }
    }
    else if (e.GetIndex() == 8)
    {
        for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSWeekProfile it;
            CGXDLMSVariant tmp;
            CGXDLMSClient::ChangeType((*item).Arr[0], DLMS_DATA_TYPE_STRING, tmp);
            it.SetName(tmp.strVal);
            it.SetMonday((*item).Arr[1].lVal);
            it.SetTuesday((*item).Arr[2].lVal);
            it.SetWednesday((*item).Arr[3].lVal);
            it.SetThursday((*item).Arr[4].lVal);
            it.SetFriday((*item).Arr[5].lVal);
            it.SetSaturday((*item).Arr[6].lVal);
            it.SetSunday((*item).Arr[7].lVal);
            m_WeekProfileTablePassive.push_back(it);
        }
    }
    else if (e.GetIndex() == 9)
    {
        for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
        {
            CGXDLMSDayProfile it;
            it.SetDayId((*item).Arr[0].iVal);
            for(std::vector<CGXDLMSVariant>::iterator it2 = (*item).Arr[1].Arr.begin(); it2 != (*item).Arr[1].Arr.end(); ++it2)
            {
                CGXDLMSDayProfileAction ac;
                CGXDLMSVariant tmp;
                CGXDLMSClient::ChangeType((*it2).Arr[0], DLMS_DATA_TYPE_TIME, tmp);
                ac.SetStartTime(tmp.dateTime);
                CGXDLMSClient::ChangeType((*it2).Arr[1], DLMS_DATA_TYPE_STRING, tmp);
                ac.SetScriptLogicalName(tmp.strVal);
                ac.SetScriptSelector((*it2).Arr[2].lVal);
                it.GetDaySchedules().push_back(ac);
            }
            m_DayProfileTablePassive.push_back(it);
        }
    }
    else if (e.GetIndex() == 10)
    {
        CGXDLMSVariant tmp;
        CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_DATETIME, tmp);
        SetTime(tmp.dateTime);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
