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
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSAssociationLogicalName.h"


void CGXDLMSAssociationLogicalName::Init()
{
    m_AssociationStatus = DLMS_DLMS_ASSOCIATION_STATUS_NON_ASSOCIATED;
}

void CGXDLMSAssociationLogicalName::UpdateAccessRights(CGXDLMSObject* pObj, CGXDLMSVariant data)
{
    for (std::vector<CGXDLMSVariant >::iterator it = data.Arr[0].Arr.begin(); it != data.Arr[0].Arr.end(); ++it)
    {
        int id = it->Arr[0].ToInteger();
        DLMS_ACCESS_MODE mode = (DLMS_ACCESS_MODE) it->Arr[1].ToInteger();
        pObj->SetAccess(id, mode);
    }
    for (std::vector<CGXDLMSVariant >::iterator it = data.Arr[1].Arr.begin(); it != data.Arr[1].Arr.end(); ++it)
    {
        int id = it->Arr[0].ToInteger();
        DLMS_METHOD_ACCESS_MODE mode = (DLMS_METHOD_ACCESS_MODE) it->Arr[1].ToInteger();
        pObj->SetMethodAccess(id, mode);
    }
}

int CGXDLMSAssociationLogicalName::GetAccessRights(CGXDLMSObject* pItem, CGXByteBuffer& data)
{
    int ret;
    CGXAttributeCollection& attributes = pItem->GetAttributes();
    data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    data.SetUInt8(2);
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    data.SetUInt8(attributes.size());
    CGXDLMSVariant index, access, empty;
    for(CGXAttributeCollection::iterator it = attributes.begin(); it != attributes.end(); ++it)
    {
        index = (*it).GetIndex();
        access = (*it).GetAccess();
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE); //attribute_access_item
        data.SetUInt8(3);
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, access)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_NONE, empty)) != 0)
        {
            return ret;
        }
    }

    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    CGXAttributeCollection& m = pItem->GetMethodAttributes();
    data.SetUInt8(m.size());
    for(CGXAttributeCollection::iterator it = m.begin(); it != m.end(); ++it)
    {
        CGXDLMSVariant index = (*it).GetIndex();
        CGXDLMSVariant access = (*it).GetMethodAccess();
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE); //attribute_access_item
        data.SetUInt8(2);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, access);
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns LN Association View.
int CGXDLMSAssociationLogicalName::GetObjects(CGXByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    //Add count
    GXHelpers::SetObjectCount(m_ObjectList.size(), data);
    for(CGXDLMSObjectCollection::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(4);//Count
        CGXDLMSVariant type = (*it)->GetObjectType();
        CGXDLMSVariant version = (*it)->GetVersion();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, type);//ClassID
        GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, version);//Version
        CGXDLMSVariant ln((*it)->m_LN, 6, DLMS_DATA_TYPE_OCTET_STRING);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln);//LN
        GetAccessRights(*it, data);//Access rights.
    }
    return DLMS_ERROR_CODE_OK;
}

/**
 Constructor.
*/
CGXDLMSAssociationLogicalName::CGXDLMSAssociationLogicalName() : CGXDLMSObject(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, "0.0.40.0.0.255")
{
    Init();
}

/**
 Constructor.
 @param ln Logical Name of the object.
*/
CGXDLMSAssociationLogicalName::CGXDLMSAssociationLogicalName(std::string ln) : CGXDLMSObject(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, ln)
{
    Init();
}

CGXDLMSObjectCollection& CGXDLMSAssociationLogicalName::GetObjectList()
{
    return m_ObjectList;
}

unsigned char CGXDLMSAssociationLogicalName::GetClientSAP()
{
    return m_ClientSAP;
}
void CGXDLMSAssociationLogicalName::SetClientSAP(unsigned char value)
{
    m_ClientSAP = value;
}

unsigned short CGXDLMSAssociationLogicalName::GetServerSAP()
{
    return m_ServerSAP;
}

void CGXDLMSAssociationLogicalName::SetServerSAP(unsigned short value)
{
    m_ServerSAP = value;
}

CGXApplicationContextName CGXDLMSAssociationLogicalName::GetApplicationContextName()
{
    return m_ApplicationContextName;
}

CGXDLMSContextType CGXDLMSAssociationLogicalName::GetXDLMSContextInfo()
{
    return m_XDLMSContextInfo;
}

CGXAuthenticationMechanismName CGXDLMSAssociationLogicalName::GetAuthenticationMechanismMame()
{
    return m_AuthenticationMechanismName;
}

CGXByteBuffer& CGXDLMSAssociationLogicalName::GetSecret()
{
    return m_Secret;
}
void CGXDLMSAssociationLogicalName::SetSecret(CGXByteBuffer& value)
{
    m_Secret = value;
}

DLMS_DLMS_ASSOCIATION_STATUS CGXDLMSAssociationLogicalName::GetAssociationStatus()
{
    return m_AssociationStatus;
}

void CGXDLMSAssociationLogicalName::SetAssociationStatus(DLMS_DLMS_ASSOCIATION_STATUS value)
{
    m_AssociationStatus = value;
}

std::string CGXDLMSAssociationLogicalName::GetSecuritySetupReference()
{
    return m_SecuritySetupReference;
}
void CGXDLMSAssociationLogicalName::SetSecuritySetupReference(std::string value)
{
    m_SecuritySetupReference = value;
}

// Returns amount of attributes.
int CGXDLMSAssociationLogicalName::GetAttributeCount()
{
    //Security Setup Reference is from version 1.
    if (GetVersion() > 0)
        return 9;
    return 8;
}

// Returns amount of methods.
int CGXDLMSAssociationLogicalName::GetMethodCount()
{
    return 4;
}

void CGXDLMSAssociationLogicalName::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ObjectList.ToString());
    std::string str = CGXDLMSVariant(m_ClientSAP).ToString();
    str += "/";
    str += CGXDLMSVariant(m_ServerSAP).ToString();
    values.push_back(str);
    values.push_back(m_ApplicationContextName.ToString());
    values.push_back(m_XDLMSContextInfo.ToString());
    values.push_back(m_AuthenticationMechanismName.ToString());
    values.push_back(m_Secret.ToHexString());
    values.push_back(CGXDLMSVariant(m_AssociationStatus).ToString());
    //Security Setup Reference is from version 1.
    if (GetVersion() > 0)
    {
        values.push_back(m_SecuritySetupReference);
    }
}

void CGXDLMSAssociationLogicalName::GetAttributeIndexToRead(std::vector<int>& attributes)
{
    //LN is static and read only once.
    if (CGXDLMSObject::IsLogicalNameEmpty(m_LN))
    {
        attributes.push_back(1);
    }
    //ObjectList is static and read only once.
    if (!IsRead(2))
    {
        attributes.push_back(2);
    }
    //associated_partners_id is static and read only once.
    if (!IsRead(3))
    {
        attributes.push_back(3);
    }
    //Application Context Name is static and read only once.
    if (!IsRead(4))
    {
        attributes.push_back(4);
    }
    //xDLMS Context Info
    if (!IsRead(5))
    {
        attributes.push_back(5);
    }
    // Authentication Mechanism Name
    if (!IsRead(6))
    {
        attributes.push_back(6);
    }
    // Secret
    if (!IsRead(7))
    {
        attributes.push_back(7);
    }
    // Association Status
    if (!IsRead(8))
    {
        attributes.push_back(8);
    }
    //Security Setup Reference is from version 1.
    if (GetVersion() > 0 && !IsRead(9))
    {
        attributes.push_back(9);
    }
}

int CGXDLMSAssociationLogicalName::GetDataType(int index, DLMS_DATA_TYPE& type)
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
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 4)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 5)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 6)
    {
        type = DLMS_DATA_TYPE_STRUCTURE;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 7)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 8)
    {
        type = DLMS_DATA_TYPE_ENUM;
        return DLMS_ERROR_CODE_OK;
    }
    if (index == 9)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSAssociationLogicalName::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    int ret;
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
        CGXByteBuffer Packets;
        ret = GetObjects(Packets);
        e.SetValue(Packets);
        return ret;
    }
    if (e.GetIndex() == 3)
    {
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        data.SetUInt8(2);
        data.SetUInt8(DLMS_DATA_TYPE_UINT8);
        data.SetUInt8(m_ClientSAP);
        data.SetUInt8(DLMS_DATA_TYPE_UINT16);
        data.SetUInt16(m_ServerSAP);
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 4)
    {
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        //Add count
        data.SetUInt8(0x7);
        CGXDLMSVariant ctt = m_ApplicationContextName.GetJointIsoCtt();
        CGXDLMSVariant country = m_ApplicationContextName.GetCountry();
        CGXDLMSVariant name = m_ApplicationContextName.GetCountryName();
        CGXDLMSVariant organization = m_ApplicationContextName.GetIdentifiedOrganization();
        CGXDLMSVariant ua = m_ApplicationContextName.GetDlmsUA();
        CGXDLMSVariant context = m_ApplicationContextName.GetApplicationContext();
        CGXDLMSVariant id = m_ApplicationContextName.GetContextId();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ctt)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, country)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, name)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, organization)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ua)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, context)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, id)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 5)
    {
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        data.SetUInt8(6);
        CGXDLMSVariant conformance = m_XDLMSContextInfo.GetConformance();
        CGXDLMSVariant rx = m_XDLMSContextInfo.GetMaxReceivePduSize();
        CGXDLMSVariant tx = m_XDLMSContextInfo.GetMaxSendPpuSize();
        CGXDLMSVariant version = m_XDLMSContextInfo.GetDlmsVersionNumber();
        CGXDLMSVariant quality = m_XDLMSContextInfo.GetQualityOfService();
        CGXDLMSVariant info = m_XDLMSContextInfo.GetCypheringInfo();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_BIT_STRING, conformance)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, rx)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, tx)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, version)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, quality)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, info)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 6)
    {
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
        //Add count
        data.SetUInt8(0x7);
        CGXDLMSVariant ctt = m_AuthenticationMechanismName.GetJointIsoCtt();
        CGXDLMSVariant country = m_AuthenticationMechanismName.GetCountry();
        CGXDLMSVariant name = m_AuthenticationMechanismName.GetCountryName();
        CGXDLMSVariant organization = m_AuthenticationMechanismName.GetIdentifiedOrganization();
        CGXDLMSVariant ua = m_AuthenticationMechanismName.GetDlmsUA();
        CGXDLMSVariant context = m_AuthenticationMechanismName.GetAuthenticationMechanismName();
        CGXDLMSVariant id = m_AuthenticationMechanismName.GetMechanismId();
        if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ctt)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, country)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, name)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, organization)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, ua)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, context)) != 0 ||
                (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, id)) != 0)
        {
            return ret;
        }
        e.SetValue(data);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 7)
    {
        e.SetValue(GetSecret());
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 8)
    {
        e.SetValue((unsigned char)m_AssociationStatus);
        return DLMS_ERROR_CODE_OK;
    }
    if (e.GetIndex() == 9)
    {
        e.SetValue(m_SecuritySetupReference);
        return DLMS_ERROR_CODE_OK;
    }
    return DLMS_ERROR_CODE_INVALID_PARAMETER;
}

int CGXDLMSAssociationLogicalName::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArgs& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ObjectList.clear();
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            for (std::vector<CGXDLMSVariant >::iterator it = e.GetValue().Arr.begin(); it != e.GetValue().Arr.end(); ++it)
            {
                DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE) (*it).Arr[0].ToInteger();
                int version = (*it).Arr[1].ToInteger();
                std::string ln;
                GXHelpers::GetLogicalName((*it).Arr[2].byteArr, ln);
                CGXDLMSObject* pObj = settings.GetObjects().FindByLN(type, ln);
                if (pObj == NULL)
                {
                    pObj = CGXDLMSObjectFactory::CreateObject(type, ln);
                    pObj->SetVersion(version);
                }
                UpdateAccessRights(pObj, (*it).Arr[3]);
                m_ObjectList.push_back(pObj);
            }
        }
    }
    else if (e.GetIndex() == 3)
    {
        m_ClientSAP = e.GetValue().Arr[0].ToInteger();
        m_ServerSAP = e.GetValue().Arr[1].ToInteger();
    }
    else if (e.GetIndex() == 4)
    {
        //Value of the object identifier encoded in BER
        if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            CGXByteBuffer tmp;
            tmp.Set(e.GetValue().byteArr, e.GetValue().GetSize());
            int ret;
            unsigned char val;
            if ((ret = tmp.GetUInt8(0, &val)) != 0)
            {
                return ret;
            }
            if (val == 0x60)
            {
                m_ApplicationContextName.SetJointIsoCtt(0);
                m_ApplicationContextName.SetCountry(0);
                m_ApplicationContextName.SetCountryName(0);
                tmp.SetPosition(3);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetIdentifiedOrganization(val);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetDlmsUA(val);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetApplicationContext(val);
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetContextId(val);
            }
            else
            {
                //Get Tag and Len.
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 2)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 7)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetJointIsoCtt(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetCountry(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x12)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetCountryName(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetIdentifiedOrganization(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetDlmsUA(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetApplicationContext(val);
                //Get tag
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                if (val != 0x11)
                {
                    return DLMS_ERROR_CODE_INVALID_PARAMETER;
                }
                if ((ret = tmp.GetUInt8(&val)) != 0)
                {
                    return ret;
                }
                m_ApplicationContextName.SetContextId(val);
            }
        }
        else
        {
            m_ApplicationContextName.SetJointIsoCtt(e.GetValue().Arr[0].ToInteger());
            m_ApplicationContextName.SetCountry(e.GetValue().Arr[1].ToInteger());
            m_ApplicationContextName.SetCountryName(e.GetValue().Arr[2].ToInteger());
            m_ApplicationContextName.SetIdentifiedOrganization(e.GetValue().Arr[3].ToInteger());
            m_ApplicationContextName.SetDlmsUA(e.GetValue().Arr[4].ToInteger());
            m_ApplicationContextName.SetApplicationContext(e.GetValue().Arr[5].ToInteger());
            m_ApplicationContextName.SetContextId(e.GetValue().Arr[6].ToInteger());
        }
    }
    else if (e.GetIndex() == 5)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRUCTURE)
        {
            m_XDLMSContextInfo.SetConformance(e.GetValue().Arr[0].ToString());
            m_XDLMSContextInfo.SetMaxReceivePduSize(e.GetValue().Arr[1].ToInteger());
            m_XDLMSContextInfo.SetMaxSendPpuSize(e.GetValue().Arr[2].ToInteger());
            m_XDLMSContextInfo.SetDlmsVersionNumber(e.GetValue().Arr[3].ToInteger());
            m_XDLMSContextInfo.SetQualityOfService(e.GetValue().Arr[4].ToInteger());
            CGXByteBuffer tmp;
            tmp.Set(e.GetValue().Arr[5].byteArr, e.GetValue().Arr[5].GetSize());
            m_XDLMSContextInfo.SetCypheringInfo(tmp);
        }
    }
    else if (e.GetIndex() == 6)
    {
        if (e.GetValue().vt != DLMS_DATA_TYPE_NONE)
        {
            unsigned char val;
            int ret;
            //Value of the object identifier encoded in BER
            if (e.GetValue().vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                CGXByteBuffer tmp;
                tmp.Set(e.GetValue().byteArr, e.GetValue().GetSize());
                if ((ret = tmp.GetUInt8(0, &val)) != 0)
                {
                    return ret;
                }
                if (val == 0x60)
                {
                    m_AuthenticationMechanismName.SetJointIsoCtt(0);
                    m_AuthenticationMechanismName.SetCountry(0);
                    m_AuthenticationMechanismName.SetCountryName(0);
                    tmp.SetPosition(3);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetIdentifiedOrganization(val);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetDlmsUA(val);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetAuthenticationMechanismName(val);
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetMechanismId((DLMS_AUTHENTICATION) val);
                }
                else
                {
                    //Get Tag and Len.
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 2)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 7)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetJointIsoCtt(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetCountry(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x12)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    unsigned short tmp2;
                    if ((ret = tmp.GetUInt16(&tmp2)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetCountryName(tmp2);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetIdentifiedOrganization(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetDlmsUA(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetAuthenticationMechanismName(val);
                    //Get tag
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    if (val != 0x11)
                    {
                        return DLMS_ERROR_CODE_INVALID_PARAMETER;
                    }
                    if ((ret = tmp.GetUInt8(&val)) != 0)
                    {
                        return ret;
                    }
                    m_AuthenticationMechanismName.SetMechanismId((DLMS_AUTHENTICATION) val);
                }
            }
            else
            {
                m_AuthenticationMechanismName.SetJointIsoCtt(e.GetValue().Arr[0].ToInteger());
                m_AuthenticationMechanismName.SetCountry(e.GetValue().Arr[1].ToInteger());
                m_AuthenticationMechanismName.SetCountryName(e.GetValue().Arr[2].ToInteger());
                m_AuthenticationMechanismName.SetIdentifiedOrganization(e.GetValue().Arr[3].ToInteger());
                m_AuthenticationMechanismName.SetDlmsUA(e.GetValue().Arr[4].ToInteger());
                m_AuthenticationMechanismName.SetAuthenticationMechanismName(e.GetValue().Arr[5].ToInteger());
                m_AuthenticationMechanismName.SetMechanismId((DLMS_AUTHENTICATION) e.GetValue().Arr[6].ToInteger());
            }
        }
    }
    else if (e.GetIndex() == 7)
    {
        m_Secret.Clear();
        m_Secret.Set(e.GetValue().byteArr, e.GetValue().size);
    }
    else if (e.GetIndex() == 8)
    {
        m_AssociationStatus = (DLMS_DLMS_ASSOCIATION_STATUS) e.GetValue().ToInteger();
    }
    else if (e.GetIndex() == 9)
    {
        m_SecuritySetupReference = e.GetValue().ToString();
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
