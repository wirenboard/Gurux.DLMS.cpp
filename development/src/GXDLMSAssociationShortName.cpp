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
#include "../include/GXDLMSAssociationShortName.h"
#include "../include/GXDLMSObjectFactory.h"

int CGXDLMSAssociationShortName::GetAccessRights(CGXDLMSObject* pObj, CGXByteBuffer& data)
{
    data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    data.SetUInt8(3);
    CGXDLMSVariant ln = pObj->GetShortName();
    GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, ln);
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    data.SetUInt8(pObj->GetAttributes().size());
    CGXDLMSVariant empty;
    CGXDLMSVariant index, access;
    for(std::vector<CGXDLMSAttribute>::iterator att = pObj->GetAttributes().begin(); att != pObj->GetAttributes().end(); ++att)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE); //attribute_access_item
        data.SetUInt8(3);
        index = att->GetIndex();
        access = att->GetAccess();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, access);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_NONE, empty);
    }
    data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
    data.SetUInt8(pObj->GetMethodAttributes().size());
    for(std::vector<CGXDLMSAttribute>::iterator it = pObj->GetMethodAttributes().begin(); it != pObj->GetMethodAttributes().end(); ++it)
    {
        data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE); //attribute_access_item
        data.SetUInt8(2);
        index = it->GetIndex();
        access = it->GetAccess();
        GXHelpers::SetData(data, DLMS_DATA_TYPE_INT8, index);
        GXHelpers::SetData(data, DLMS_DATA_TYPE_ENUM, access);
    }
    return DLMS_ERROR_CODE_OK;
}

void CGXDLMSAssociationShortName::UpdateAccessRights(CGXDLMSVariant& buff)
{
    for(std::vector<CGXDLMSVariant>::iterator access = buff.Arr.begin(); access != buff.Arr.end(); ++access)
    {
        int sn = access->Arr[0].ToInteger();
        CGXDLMSObject* pObj = m_ObjectList.FindBySN(sn);
        if (pObj != NULL)
        {
            for(std::vector<CGXDLMSVariant>::iterator attributeAccess = access->Arr[1].Arr.begin();
                    attributeAccess != access->Arr[1].Arr.end(); ++attributeAccess)
            {
                int id = attributeAccess->Arr[0].ToInteger();
                int tmp = attributeAccess->Arr[1].ToInteger();
                pObj->SetAccess(id, (DLMS_ACCESS_MODE) tmp);
            }
            for(std::vector<CGXDLMSVariant>::iterator methodAccess = access->Arr[2].Arr.begin();
                    methodAccess != access->Arr[2].Arr.end(); ++methodAccess)
            {
                int id = methodAccess->Arr[0].ToInteger();
                int tmp = methodAccess->Arr[1].ToInteger();
                pObj->SetMethodAccess(id, (DLMS_METHOD_ACCESS_MODE) tmp);
            }
        }
    }
}

//Constructor.
CGXDLMSAssociationShortName::CGXDLMSAssociationShortName() : CGXDLMSObject(DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME)
{
    GXHelpers::SetLogicalName("0.0.40.0.0.255", m_LN);
    m_Secret.AddString("Gurux");
    m_SN = 0xFA00;
}

CGXDLMSObjectCollection& CGXDLMSAssociationShortName::GetObjectList()
{
    return m_ObjectList;
}

CGXByteBuffer& CGXDLMSAssociationShortName::GetSecret()
{
    return m_Secret;
}
void CGXDLMSAssociationShortName::SetSecret(CGXByteBuffer& value)
{
    m_Secret = value;
}

/* TODO:
Object GetAccessRightsList()
{
    return m_AccessRightsList;
}
void SetAccessRightsList(Object value)
{
    m_AccessRightsList = value;
}

Object GetSecuritySetupReference()
{
    return m_SecuritySetupReference;
}
void SetSecuritySetupReference(Object value)
{
    m_SecuritySetupReference = value;
}
*/

void CGXDLMSAssociationShortName::GetValues(std::vector<std::string>& values)
{
    values.clear();
    std::string ln;
    GetLogicalName(ln);
    values.push_back(ln);
    values.push_back(m_ObjectList.ToString());
    values.push_back(m_AccessRightsList.ToString());
    values.push_back(m_SecuritySetupReference);
}

void CGXDLMSAssociationShortName::GetAttributeIndexToRead(std::vector<int>& attributes)
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
    //AccessRightsList is static and read only once.
    if (!IsRead(3))
    {
        attributes.push_back(3);
    }
    //SecuritySetupReference is static and read only once.
    if (!IsRead(4))
    {
        attributes.push_back(4);
    }
}

// Returns amount of attributes.
int CGXDLMSAssociationShortName::GetAttributeCount()
{
    return 4;
}

// Returns amount of methods.
int CGXDLMSAssociationShortName::GetMethodCount()
{
    return 8;
}

int CGXDLMSAssociationShortName::GetDataType(int index, DLMS_DATA_TYPE& type)
{
    if (index == 1)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else if (index == 2)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 3)
    {
        type = DLMS_DATA_TYPE_ARRAY;
    }
    else if (index == 4)
    {
        type = DLMS_DATA_TYPE_OCTET_STRING;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

// Returns SN Association View.
int CGXDLMSAssociationShortName::GetObjects(CGXDLMSSettings& settings, CGXByteBuffer& data)
{
    unsigned long pos = 0;
    int ret;
    //Add count only for first time.
    if (settings.GetIndex() == 0)
    {
        settings.SetCount(m_ObjectList.size());
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        //Add count
        GXHelpers::SetObjectCount(m_ObjectList.size(), data);
    }
    for(CGXDLMSObjectCollection::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
    {
        ++pos;
        if (!(pos <= settings.GetIndex()))
        {
            data.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
            data.SetUInt8(4);//Count
            CGXDLMSVariant type = (*it)->GetObjectType();
            CGXDLMSVariant version = (*it)->GetVersion();
            CGXDLMSVariant sn = (*it)->GetShortName();
            CGXDLMSVariant ln((*it)->m_LN, 6, DLMS_DATA_TYPE_OCTET_STRING);
            if ((ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_INT16, sn)) != 0 || //base address.
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT16, type)) != 0 || //ClassID
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_UINT8, version)) != 0 || //Version
                    (ret = GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, ln)) != 0) //LN
            {
                return ret;
            }
            settings.SetIndex(settings.GetIndex() + 1);
            //If PDU is full.
            if (data.GetSize() >= settings.GetMaxReceivePDUSize())
            {
                break;
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSAssociationShortName::Invoke(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    // Check reply_to_HLS_authentication
    if (e.GetIndex() == 8)
    {
        int ret;
        unsigned long ic = 0;
        CGXByteBuffer* readSecret;
        if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
        {
            unsigned char ch;
            readSecret = &settings.GetSourceSystemTitle();
            CGXByteBuffer bb;
            bb.Set(e.GetParameters().byteArr, e.GetParameters().GetSize());
            if ((ret = bb.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if ((ret = bb.GetUInt32(&ic)) != 0)
            {
                return ret;
            }
        }
        else
        {
            readSecret = &m_Secret;
        }
        CGXByteBuffer serverChallenge;
        if ((ret = CGXSecure::Secure(settings, settings.GetCipher(), ic,
                                     settings.GetStoCChallenge(), *readSecret, serverChallenge)) != 0)
        {
            return ret;
        }

        if (serverChallenge.Compare(e.GetParameters().byteArr, e.GetParameters().GetSize()))
        {
            if (settings.GetAuthentication() == DLMS_AUTHENTICATION_HIGH_GMAC)
            {
                readSecret = &settings.GetCipher()->GetSystemTitle();
                ic = settings.GetCipher()->GetFrameCounter();
            }
            else
            {
                readSecret = &m_Secret;
            }
            serverChallenge.Clear();
            if ((ret = CGXSecure::Secure(settings,
                                         settings.GetCipher(),
                                         ic,
                                         settings.GetCtoSChallenge(),
                                         *readSecret,
                                         serverChallenge)) != 0)
            {
                return ret;
            }
            e.SetValue(serverChallenge);
            settings.SetConnected(true);

        }
        else
        {
            settings.SetConnected(false);
            return 0;
        }
    }
    else
    {
        e.SetError(DLMS_ERROR_CODE_READ_WRITE_DENIED);
    }
    return 0;
}

int CGXDLMSAssociationShortName::GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
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
    }
    else if (e.GetIndex() == 2)
    {
        CGXByteBuffer buff;
        int ret = GetObjects(settings, buff);
        e.SetValue(buff);
        return ret;
    }
    else if (e.GetIndex() == 3)
    {
        int ret;
        bool lnExists = m_ObjectList.FindBySN(GetShortName()) != NULL;
        //Add count
        int cnt = m_ObjectList.size();
        if (!lnExists)
        {
            ++cnt;
        }
        CGXByteBuffer data;
        data.SetUInt8(DLMS_DATA_TYPE_ARRAY);
        GXHelpers::SetObjectCount(cnt, data);
        for(std::vector<CGXDLMSObject*>::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
        {
            if ((ret = GetAccessRights(*it, data)) != 0)
            {
                return ret;
            }
        }
        if (!lnExists)
        {
            if ((ret = GetAccessRights(this, data)) != 0)
            {
                return ret;
            }
        }
        e.SetValue(data);
    }
    else if (e.GetIndex() == 4)
    {
        CGXByteBuffer data;
        CGXDLMSVariant tmp = m_SecuritySetupReference;
        GXHelpers::SetData(data, DLMS_DATA_TYPE_OCTET_STRING, tmp);
        e.SetValue(data);
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMSAssociationShortName::SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e)
{
    if (e.GetIndex() == 1)
    {
        return SetLogicalName(this, e.GetValue());
    }
    else if (e.GetIndex() == 2)
    {
        m_ObjectList.clear();
        if (e.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
        {
            for(std::vector<CGXDLMSVariant>::iterator item = e.GetValue().Arr.begin(); item != e.GetValue().Arr.end(); ++item)
            {
                int sn = item->Arr[0].ToInteger();
                CGXDLMSObject* pObj = settings.GetObjects().FindBySN(sn);
                if (pObj == NULL)
                {
                    DLMS_OBJECT_TYPE type = (DLMS_OBJECT_TYPE) item->Arr[1].ToInteger();
                    int version = item->Arr[2].ToInteger();
                    std::string ln;
                    GXHelpers::GetLogicalName((*item).Arr[3].byteArr, ln);
                    pObj = CGXDLMSObjectFactory::CreateObject(type, ln);
                    pObj->SetShortName(sn);
                    pObj->SetVersion(version);
                }
                m_ObjectList.push_back(pObj);
            }
        }
    }
    else if (e.GetIndex() == 3)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_NONE)
        {
            for(std::vector<CGXDLMSObject*>::iterator it = m_ObjectList.begin(); it != m_ObjectList.end(); ++it)
            {
                for(int pos = 1; pos != (*it)->GetAttributeCount(); ++pos)
                {
                    (*it)->SetAccess(pos, DLMS_ACCESS_MODE_NONE);
                }
            }
        }
        else
        {
            UpdateAccessRights(e.GetValue());
        }
    }
    else if (e.GetIndex() == 4)
    {
        if (e.GetValue().vt == DLMS_DATA_TYPE_STRING)
        {
            m_SecuritySetupReference = e.GetValue().ToString();
        }
        else
        {
            int ret;
            CGXDLMSVariant tmp;
            if ((ret = CGXDLMSClient::ChangeType(e.GetValue(), DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            m_SecuritySetupReference = tmp.ToString();
        }
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}
