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
#include "../include/GXDLMSRegisterActivation.h"
#include <sstream>

//Constructor.
CGXDLMSRegisterActivation::CGXDLMSRegisterActivation() : CGXDLMSObject(OBJECT_TYPE_REGISTER_ACTIVATION)
{
}

//SN Constructor.
CGXDLMSRegisterActivation::CGXDLMSRegisterActivation(unsigned short sn) : CGXDLMSObject(OBJECT_TYPE_REGISTER_ACTIVATION, sn)
{

}

//LN Constructor.
CGXDLMSRegisterActivation::CGXDLMSRegisterActivation(std::string ln) : CGXDLMSObject(OBJECT_TYPE_REGISTER_ACTIVATION, ln)
{

}

std::vector<CGXDLMSObjectDefinition>& CGXDLMSRegisterActivation::GetRegisterAssignment()
{
    return m_RegisterAssignment;
}

std::vector<std::pair<CGXByteBuffer, CGXByteBuffer > >& CGXDLMSRegisterActivation::GetMaskList()
{
    return m_MaskList;
}

CGXByteBuffer& CGXDLMSRegisterActivation::GetActiveMask()
{
    return m_ActiveMask;
}

// Returns amount of attributes.
int CGXDLMSRegisterActivation::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSRegisterActivation::GetMethodCount()
{
    return 3;
}

void CGXDLMSRegisterActivation::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);

    std::stringstream sb;
    sb << '[';
    bool empty = true;
    for(std::vector<CGXDLMSObjectDefinition>::iterator it = m_RegisterAssignment.begin(); it != m_RegisterAssignment.end(); ++it)
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

    values.push_back("");
    /*TODO: Add MaskList.
    sb << '[';
    empty = true;
    for(std::vector<std::pair<CGXByteBuffer, CGXByteBuffer > >::iterator it = m_MaskList.begin(); it != m_MaskList.end(); ++it)
    {
    	if (!empty)
    	{
    		sb << ", ";
    	}
    	empty = false;
    	sb << it->ToString();
    }
    sb << ']';
    values.push_back(sb.str());
    */
    if (m_ActiveMask.GetSize() != 0)
    {
        values.push_back(m_ActiveMask.ToHexString());
    }
    else
    {
        values.push_back("");
    }
}

void CGXDLMSRegisterActivation::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //RegisterAssignment
    if (!IsRead(2))
    {
        attributes.push_back(2);
    }
    //MaskList
    if (!IsRead(3))
    {
        attributes.push_back(3);
    }
    //ActiveMask
    if (!IsRead(4))
    {
        attributes.push_back(4);
    }
}

int CGXDLMSRegisterActivation::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return ERROR_CODES_OK;
    }
    if (index == 2)
    {
        return CGXDLMSObject::GetDataType(index, type);
    }
    if (index == 3)
    {
        return CGXDLMSObject::GetDataType(index, type);
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}

// Returns value of given attribute.
int CGXDLMSRegisterActivation::GetValue(int index, int selector, CGXDLMSVariant& parameters, CGXDLMSVariant& value)
{
    CGXByteBuffer data;
    if (index == 1)
    {
        return GetLogicalName(this, value);
    }
    if (index == 2)
    {
    	int ret;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount(m_RegisterAssignment.size(), data);
        CGXDLMSVariant id, ln;
        for(std::vector<CGXDLMSObjectDefinition>::iterator it = m_RegisterAssignment.begin(); it != m_RegisterAssignment.end(); ++it)
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(2);
            id = it->GetClassId();
            ln = it->GetLogicalName();
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, id)) != 0 ||
            	(ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0)
			{
				return ret;
			}
        }
        value = data;
        return ERROR_CODES_OK;
    }
    if (index == 3)
    {
        //TODO: value = m_MaskList;
        return ERROR_CODES_OK;
    }
    if (index == 4)
    {
        value.Add(m_ActiveMask.GetData(), m_ActiveMask.GetSize());
        value.vt = DLMS_DATA_TYPE_OCTET_STRING;
        return ERROR_CODES_OK;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}

// Set value of given attribute.
int CGXDLMSRegisterActivation::SetValue(CGXDLMSSettings* settings, int index, CGXDLMSVariant& value)
{
    if (index == 1)
    {
        return SetLogicalName(this, value);
    }
    else if (index == 2)
    {
        m_RegisterAssignment.clear();
        if (value.vt == DLMS_DATA_TYPE_ARRAY)
        {
            for(std::vector<CGXDLMSVariant>::iterator it = value.Arr.begin(); it != value.Arr.end(); ++it)
            {
                CGXDLMSObjectDefinition item;
                item.SetClassId((OBJECT_TYPE) it->Arr[0].ToInteger());
                std::string ln;
                GXHelpers::GetLogicalName(it->Arr[1].byteArr, ln);
                item.SetLogicalName(ln);
                m_RegisterAssignment.push_back(item);
            }
        }
    }
    else if (index == 3)
    {
        m_MaskList.clear();
        if (value.vt == DLMS_DATA_TYPE_ARRAY)
        {
            for(std::vector<CGXDLMSVariant>::iterator it = value.Arr.begin(); it != value.Arr.end(); ++it)
            {
                CGXByteBuffer key, arr;
                key.AddRange(it->Arr[0].byteArr, it->Arr[0].size);
                arr.AddRange(it->Arr[1].byteArr, it->Arr[1].size);
                m_MaskList.push_back(std::pair<CGXByteBuffer, CGXByteBuffer>(key, arr));
            }
        }
    }
    else if (index == 4)
    {
        m_ActiveMask.AddRange(&value.byteArr, value.size);
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}
