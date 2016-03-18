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

#include "../include/GXDLMSScriptTable.h"
#include <sstream>

//Constructor.
CGXDLMSScriptTable::CGXDLMSScriptTable() : CGXDLMSObject(OBJECT_TYPE_SCRIPT_TABLE)
{
}

//SN Constructor.
CGXDLMSScriptTable::CGXDLMSScriptTable(unsigned short sn) : CGXDLMSObject(OBJECT_TYPE_SCRIPT_TABLE, sn)
{

}

//LN Constructor.
CGXDLMSScriptTable::CGXDLMSScriptTable(std::string ln) : CGXDLMSObject(OBJECT_TYPE_SCRIPT_TABLE, ln)
{

}

std::vector<std::pair<int, CGXDLMSScriptAction> >& CGXDLMSScriptTable::GetScripts()
{
    return m_Scripts;
}

// Returns amount of attributes.
int CGXDLMSScriptTable::GetAttributeCount()
{
    return 2;
}

// Returns amount of methods.
int CGXDLMSScriptTable::GetMethodCount()
{
    return 1;
}

void CGXDLMSScriptTable::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for(std::vector<std::pair<int, CGXDLMSScriptAction> >::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
    {
        if (!empty)
        {
            sb << ", ";
        }
        empty = false;
        sb << it->first;
        sb << " ";
        std::string str = it->second.ToString();
        sb.write(str.c_str(), str.size());
    }
    sb << ']';
    values.push_back(sb.str());
}

void CGXDLMSScriptTable::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //Scripts
    if (CanRead(2))
    {
        attributes.push_back(2);
    }
}

int CGXDLMSScriptTable::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return ERROR_CODES_OK;
    }
    //Scripts
    if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSScriptTable::GetValue(int index, int selector, CGXDLMSVariant& parameters, CGXDLMSVariant& value)
{
    if (index == 1)
    {
        return GetLogicalName(this, value);
    }
    if (index == 2)
    {
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount(m_Scripts.size(), data);
        int ret;
        CGXDLMSVariant id, type, oType, ln, index;
        for(std::vector<std::pair<int, CGXDLMSScriptAction> >::iterator it = m_Scripts.begin(); it != m_Scripts.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2); //Count
        	//Script_identifier:
            id = it->first;
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, id)) != 0)
            {
                return ret;
            }
            data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
            data.SetUInt8(5); //Count
            CGXDLMSScriptAction tmp = it->second;
            type = tmp.GetType();
            oType = tmp.GetObjectType();
            ln = tmp.GetLogicalName();
            index = tmp.GetIndex();
            CGXDLMSVariant param = tmp.GetParameter();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, type)) != 0 || //service_id
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, oType)) != 0 || //class_id
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0 || //logical_name
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != 0 || //index
                    (ret = GXHelpers::SetData(data, tmp.GetParameter().vt, param)) != 0) //parameter
            {
                return ret;
            }
        }
        value = data;
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSScriptTable::SetValue(CGXDLMSSettings* settings, int index, CGXDLMSVariant& value)
{
    if (index == 1)
    {
        return SetLogicalName(this, value);
    }
    else if (index == 2)
    {
        m_Scripts.clear();
        //Fix Xemex bug here.
        //Xemex meters do not return array as they shoul be according standard.
        if (value.Arr.size()  != 0)
        {
            if (value.Arr[0].vt == DLMS_DATA_TYPE_STRUCTURE)
            {
                std::string ln;
                for(std::vector<CGXDLMSVariant>::iterator item = value.Arr.begin(); item != value.Arr.end(); ++item)
                {
                    int script_identifier = (*item).Arr[0].ToInteger();
                    for(std::vector<CGXDLMSVariant>::iterator arr = (*item).Arr[1].Arr.begin(); arr != (*item).Arr[1].Arr.end(); ++arr)
                    {
                        CGXDLMSScriptAction it;
                        SCRIPT_ACTION_TYPE type = (SCRIPT_ACTION_TYPE) (*arr).Arr[0].ToInteger();
                        it.SetType(type);
                        OBJECT_TYPE ot = (OBJECT_TYPE) (*arr).Arr[1].ToInteger();
                        it.SetObjectType(ot);
                        ln.clear();
                        GXHelpers::GetLogicalName((*arr).Arr[2].byteArr, ln);
                        it.SetLogicalName(ln);
                        it.SetIndex((*arr).Arr[3].ToInteger());
                        it.SetParameter((*arr).Arr[4]);
                        m_Scripts.push_back(std::pair<int, CGXDLMSScriptAction>(script_identifier, it));
                    }
                }
            }
            else //Read Xemex meter here.
            {
                int script_identifier = value.Arr[0].ToInteger();
                CGXDLMSScriptAction it;
                SCRIPT_ACTION_TYPE type = (SCRIPT_ACTION_TYPE) value.Arr[1].Arr[0].ToInteger();
                it.SetType(type);
                OBJECT_TYPE ot = (OBJECT_TYPE) value.Arr[1].Arr[1].ToInteger();
                it.SetObjectType(ot);
                std::string ln;
                GXHelpers::GetLogicalName(value.Arr[1].Arr[2].byteArr, ln);
                it.SetLogicalName(ln);
                it.SetIndex(value.Arr[1].Arr[3].ToInteger());
                it.SetParameter(value.Arr[1].Arr[4]);
                m_Scripts.push_back(std::pair<int, CGXDLMSScriptAction>(script_identifier, it));
            }
        }
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}
