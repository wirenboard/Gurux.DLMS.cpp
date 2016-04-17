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

#include "../include/enums.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXHelpers.h"
#include "../include/GXAPDU.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXSecure.h"
#include "../include/GXSerialNumberCounter.h"

CGXDLMSClient::CGXDLMSClient(bool UseLogicalNameReferencing,
                             int clientAddress,
                             int serverAddress,
                             //Authentication type.
                             GXDLMS_AUTHENTICATION authentication,
                             //Password if authentication is used.
                             const char* password,
                             GXDLMS_INTERFACETYPE intefaceType) : m_Settings(false)
{
    static CGXDLMS dlms;
    m_Cipher = NULL;
    m_IsAuthenticationRequired = false;
    m_Settings.SetUseLogicalNameReferencing(UseLogicalNameReferencing);
    m_Settings.SetClientAddress(clientAddress);
    m_Settings.SetServerAddress(serverAddress);
    m_Settings.SetInterfaceType(intefaceType);
    m_Settings.SetAuthentication(authentication);
    CGXByteBuffer tmp;
    tmp.AddString(password);
    m_Settings.SetPassword(tmp);
}

CGXDLMSClient::~CGXDLMSClient()
{
}

bool CGXDLMSClient::GetUseLogicalNameReferencing()
{
    return m_Settings.GetUseLogicalNameReferencing();
}

GXDLMS_INTERFACETYPE CGXDLMSClient::GetInterfaceType()
{
    return m_Settings.GetInterfaceType();
}

CGXDLMSLimits& CGXDLMSClient::GetLimits()
{
    return m_Settings.GetLimits();
}

int CGXDLMSClient::SNRMRequest(std::vector<CGXByteBuffer>& packets)
{
    int ret;
    m_Settings.SetConnected(false);
    packets.clear();
    m_IsAuthenticationRequired = false;
    m_Settings.SetMaxReceivePDUSize(0xFFFF);
    // SNRM request is not used in network connections.
    if (m_Settings.GetInterfaceType() == GXDLMS_INTERFACETYPE_WRAPPER)
    {
        return 0;
    }
    CGXByteBuffer data(25);
    // FromatID
    data.SetUInt8(0x81);
    // GroupID
    data.SetUInt8(0x80);
    // Length is updated later.
    data.SetUInt8(0);
    // If custom HDLC parameters are used.
    if (CGXDLMSLimits::DEFAULT_MAX_INFO_RX == GetLimits().GetMaxInfoTX().ToInteger())
    {
        data.SetUInt8(HDLC_INFO_MAX_INFO_TX);
        data.SetUInt8(GetLimits().GetMaxInfoTX().GetSize());
        if ((ret = GetLimits().GetMaxInfoTX().GetBytes(data)) != 0)
        {
            return ret;
        }
    }
    if (CGXDLMSLimits::DEFAULT_MAX_INFO_RX != GetLimits().GetMaxInfoRX().ToInteger())
    {
        data.SetUInt8(HDLC_INFO_MAX_INFO_RX);
        data.SetUInt8(GetLimits().GetMaxInfoRX().GetSize());
        if ((ret = GetLimits().GetMaxInfoRX().GetBytes(data)) != 0)
        {
            return ret;
        }
    }
    if (CGXDLMSLimits::DEFAULT_WINDOWS_SIZE_TX != GetLimits().GetWindowSizeTX().ToInteger())
    {
        data.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
        data.SetUInt8(GetLimits().GetWindowSizeTX().GetSize());
        if ((ret = GetLimits().GetWindowSizeTX().GetBytes(data)) != 0)
        {
            return ret;
        }
    }
    if (CGXDLMSLimits::DEFAULT_WINDOWS_SIZE_TX != GetLimits().GetWindowSizeRX().ToInteger())
    {
        data.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
        data.SetUInt8(GetLimits().GetWindowSizeRX().ToInteger());
        if ((ret = GetLimits().GetWindowSizeRX().GetBytes(data)) != 0)
        {
            return ret;
        }
    }
    // If default HDLC parameters are not used.
    if (data.GetSize() != 3)
    {
        // Length.
        data.SetUInt8(2, data.GetPosition() - 3);
    }
    else
    {
        data.Clear();
    }
    return CGXDLMS::SplitToHdlcFrames(m_Settings, DLMS_COMMAND_SNRM, &data, packets);
}

// SN referencing
int CGXDLMSClient::ParseSNObjects(CGXByteBuffer& buff, CGXDLMSObjectCollection& objects, bool onlyKnownObjects)
{
    int ret;
    CGXDataInfo info;
    //Get array tag.
    unsigned char ch;
    unsigned long cnt;
    //Check that data is in the array
    // Get array tag.
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 1)
    {
        return ERROR_CODES_INVALID_RESPONSE;
    }
    //get object count
    CGXDLMSVariant value;
    if ((ret = GXHelpers::GetObjectCount(buff, cnt)) != 0)
    {
        return ret;
    }
    for(unsigned long objPos = 0; objPos != cnt; ++objPos)
    {
        info.Clear();
        if ((ret = GXHelpers::GetData(buff, info, value)) != 0)
        {
            return ret;
        }
        if (value.vt != DLMS_DATA_TYPE_STRUCTURE || value.Arr.size() != 4)
        {
            return ERROR_CODES_INVALID_PARAMETER;
        }
        if (value.Arr[0].vt != DLMS_DATA_TYPE_INT16 ||
                value.Arr[1].vt != DLMS_DATA_TYPE_UINT16 ||
                value.Arr[2].vt != DLMS_DATA_TYPE_UINT8 ||
                value.Arr[3].vt != DLMS_DATA_TYPE_OCTET_STRING)
        {
            return ERROR_CODES_INVALID_PARAMETER;
        }
        short sn = value.Arr[0].ToInteger();
        unsigned short class_id = (unsigned short) value.Arr[1].ToInteger();
        unsigned char version = (unsigned char) value.Arr[2].ToInteger();
        CGXDLMSVariant ln = value.Arr[3];
        CGXDLMSObject* pObj = CGXDLMSObjectFactory::CreateObject((OBJECT_TYPE) class_id);
        pObj->SetShortName(sn);
        pObj->SetVersion(version);
        int cnt = ln.GetSize();
        assert(cnt == 6);
        CGXDLMSObject::SetLogicalName(pObj, ln);
        objects.push_back(pObj);
    }
    return 0;
}

int CGXDLMSClient::ParseLNObjects(CGXByteBuffer& buff, CGXDLMSObjectCollection& objects, bool onlyKnownObjects)
{
    int ret;
    unsigned long cnt;
    unsigned char ch;
    CGXDataInfo info;
    // Get array tag.
    if ((ret = buff.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Check that data is in the array
    if (ch != 0x01)
    {
        //Invalid response.
        return ERROR_CODES_INVALID_PARAMETER;
    }
    CGXDLMSVariant value;
    if ((ret = GXHelpers::GetObjectCount(buff, cnt)) != 0)
    {
        return ret;
    }
    for (unsigned long pos = 0; pos != cnt; ++pos)
    {
        // Some meters give wrong item count.
        // This fix Iskraemeco (MT-880) bug.
        if (buff.GetPosition() == buff.GetSize())
        {
            break;
        }
        info.SetType(DLMS_DATA_TYPE_NONE);
        info.SetIndex(0);
        info.SetCount(0);

        if ((ret = GXHelpers::GetData(buff, info, value)) != 0)
        {
            return ret;
        }
        if (value.Arr.size() != 4)
        {
            //Invalid structure format.
            return ERROR_CODES_INVALID_PARAMETER;
        }
        int classID = value.Arr[0].ToInteger() & 0xFFFF;
        if (classID > 0)
        {
            CGXDLMSObject* pObj = CGXDLMSObjectFactory::CreateObject((OBJECT_TYPE) classID);
            if (value.vt != DLMS_DATA_TYPE_STRUCTURE || value.Arr.size() != 4)
            {
                return ERROR_CODES_INVALID_PARAMETER;
            }
            if (value.Arr[0].vt != DLMS_DATA_TYPE_UINT16)
            {
                return ERROR_CODES_INVALID_PARAMETER;
            }
            if (value.Arr[1].vt != DLMS_DATA_TYPE_UINT8)
            {
                return ERROR_CODES_INVALID_PARAMETER;
            }
            unsigned char version = value.Arr[1].ToInteger();
            if (value.Arr[2].vt != DLMS_DATA_TYPE_OCTET_STRING)
            {
                return ERROR_CODES_INVALID_PARAMETER;
            }
            CGXDLMSVariant ln = value.Arr[2];
            //Get Access rights...
            if (value.Arr[3].vt != DLMS_DATA_TYPE_STRUCTURE || value.Arr[3].Arr.size() != 2)
            {
                return ERROR_CODES_INVALID_PARAMETER;
            }
            pObj->SetVersion(version);
            int cnt;
            // attribute_access_descriptor Start
            if (value.Arr[3].Arr[0].vt != DLMS_DATA_TYPE_ARRAY)
            {
                delete pObj;
                return ERROR_CODES_INVALID_PARAMETER;
            }
            if (value.Arr[3].Arr.size() == 2)
            {
                for(unsigned int pos = 0; pos != value.Arr[3].Arr[0].Arr.size(); ++pos)
                {
                    if (value.Arr[3].Arr[0].Arr[pos].vt != DLMS_DATA_TYPE_STRUCTURE ||
                            value.Arr[3].Arr[0].Arr[pos].Arr.size() != 3)
                    {
                        delete pObj;
                        return ERROR_CODES_INVALID_PARAMETER;
                    }
                    int id = value.Arr[3].Arr[0].Arr[pos].Arr[0].ToInteger();
                    //Get access_mode
                    DLMS_DATA_TYPE tp = value.Arr[3].Arr[0].Arr[pos].Arr[1].vt;
                    if (tp != DLMS_DATA_TYPE_ENUM)
                    {
                        delete pObj;
                        return ERROR_CODES_INVALID_PARAMETER;
                    }
                    pObj->SetAccess(id, (ACCESSMODE) value.Arr[3].Arr[0].Arr[pos].Arr[1].ToInteger());
                    //Get access_selectors
                    if (value.Arr[3].Arr[0].Arr[pos].Arr[2].vt == DLMS_DATA_TYPE_ARRAY)
                    {
                        int cnt2 = value.Arr[3].Arr[0].Arr[pos].Arr[2].Arr.size();
                        for(int pos2 = 0; pos2 != cnt2; ++pos2)
                        {
                            //Get access_mode
                        }
                    }
                    else if (value.Arr[3].Arr[0].Arr[pos].Arr[2].vt != DLMS_DATA_TYPE_NONE)
                    {
                        delete pObj;
                        return ERROR_CODES_INVALID_PARAMETER;
                    }
                }
                // attribute_access_descriptor End
                // method_access_item Start
                if (value.Arr[3].Arr[1].vt != DLMS_DATA_TYPE_ARRAY)
                {
                    delete pObj;
                    return ERROR_CODES_INVALID_PARAMETER;
                }
                for(unsigned int pos = 0; pos != value.Arr[3].Arr[1].Arr.size(); ++pos)
                {
                    CGXDLMSVariant tmp = value.Arr[3].Arr[1].Arr[pos];
                    if (tmp.vt != DLMS_DATA_TYPE_STRUCTURE ||
                            tmp.Arr.size() != 2)
                    {
                        delete pObj;
                        return ERROR_CODES_INVALID_PARAMETER;
                    }
                    int id = tmp.Arr[0].ToInteger();
                    //Get access_mode
                    //In version 0 data type is boolean.
                    if (tmp.Arr[1].vt != DLMS_DATA_TYPE_ENUM && tmp.Arr[1].vt != DLMS_DATA_TYPE_BOOLEAN)
                    {
                        delete pObj;
                        return ERROR_CODES_INVALID_PARAMETER;
                    }
                    pObj->SetMethodAccess(id, (METHOD_ACCESSMODE) tmp.Arr[1].ToInteger());
                }
            }
            // method_access_item End
            cnt = ln.GetSize();
            assert(cnt == 6);
            if ((ret = CGXDLMSObject::SetLogicalName(pObj, ln)) != 0)
            {
                return ret;
            }
            objects.push_back(pObj);
        }
    }
    return 0;
}

int CGXDLMSClient::ParseObjects(CGXByteBuffer& data, CGXDLMSObjectCollection& objects, bool onlyKnownObjects)
{
    int ret;
    objects.clear();
    if (GetUseLogicalNameReferencing())
    {
        if ((ret = ParseLNObjects(data, objects, onlyKnownObjects)) != 0)
        {
            return ret;
        }
    }
    else
    {
        if ((ret = ParseSNObjects(data, objects, onlyKnownObjects)) != 0)
        {
            return ret;
        }
    }
    m_Settings.GetObjects().insert(m_Settings.GetObjects().end(), objects.begin(), objects.end());
    return 0;
}

int CGXDLMSClient::UpdateValue(CGXDLMSObject& target, int attributeIndex, CGXDLMSVariant& value)
{
    int ret;
    if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        DLMS_DATA_TYPE type;
        if ((ret = target.GetUIDataType(attributeIndex, type)) != 0)
        {
            return ret;
        }
        if (type == DLMS_DATA_TYPE_DATETIME && value.GetSize() == 5)
        {
            type = DLMS_DATA_TYPE_DATE;
            target.SetUIDataType(attributeIndex, type);
        }
        if (type != DLMS_DATA_TYPE_NONE)
        {
            if ((ret = ChangeType(value, type, value)) != 0)
            {
                return ret;
            }
        }
    }
    return target.SetValue(&m_Settings, attributeIndex, value);
}

int CGXDLMSClient::GetValue(CGXByteBuffer& data, CGXDLMSVariant& value)
{
    CGXDataInfo info;
    return GXHelpers::GetData(data, info, value);
}

int CGXDLMSClient::UpdateValues(std::vector< std::pair<CGXDLMSObject*, int> >& list, CGXByteBuffer& data)
{
    CGXDLMSVariant value;
    CGXDataInfo info;
    unsigned long cnt;
    int ret;
    unsigned char ch;
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    if (cnt != (unsigned long) list.size())
    {
        // Invalid reply. Read items count do not match.
        return ERROR_CODES_INVALID_PARAMETER;
    }
    for (std::vector< std::pair<CGXDLMSObject*, int> >::iterator it = list.begin(); it != list.end(); ++it)
    {
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch == 0)
        {
            if ((ret = GXHelpers::GetData(data, info, value)) != 0)
            {
                return ret;
            }
            it->first->SetValue(&m_Settings, it->second, value);
            info.Clear();
        }
        else
        {
            //Return error code.
            return ch;
        }
    }
    return 0;
}

int CGXDLMSClient::ChangeType(CGXDLMSVariant& value, DLMS_DATA_TYPE type, CGXDLMSVariant& newValue)
{
    CGXByteBuffer tmp;
    tmp.AddRange(value.byteArr, value.GetSize());
    return ChangeType(tmp, type, newValue);
}

int CGXDLMSClient::ChangeType(CGXByteBuffer& value, DLMS_DATA_TYPE type, CGXDLMSVariant& newValue)
{
    int ret;
    CGXDataInfo info;
    newValue.Clear();
    if (value.GetSize() == 0)
    {
        return 0;
    }
    if (type == DLMS_DATA_TYPE_NONE)
    {
        newValue = value.ToHexString();
    }
    if (value.GetSize() == 0 && (type == DLMS_DATA_TYPE_STRING || type == DLMS_DATA_TYPE_OCTET_STRING))
    {
        newValue = "";
        return 0;
    }
    info.SetType(type);
    if ((ret = GXHelpers::GetData(value, info, newValue)) != 0)
    {
        return ret;
    }
    if (!info.IsCompleate())
    {
        return ERROR_CODES_OUTOFMEMORY;
    }
    if (type == DLMS_DATA_TYPE_OCTET_STRING && newValue.vt == DLMS_DATA_TYPE_OCTET_STRING)
    {
        int size = newValue.size;
        std::string str;
        if (size == 0)
        {
            str = "";
        }
        else
        {
            CGXByteBuffer bcd(size * 4);
            for (int pos = 0; pos != size; ++pos)
            {
                bcd.AddIntAsString(newValue.byteArr[pos]);
                bcd.SetUInt8('.');
            }
            //Remove last dot.
            if (bcd.GetSize() != 0)
            {
                bcd.SetSize(bcd.GetSize() - 1);
            }
            str = bcd.ToString();
        }
        newValue = str;
    }
    return ret;
}

int CGXDLMSClient::ParseUAResponse(CGXByteBuffer& data)
{
    unsigned char ch, id, len;
    unsigned short ui;
    unsigned long ul;
    int ret;
    if (data.GetSize() == 0)
    {
        return 0;
    }
    // Skip FromatID
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Skip Group ID.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Skip Group len
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    CGXDLMSVariant value;
    while (data.GetPosition() < data.GetSize())
    {
        if ((ret = data.GetUInt8(&id)) != 0)
        {
            return ret;
        }
        if ((ret = data.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        switch (len)
        {
        case 1:
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            value = ch;
            break;
        case 2:
            if ((ret = data.GetUInt16(&ui)) != 0)
            {
                return ret;
            }
            value = ui;
            break;
        case 4:
            if ((ret = data.GetUInt32(&ul)) != 0)
            {
                return ret;
            }
            value = ul;
            break;
        default:
            return ERROR_CODES_INVALID_PARAMETER;
        }
        switch(id)
        {
        case HDLC_INFO_MAX_INFO_TX:
            m_Settings.GetLimits().SetMaxInfoTX(value);
            break;
        case HDLC_INFO_MAX_INFO_RX:
            m_Settings.GetLimits().SetMaxInfoRX(value);
            break;
        case HDLC_INFO_WINDOW_SIZE_TX:
            m_Settings.GetLimits().SetWindowSizeTX(value);
            break;
        case HDLC_INFO_WINDOW_SIZE_RX:
            m_Settings.GetLimits().SetWindowSizeRX(value);
            break;
        default:
            ret = ERROR_CODES_INVALID_PARAMETER;
            break;
        }
    }
    return ret;
}

int CGXDLMSClient::AARQRequest(std::vector<CGXByteBuffer>& packets)
{
    m_Settings.SetConnected(true);
    CGXByteBuffer buff(20);
    unsigned char ln[] = {0x00, 0x7E, 0x1F};
    unsigned char sn[] = {0x1C, 0x03, 0x20};

    int ret = CGXDLMS::CheckInit(m_Settings);
    if (ret != 0)
    {
        return ret;
    }
    packets.clear();
    GXAPDU aarq;
    if (m_Settings.GetUseLogicalNameReferencing())
    {
        m_Settings.GetLnSettings().SetConformanceBlock(ln);
    }
    else
    {
        m_Settings.GetSnSettings().SetConformanceBlock(sn);
    }
    m_Settings.GetStoCChallenge().Clear();
    if (m_Settings.GetAuthentication() > GXDLMS_AUTHENTICATION_HIGH)
    {
        CGXByteBuffer challenge;
        if ((ret = CGXSecure::GenerateChallenge(m_Settings.GetAuthentication(), challenge)) != 0)
        {
            return ret;
        }
        m_Settings.SetCtoSChallenge(challenge);
    }
    else
    {
        m_Settings.GetCtoSChallenge().Clear();
    }
    bool ciphering = m_Cipher != NULL && m_Cipher->IsCiphered();
    aarq.CodeData(m_Settings, ciphering, buff);
    return CGXDLMS::SplitPdu(m_Settings, DLMS_COMMAND_AARQ, 0, buff, ERROR_CODES_OK, m_Cipher, packets);
}

/**
* Parses the AARE response. Parse method will update the following data:
* <ul>
* <li>DLMSVersion</li>
* <li>MaxReceivePDUSize</li>
* <li>UseLogicalNameReferencing</li>
* <li>LNSettings or SNSettings</li>
* </ul>
* LNSettings or SNSettings will be updated, depending on the referencing,
* Logical name or Short name.
*
* reply
*            Received data.
* @see GXDLMSClient#aarqRequest
* @see GXDLMSClient#getUseLogicalNameReferencing
* @see GXDLMSClient#getLNSettings
* @see GXDLMSClient#getSNSettings
*/
int CGXDLMSClient::ParseAAREResponse(CGXByteBuffer& reply)
{
    int ret;
    GXAPDU pdu;
    m_IsAuthenticationRequired = GXDLMS_SOURCE_DIAGNOSTIC_NULL;
    if ((ret = pdu.EncodeData(m_Settings, reply)) != 0)
    {
        return ret;
    }
    GXDLMS_ASSOCIATION_RESULT ar = pdu.GetResultComponent();
    GXDLMS_SOURCE_DIAGNOSTIC sd = pdu.GetResultDiagnostic();
    if (ar != GXDLMS_ASSOCIATION_RESULT_ACCEPTED)
    {
        switch(sd)
        {
        case GXDLMS_SOURCE_DIAGNOSTIC_NO_REASON_GIVEN:
            return ERROR_CODES_NO_REASON_GIVEN;
            break;
        case GXDLMS_SOURCE_DIAGNOSTIC_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED:
            return ERROR_CODES_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED;
            break;
        case GXDLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED:
            return ERROR_CODES_AUTHENTICATION_MECHANISM_NAME_NOT_RECOGNISED;
            break;
        case GXDLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED:
            return ERROR_CODES_AUTHENTICATION_MECHANISM_NAME_REQUIRED;
            break;
        case GXDLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE:
            return ERROR_CODES_AUTHENTICATION_FAILURE;
            break;
        default:
            //OK.
            break;
        }
    }
    m_IsAuthenticationRequired = sd == GXDLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED;
    m_Settings.SetConnected(true);
    return ret;
}

int CGXDLMSClient::ReceiverReady(GXDLMS_DATA_REQUEST_TYPES type, CGXByteBuffer& packets)
{
    return CGXDLMS::ReceiverReady(m_Settings, type, NULL, packets);
}

std::string CGXDLMSClient::ObjectTypeToString(OBJECT_TYPE type)
{
    return CGXDLMSObjectFactory::ObjectTypeToString(type);
}

void CGXDLMSClient::UpdateOBISCodes(CGXDLMSObjectCollection& objects)
{
    /*
    std::stream sb;
    sb << OBIS_CODES1;
    sb << OBIS_CODES2;
    sb << OBIS_CODES3;
    sb << OBIS_CODES4;
    sb << OBIS_CODES5;
    sb << OBIS_CODES6;
    sb << OBIS_CODES7;
    sb << OBIS_CODES8;
    sb << OBIS_CODES9;
    sb << OBIS_CODES10;
    sb << OBIS_CODES11;
    CGXStandardObisCodeCollection codes;
    std::string str = sb.str();
    std::vector< std::string > rows = GXHelpers::Split(str, "\r\n", true);
    int row = 0;
    std::string last;
    for(std::vector< std::string >::iterator it = rows.begin(); it != rows.end(); ++it)
    {
        std::vector< std::string > items = GXHelpers::Split(*it, ";\r\n", false);
    	if (items.size() != 8)
    	{
    		items = GXHelpers::Split(*it, ";\r\n", false);
    	}
    	assert(items.size() == 8);
        std::vector< std::string > obis = GXHelpers::Split(items[0], ".\r\n", false);
    	if (obis.size() != 6)
    	{
    		obis = GXHelpers::Split(items[0], ".\r\n", false);
    	}
        std::string str = items[3] + "; " + items[4] + "; " + items[5] + "; " + items[6] + "; " + items[7];
        codes.push_back(CGXStandardObisCode(obis, str, items[1], items[2]));
    	++row;
    	last = *it;
    }
    for(std::vector<CGXDLMSObject*>::iterator it = objects.begin(); it != objects.end(); ++it)
    {
        std::string ln;
    	(*it)->GetLogicalName(ln);
        CGXStandardObisCode code;
        if (codes.Find(ln, (*it)->GetObjectType(), code))
        {
            (*it)->SetDescription(code.GetDescription());
            //If std::string is used
    		if (code.GetDataType().find("10") != std::string::npos)
            {
                code.SetDataType("10");
            }
            //If date time is used.
            else if (code.GetDataType().find("25") != std::string::npos ||
    			code.GetDataType().find("26") != std::string::npos)
            {
                code.SetDataType("25");
            }
            else if (code.GetDataType().find("9"))
            {
                //Time stamps of the billing periods objects (first scheme if there are two)
                if ((CGXStandardObisCodeCollection::EqualsMask("0.0-64.96.7.10-14.255", ln) ||
                    //Time stamps of the billing periods objects (second scheme)
                    CGXStandardObisCodeCollection::EqualsMask("0.0-64.0.1.5.0-99,255", ln) ||
                    //Time of power failure
                    CGXStandardObisCodeCollection::EqualsMask("0.0-64.0.1.2.0-99,255", ln) ||
                    //Time stamps of the billing periods objects (first scheme if there are two)
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.1.2.0-99,255", ln) ||
                    //Time stamps of the billing periods objects (second scheme)
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.1.5.0-99,255", ln) ||
                    //Time expired since last end of billing period
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.0.255", ln) ||
                    //Time of last reset
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.6.255", ln) ||
                    //Date of last reset
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.7.255", ln) ||
                    //Time expired since last end of billing period (Second billing period scheme)
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.13.255", ln) ||
                    //Time of last reset (Second billing period scheme)
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.14.255", ln) ||
                    //Date of last reset (Second billing period scheme)
                    CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.15.255", ln)))
                {
                    code.SetDataType("25");
                }
                //Local time
                else if (CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.1.255", ln))
                {
                    code.SetDataType("27");
                }
                //Local date
                else if (CGXStandardObisCodeCollection::EqualsMask("1.0-64.0.9.2.255", ln))
                {
                    code.SetDataType("26");
                }
    			//Active firmware identifier
    			else if (CGXStandardObisCodeCollection::EqualsMask("1.0.0.2.0.255", ln))
                {
                    code.SetDataType("10");
                }
            }
            if (code.GetDataType() != "*" &&
                    code.GetDataType() != "" &&
                    code.GetDataType().find(",") == std::string::npos)
            {
    			int value;
    #if _MSC_VER > 1000
    			sscanf_s(code.GetDataType().c_str(), "%d", &value);
    #else
    			sscanf(code.GetDataType().c_str(), "%d", &value);
    #endif
    			DLMS_DATA_TYPE type = (DLMS_DATA_TYPE) value;
    			switch ((*it)->GetObjectType())
    			{
    				case OBJECT_TYPE_DATA:
    				case OBJECT_TYPE_REGISTER:
    				case OBJECT_TYPE_REGISTER_ACTIVATION:
    				case OBJECT_TYPE_EXTENDED_REGISTER:
    					(*it)->SetUIDataType(2, type);
    					break;
    				default:
    					break;
    			}
    		}
    	}
    	else
    	{
    		System.out.println("Unknown OBIS Code: " + it.getLogicalName() +
    				" Type: " + it.getObjectType());
    	}
    }
    */
}

int CGXDLMSClient::DisconnectRequest(std::vector<CGXByteBuffer>& packets)
{
    // If connection is not established, there is no need to send DisconnectRequest.
    if (!m_Settings.IsConnected())
    {
        return 0;
    }
    if (GetInterfaceType() == GXDLMS_INTERFACETYPE_HDLC)
    {
        return CGXDLMS::SplitToHdlcFrames(m_Settings, DLMS_FRAME_TYPE_DISCONNECT_REQUEST, NULL, packets);
    }
    CGXByteBuffer bb(2);
    bb.SetUInt8(DLMS_COMMAND_DISCONNECT_REQUEST);
    bb.SetUInt8(0x0);
    CGXDLMS::SplitToWrapperFrames(m_Settings, &bb, packets);
    return 0;
}

int CGXDLMSClient::GetData(CGXByteBuffer& reply, CGXReplyData& data)
{
    return CGXDLMS::GetData(m_Settings, reply, data, m_Cipher);
}

int CGXDLMSClient::GetObjectsRequest(std::vector<CGXByteBuffer>& reply)
{
    CGXDLMSVariant name;
    if (GetUseLogicalNameReferencing())
    {
        name = "0.0.40.0.0.255";
    }
    else
    {
        name = (short) 0xFA00;
    }
    return Read(name, OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, 2, reply);
}

int CGXDLMSClient::Read(CGXDLMSVariant name, OBJECT_TYPE objectType, int attributeOrdinal, std::vector<CGXByteBuffer>& reply)
{
    return Read(name, objectType, attributeOrdinal, NULL, reply);
}

int CGXDLMSClient::Read(CGXDLMSVariant name, OBJECT_TYPE objectType, int attributeOrdinal, CGXByteBuffer* data,
                        std::vector<CGXByteBuffer>& reply)
{
    if ((attributeOrdinal < 1))
    {
        //Invalid parameter
        return ERROR_CODES_INVALID_PARAMETER;
    }
    m_Settings.ResetBlockIndex();
    DLMS_COMMAND cmd;
    CGXByteBuffer bb;
    if (GetUseLogicalNameReferencing())
    {
        cmd = DLMS_COMMAND_GET_REQUEST;
        // CI
        bb.SetUInt16(objectType);
        // Add LN
        unsigned char ln[6];
        GXHelpers::SetLogicalName(name.strVal.c_str(), ln);
        bb.AddRange(ln, 6);
        // Attribute ID.
        bb.SetUInt8(attributeOrdinal);
        if (data == NULL || data->GetSize() == 0)
        {
            // Access selection is not used.
            bb.SetUInt8(0);
        }
        else
        {
            // Access selection is used.
            bb.SetUInt8(1);
            // Add data.
            bb.AddRange2(data, 0, data->GetSize());
        }
    }
    else
    {
        cmd = DLMS_COMMAND_READ_REQUEST;
        // Add length.
        bb.SetUInt8(1);
        // Add Selector.
        if (data != NULL && data->GetSize() != 0)
        {
            bb.SetUInt8(4);
        }
        else
        {
            bb.SetUInt8(2);
        }
        int sn = name.ToInteger();
        sn += (attributeOrdinal - 1) * 8;
        bb.SetUInt16(sn);
        // Add data.
        if (data != NULL && data->GetSize() != 0)
        {
            bb.AddRange2(data, 0, data->GetSize());
        }
    }
    return CGXDLMS::SplitPdu(m_Settings, cmd, 1, bb, ERROR_CODES_OK, m_Cipher, reply);
}

int CGXDLMSClient::Write(CGXDLMSObject* pObject,
                         int index,
                         CGXDLMSVariant& data,
                         std::vector<CGXByteBuffer>& reply)
{
    return Write(pObject->GetName(), pObject->GetObjectType(), index, data, reply);
}

int CGXDLMSClient::Write(CGXDLMSVariant name, OBJECT_TYPE objectType,
                         int index, CGXDLMSVariant& data, std::vector<CGXByteBuffer>& reply)
{
    if (index < 1)
    {
        //Invalid parameter
        return ERROR_CODES_INVALID_PARAMETER;
    }
    m_Settings.ResetBlockIndex();
    CGXByteBuffer bb;
    DLMS_COMMAND cmd;
    if (GetUseLogicalNameReferencing())
    {
        cmd = DLMS_COMMAND_SET_REQUEST;
        // Add CI.
        bb.SetUInt16(objectType);
        // Add LN.
        unsigned char ln[6];
        GXHelpers::SetLogicalName(name.strVal.c_str(), ln);
        bb.AddRange(ln, 6);
        // Attribute ID.
        bb.SetUInt8(index);
        // Access selection is not used.
        bb.SetUInt8(0);
    }
    else
    {
        cmd = DLMS_COMMAND_WRITE_REQUEST;
        // Add SN count.
        bb.SetUInt8(1);
        // Add name length.
        bb.SetUInt8(2);
        // Add name.
        int sn = name.ToInteger();
        sn += (index - 1) * 8;
        bb.SetUInt16(sn);
        // Add data count.
        bb.SetUInt8(1);
    }
    GXHelpers::SetData(bb, data.vt, data);
    return CGXDLMS::SplitPdu(m_Settings, cmd, 1, bb, ERROR_CODES_OK, m_Cipher, reply);
}

/**
    * Generate Method (Action) request.
    *
    * @param item
    *            Method object short name or Logical Name.
    * @param index
    *            Method index.
    * @param data
    *            Method data.
    * @param type
    *            Data type.
    * @return DLMS action message.
    */
int CGXDLMSClient::Method(CGXDLMSObject* item, int index,
                          CGXDLMSVariant& data, std::vector<CGXByteBuffer>& reply)
{
    return Method(item->GetName(), item->GetObjectType(), index, data, reply);
}

/**
    * Generate Method (Action) request..
    *
    * @param name
    *            Method object short name or Logical Name.
    * @param objectType
    *            Object type.
    * @param index
    *            Method index.
    * @param value
    *            Method data.
    * @param dataType
    *            Data type.
    * @return DLMS action message.
    */
int CGXDLMSClient::Method(CGXDLMSVariant name, OBJECT_TYPE objectType,
                          int index, CGXDLMSVariant& data, std::vector<CGXByteBuffer>& reply)
{
    if (index < 1)
    {
        //Invalid parameter
        return ERROR_CODES_INVALID_PARAMETER;
    }

    CGXByteBuffer bb;
    DLMS_COMMAND cmd;
    m_Settings.ResetBlockIndex();
    if (GetUseLogicalNameReferencing())
    {
        cmd = DLMS_COMMAND_METHOD_REQUEST;
        // CI
        bb.SetUInt16(objectType);
        // Add LN.
        unsigned char ln[6];
        GXHelpers::SetLogicalName(name.strVal.c_str(), ln);
        bb.AddRange(ln, 6);
        // Attribute ID.
        bb.SetUInt8(index);
        // Method Invocation Parameters is not used.
        if (data.vt == DLMS_DATA_TYPE_NONE)
        {
            bb.SetUInt8(0);
        }
        else
        {
            bb.SetUInt8(1);
        }
    }
    else
    {
        int value, count;
        CGXDLMS::GetActionInfo(objectType, value, count);
        if (index > count)
        {
            //Invalid parameter
            return ERROR_CODES_INVALID_PARAMETER;
        }
        int sn = name.ToInteger();
        index = (value + (index - 1) * 0x8);
        sn += index;
        cmd = DLMS_COMMAND_READ_REQUEST;
        // Add SN count.
        bb.SetUInt8(1);
        // Add name length.
        bb.SetUInt8(4);
        // Add name.
        bb.SetUInt16(sn);
        // Method Invocation Parameters is not used.
        if (data.vt == DLMS_DATA_TYPE_NONE)
        {
            bb.SetUInt8(0);
        }
        else
        {
            bb.SetUInt8(1);
        }
    }
    if (data.vt != DLMS_DATA_TYPE_NONE)
    {
        if (data.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            GXHelpers::SetData(bb, data.vt, data);
            bb.AddRange(data.byteArr, data.size);
        }
        else
        {
            GXHelpers::SetData(bb, data.vt, data);
        }
    }
    return CGXDLMS::SplitPdu(m_Settings, cmd, 1, bb, ERROR_CODES_OK, m_Cipher, reply);
}

int CGXDLMSClient::ReadRowsByEntry(CGXDLMSProfileGeneric* pg,
                                   int index, int count, std::vector<CGXByteBuffer>& reply)
{
    CGXByteBuffer buff(19);
    // Add AccessSelector value
    buff.SetUInt8(0x02);
    // Add enum tag.
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    // Add item count
    buff.SetUInt8(0x04);
    // Add start index
    CGXDLMSVariant tmp = index;
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT32, tmp);
    // Add Count
    tmp = count;
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT32, tmp);
    // Read all columns.
    if (GetUseLogicalNameReferencing())
    {
        tmp = 1;
        GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp);
    }
    else
    {
        tmp = 0;
        GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp);
    }
    tmp = 0;
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp);
    return Read(pg->GetName(), OBJECT_TYPE_PROFILE_GENERIC, 2, &buff, reply);
}

int CGXDLMSClient::ReadRowsByRange(CGXDLMSProfileGeneric* pg,
                                   struct tm* start,
                                   struct tm* end,
                                   std::vector<CGXByteBuffer>& reply)
{
    m_Settings.ResetBlockIndex();
    CGXDLMSObject* sort = pg->GetSortObject();
    if (sort == NULL && pg->GetCaptureObjects().size() != 0)
    {
        sort = pg->GetCaptureObjects().at(0).first;
    }
    // If sort object is not found or it is not clock object read all.
    if (sort == NULL || sort->GetObjectType() != OBJECT_TYPE_CLOCK)
    {
        return Read(pg->GetName(), pg->GetObjectType(), 2, reply);
    }
    CGXByteBuffer buff(51);
    // Add AccessSelector value.
    buff.SetUInt8(0x01);
    // Add enum tag.
    buff.SetUInt8(DLMS_DATA_TYPE_STRUCTURE);
    // Add item count
    buff.SetUInt8(0x04);
    // Add enum tag.
    buff.SetUInt8(0x02);
    // Add item count
    buff.SetUInt8(0x04);
    // CI
    CGXDLMSVariant tmp = sort->GetObjectType();
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp);
    // LN
    CGXDLMSVariant ln = sort->GetName();
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_OCTET_STRING, ln);
    // Add attribute index.
    tmp = 2;
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_INT8, tmp);
    // Add version
    tmp = sort->GetVersion();
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_UINT16, tmp);
    tmp = *start;
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_DATETIME, tmp); // Add start time
    tmp = *end;
    GXHelpers::SetData(buff, DLMS_DATA_TYPE_DATETIME, tmp); // Add start time
    // Add array of read columns. Read All...
    buff.SetUInt8(0x01);
    // Add item count
    buff.SetUInt8(0x00);
    return Read(pg->GetName(), OBJECT_TYPE_PROFILE_GENERIC, 2, &buff, reply);
}

int CGXDLMSClient::GetServerAddress(unsigned long serialNumber,
                                    const char* formula)
{
    // If formula is not given use default formula.
    // This formula is defined in DLMS specification.
    if (formula == NULL || strlen(formula) == 0)
    {
        return CGXSerialNumberCounter::Count(serialNumber, "SN % 10000 + 1000");
    }
    return CGXSerialNumberCounter::Count(serialNumber, formula);
}

int  CGXDLMSClient::GetServerAddress(unsigned long logicalAddress,
                                     unsigned long physicalAddress, unsigned char addressSize)
{
    if (addressSize < 4 && physicalAddress < 0x80
            && logicalAddress < 0x80)
    {
        return logicalAddress << 7 | physicalAddress;
    }
    if (physicalAddress < 0x4000 && logicalAddress < 0x4000)
    {
        return logicalAddress << 14 | physicalAddress;
    }
    return ERROR_CODES_INVALID_PARAMETER;
}