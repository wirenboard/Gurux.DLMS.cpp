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

#include "../include/GXDLMSServer.h"
#include "../include/GXDLMS.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXDLMSProfileGeneric.h"
#include "../include/GXDLMSAssociationShortName.h"
#include "../include/GXDLMSAssociationLogicalName.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXAPDU.h"
#include "../include/GXSecure.h"
#include "../include/GXDLMSValueEventCollection.h"


CGXDLMSServer::CGXDLMSServer(bool logicalNameReferencing,
                             DLMS_INTERFACE_TYPE type) : m_Settings(true)
{
    m_Settings.SetUseLogicalNameReferencing(logicalNameReferencing);
    m_Settings.SetInterfaceType(type);
    Reset();
}

CGXDLMSServer::~CGXDLMSServer()
{

}

CGXDLMSObjectCollection& CGXDLMSServer::GetItems()
{
    return m_Settings.GetObjects();
}

void CGXDLMSServer::SetCipher(CGXCipher* value)
{
    m_Settings.SetCipher(value);
}

CGXDLMSSettings& CGXDLMSServer::GetSettings()
{
    return m_Settings;
}

CGXByteBuffer& CGXDLMSServer::GetCtoSChallenge()
{
    return m_Settings.GetCtoSChallenge();
}

CGXByteBuffer& CGXDLMSServer::GetStoCChallenge()
{
    return m_Settings.GetStoCChallenge();
}

DLMS_INTERFACE_TYPE CGXDLMSServer::GetInterfaceType()
{
    return m_Settings.GetInterfaceType();
}

void CGXDLMSServer::SetStoCChallenge(CGXByteBuffer& value)
{
    m_Settings.SetUseCustomChallenge(value.GetSize() != 0);
    m_Settings.SetStoCChallenge(value);
}

void CGXDLMSServer::SetStartingPacketIndex(int value)
{
    m_Settings.SetBlockIndex(value);
}

int CGXDLMSServer::GetInvokeID()
{
    return m_Settings.GetInvokeID();
}

CGXDLMSLimits CGXDLMSServer::GetLimits()
{
    return m_Settings.GetLimits();
}

unsigned short CGXDLMSServer::GetMaxReceivePDUSize()
{
    return m_Settings.GetMaxServerPDUSize();
}

void CGXDLMSServer::SetMaxReceivePDUSize(unsigned short value)
{
    m_Settings.SetMaxServerPDUSize(value);
}

bool CGXDLMSServer::GetUseLogicalNameReferencing()
{
    return m_Settings.GetUseLogicalNameReferencing();
}

/**
 * @param value
 *            Is Logical Name referencing used.
 */
void CGXDLMSServer::SetUseLogicalNameReferencing(bool value)
{
    m_Settings.SetUseLogicalNameReferencing(value);
}

/**
 * Gets Logical Name settings.
 *
 * @return Logical Name m_Settings.
 */
CGXDLMSLNSettings& CGXDLMSServer::GetLNSettings()
{
    return m_Settings.GetLnSettings();
}

CGXDLMSSNSettings& CGXDLMSServer::GetSNSettings()
{
    return m_Settings.GetSnSettings();
}

int CGXDLMSServer::Initialize()
{
    int ret;
    CGXDLMSObject* associationObject = NULL;
    m_Initialized = true;
    std::string ln;
    for(CGXDLMSObjectCollection::iterator it = m_Settings.GetObjects().begin(); it != m_Settings.GetObjects().end(); ++it)
    {
        (*it)->GetLogicalName(ln);
        if (ln.size() == 0)
        {
            //Invalid Logical Name.
            return DLMS_ERROR_CODE_INVALID_LOGICAL_NAME;
        }
        if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            CGXDLMSProfileGeneric* pg = (CGXDLMSProfileGeneric*) (*it);
            if (pg->GetProfileEntries() < 1)
            {
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            if (pg->GetCapturePeriod() > 0)
            {
                //TODO: Start thread. new GXProfileGenericUpdater(this, pg).start();
            }
        }
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME
                 && !m_Settings.GetUseLogicalNameReferencing())
        {
            CGXDLMSObjectCollection& list = ((CGXDLMSAssociationShortName*) *it)->GetObjectList();
            if (list.size() == 0)
            {
                list.insert(list.end(), GetItems().begin(), GetItems().end());
            }
            associationObject = *it;
        }
        else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME
                 && m_Settings.GetUseLogicalNameReferencing())
        {
            CGXDLMSObjectCollection& list = ((CGXDLMSAssociationLogicalName*) *it)->GetObjectList();
            if (list.size() == 0)
            {
                list.insert(list.end(), GetItems().begin(), GetItems().end());
            }
            associationObject = *it;
        }
    }
    if (associationObject == NULL)
    {
        if (GetUseLogicalNameReferencing())
        {
            CGXDLMSAssociationLogicalName* it2 = (CGXDLMSAssociationLogicalName*) CGXDLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME);
            CGXDLMSObjectCollection& list = it2->GetObjectList();
            GetItems().push_back(it2);
            list.insert(list.end(), GetItems().begin(), GetItems().end());
        }
        else
        {
            CGXDLMSAssociationShortName* it2 = (CGXDLMSAssociationShortName*) CGXDLMSObjectFactory::CreateObject(DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME);
            CGXDLMSObjectCollection& list = it2->GetObjectList();
            GetItems().push_back(it2);
            list.insert(list.end(), GetItems().begin(), GetItems().end());
        }
    }
    // Arrange items by Short Name.
    short sn = 0xA0;
    if (!m_Settings.GetUseLogicalNameReferencing())
    {
        unsigned char offset, count;

        for (CGXDLMSObjectCollection::iterator it = m_Settings.GetObjects().begin();
                it != m_Settings.GetObjects().end(); ++it)
        {
            // Generate Short Name if not given.
            if ((*it)->GetShortName() == 0)
            {
                (*it)->SetShortName(sn);
                // Add method index addresses.
                if ((ret = CGXDLMS::GetActionInfo((*it)->GetObjectType(), offset, count)) != 0)
                {
                    return ret;
                }
                if (count != 0)
                {
                    sn += offset + (8 * count);
                }
                else
                {
                    // If there are no methods.
                    // Add attribute index addresses.
                    sn += 8 * (*it)->GetAttributeCount();
                }
            }
        }
    }
    return 0;
}

void CGXDLMSServer::Reset()
{
    m_Transaction = NULL;
    m_Settings.SetCount(0);
    m_Settings.SetIndex(0);
    m_Info.Clear();
    m_Settings.SetConnected(false);
    m_ReceivedData.Clear();
    m_ReplyData.Clear();
    m_Settings.SetServerAddress(0);
    m_Settings.SetClientAddress(0);
    m_Settings.SetAuthentication(DLMS_AUTHENTICATION_NONE);
    if (m_Settings.GetCipher() != NULL)
    {
        m_Settings.GetCipher()->Reset();
    }
}

/**
    * Parse AARQ request that client send and returns AARE request.
    *
    * @return Reply to the client.
    */
int CGXDLMSServer::HandleAarqRequest(CGXByteBuffer& data)
{
    int ret;
    unsigned long serverAddress, clientAddress;
    DLMS_ASSOCIATION_RESULT result = DLMS_ASSOCIATION_RESULT_ACCEPTED;
    m_Settings.GetCtoSChallenge().Clear();
    m_Settings.GetStoCChallenge().Clear();
    DLMS_SOURCE_DIAGNOSTIC diagnostic;
    ret = CGXAPDU::ParsePDU(m_Settings, m_Settings.GetCipher(), data, diagnostic);
    if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        serverAddress = m_Settings.GetServerAddress();
        clientAddress = m_Settings.GetClientAddress();
        Reset();
        m_Settings.SetServerAddress(serverAddress);
        m_Settings.SetClientAddress(clientAddress);
    }
    if (ret != 0)
    {
        return ret;
    }
    if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
    {
        result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
        diagnostic = DLMS_SOURCE_DIAGNOSTIC_NOT_SUPPORTED;
    }
    else
    {
        diagnostic = ValidateAuthentication(m_Settings.GetAuthentication(), m_Settings.GetPassword());
        if (diagnostic != DLMS_SOURCE_DIAGNOSTIC_NONE)
        {
            result = DLMS_ASSOCIATION_RESULT_PERMANENT_REJECTED;
        }
        else if (m_Settings.GetAuthentication() > DLMS_AUTHENTICATION_LOW)
        {
            // If High authentication is used.
            CGXByteBuffer challenge;
            if ((ret = CGXSecure::GenerateChallenge(m_Settings.GetAuthentication(), challenge)) != 0)
            {
                return ret;
            }
            m_Settings.SetStoCChallenge(challenge);
            result = DLMS_ASSOCIATION_RESULT_ACCEPTED;
            diagnostic = DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED;
        }
    }

    // Generate AARE packet.
    m_Settings.ResetFrameSequence();
    return CGXAPDU::GenerateAARE(m_Settings, m_ReplyData, result, diagnostic, m_Settings.GetCipher());
}

/**
 * Parse SNRM Request. If server do not accept client empty byte array is
 * returned.
 *
 * @return Returns returned UA packet.
 */
int CGXDLMSServer::HandleSnrmRequest(CGXDLMSSettings& settings, CGXByteBuffer& reply)
{
    int ret;
    unsigned long serverAddress, clientAddress;
    serverAddress = m_Settings.GetServerAddress();
    clientAddress = m_Settings.GetClientAddress();
    Reset();
    m_Settings.SetServerAddress(serverAddress);
    m_Settings.SetClientAddress(clientAddress);

    reply.SetUInt8(0x81); // FromatID
    reply.SetUInt8(0x80); // GroupID
    reply.SetUInt8(0); // Length
    reply.SetUInt8(HDLC_INFO_MAX_INFO_TX);
    reply.SetUInt8(settings.GetLimits().GetMaxInfoTX().GetSize());
    if ((ret = settings.GetLimits().GetMaxInfoTX().GetBytes(reply)) != 0)
    {
        return ret;
    }

    reply.SetUInt8(HDLC_INFO_MAX_INFO_RX);
    reply.SetUInt8(settings.GetLimits().GetMaxInfoRX().GetSize());
    if ((ret = settings.GetLimits().GetMaxInfoRX().GetBytes(reply)) != 0)
    {
        return ret;
    }

    reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
    reply.SetUInt8(settings.GetLimits().GetWindowSizeTX().GetSize());
    if ((ret = settings.GetLimits().GetWindowSizeTX().GetBytes(reply)) != 0)
    {
        return ret;
    }
    reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
    reply.SetUInt8(settings.GetLimits().GetWindowSizeRX().GetSize());
    if ((ret = settings.GetLimits().GetWindowSizeRX().GetBytes(reply)) != 0)
    {
        return ret;
    }
    int len = reply.GetSize() - 3;
    reply.SetUInt8(2, len); // Length
    return ret;
}

/**
 * Generates disconnect request.
 *
 * @return Disconnect request.
 */
int GenerateDisconnectRequest(CGXDLMSSettings& settings, CGXByteBuffer& reply)
{
    int ret;
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        reply.SetUInt8(0x63);
        reply.SetUInt8(0x0);
    }
    else
    {
        reply.SetUInt8(0x81); // FromatID
        reply.SetUInt8(0x80); // GroupID
        reply.SetUInt8(0); // Length

        reply.SetUInt8(HDLC_INFO_MAX_INFO_TX);
        reply.SetUInt8(settings.GetLimits().GetMaxInfoTX().GetSize());
        if ((ret = settings.GetLimits().GetMaxInfoTX().GetBytes(reply)) != 0)
        {
            return ret;
        }

        reply.SetUInt8(HDLC_INFO_MAX_INFO_RX);
        reply.SetUInt8(settings.GetLimits().GetMaxInfoRX().GetSize());
        if ((ret = settings.GetLimits().GetMaxInfoRX().GetBytes(reply)) != 0)
        {
            return ret;
        }

        reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_TX);
        reply.SetUInt8(settings.GetLimits().GetWindowSizeTX().GetSize());
        if ((ret = settings.GetLimits().GetWindowSizeTX().GetBytes(reply)) != 0)
        {
            return ret;
        }

        reply.SetUInt8(HDLC_INFO_WINDOW_SIZE_RX);
        reply.SetUInt8(settings.GetLimits().GetWindowSizeRX().GetSize());
        if ((ret = settings.GetLimits().GetWindowSizeRX().GetBytes(reply)) != 0)
        {
            return ret;
        }

        int len = reply.GetSize() - 3;
        reply.SetUInt8(2, len); // Length.
    }
    return 0;
}

int ReportError(CGXDLMSSettings& settings, DLMS_COMMAND command, DLMS_ERROR_CODE error, CGXByteBuffer& reply)
{
    DLMS_COMMAND cmd;
    CGXByteBuffer data;
    switch (command)
    {
    case DLMS_COMMAND_READ_REQUEST:
        cmd = DLMS_COMMAND_READ_RESPONSE;
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        cmd = DLMS_COMMAND_WRITE_RESPONSE;
        break;
    case DLMS_COMMAND_GET_REQUEST:
        cmd = DLMS_COMMAND_GET_RESPONSE;
        break;
    case DLMS_COMMAND_SET_REQUEST:
        cmd = DLMS_COMMAND_SET_RESPONSE;
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        cmd = DLMS_COMMAND_METHOD_RESPONSE;
        break;
    default:
        // Return HW error and close connection.
        cmd = DLMS_COMMAND_NONE;
        break;
    }
    if (settings.GetUseLogicalNameReferencing())
    {
        CGXByteBuffer empty;
        CGXDLMS::GetLNPdu(settings, cmd, 1, empty, data, error, false, true, NULL);
    }
    else
    {
        CGXByteBuffer bb;
        bb.SetUInt8(error);
        CGXDLMS::GetSNPdu(settings, cmd, bb, data);
    }
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        return CGXDLMS::GetWrapperFrame(settings, data, reply);
    }
    else
    {
        return CGXDLMS::GetHdlcFrame(settings, 0, &data, reply);
    }
}

int CGXDLMSServer::HandleSetRequest(CGXByteBuffer& data)
{
    CGXDLMSVariant value;
    unsigned char ch, type, index;
    int ret;
    CGXDataInfo i;
    CGXByteBuffer bb;
    // Get type.
    if ((ret = data.GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    DLMS_ERROR_CODE status = DLMS_ERROR_CODE_OK;
    // SetRequest normal
    if (type == 1)
    {
        m_Settings.ResetBlockIndex();
        // CI
        unsigned short tmp;
        if ((ret = data.GetUInt16(&tmp)) != 0)
        {
            return ret;
        }
        DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE) tmp;
        CGXByteBuffer ln;
        ln.Set(&data, data.GetPosition(), 6);
        // Attribute index.
        if ((ret = data.GetUInt8(&index)) != 0)
        {
            return ret;
        }
        // Get Access Selection.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if ((ret = GXHelpers::GetData(data, i, value)) != 0)
        {
            return ret;
        }
        CGXDLMSObject* obj = m_Settings.GetObjects().FindByLN(ci, ln);
        if (obj == NULL)
        {
            std::string name;
            GXHelpers::GetLogicalName(ln, name);
            obj = FindObject(ci, 0, name);
        }
        // If target is unknown.
        if (obj == NULL)
        {
            // Device reports a undefined object.
            status = DLMS_ERROR_CODE_UNAVAILABLE_OBJECT;
        }
        else
        {
            DLMS_ACCESS_MODE am = obj->GetAccess(index);
            // If write is denied.
            if (am != DLMS_ACCESS_MODE_WRITE && am != DLMS_ACCESS_MODE_READ_WRITE)
            {
                // Read Write denied.
                status = DLMS_ERROR_CODE_READ_WRITE_DENIED;
            }
            else
            {
                if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                {
                    DLMS_DATA_TYPE dt;
                    if ((ret = (obj)->GetDataType(index, dt)) != 0)
                    {
                        return ret;
                    }
                    if (dt != DLMS_DATA_TYPE_NONE)
                    {
                        CGXByteBuffer tmp;
                        tmp.Set(value.byteArr, value.GetSize());
                        value.Clear();
                        if ((ret = CGXDLMSClient::ChangeType(tmp, dt, value)) != 0)
                        {
                            return ret;
                        }
                    }
                }
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(obj, index);
                e->SetValue(value);
                CGXDLMSValueEventCollection arr;
                arr.push_back(e);
                Write(arr);
                if (!e->GetHandled())
                {
                    obj->SetValue(m_Settings, *e);
                }
            }
        }
    }
    else
    {
        //HandleSetRequest failed. Unknown command.
        m_Settings.ResetBlockIndex();
        status = DLMS_ERROR_CODE_HARDWARE_FAULT;
    }
    return CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_SET_RESPONSE, 1, bb, m_ReplyData, status, false, true, NULL);
}

int CGXDLMSServer::HandleGetRequest(
    CGXByteBuffer& data)
{
    DLMS_ERROR_CODE error = DLMS_ERROR_CODE_OK;
    CGXDLMSVariant value;
    CGXDLMSValueEventCollection arr;
    unsigned long blockIndex;
    unsigned char type, ch, index;
    int ret;
    CGXByteBuffer bb;
    // Get type.
    if ((ret = data.GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // GetRequest normal
    if (type == 1)
    {
        m_Settings.SetCount(0);
        m_Settings.SetIndex (0);
        m_Settings.ResetBlockIndex();
        // CI
        unsigned short tmp;
        if ((ret = data.GetUInt16(&tmp)) != 0)
        {
            return ret;
        }
        DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE) tmp;
        CGXByteBuffer ln;
        ln.Set(&data, data.GetPosition(), 6);
        // Attribute Id
        if ((ret = data.GetUInt8(&index)) != 0)
        {
            return ret;
        }
        CGXDLMSObject* obj = m_Settings.GetObjects().FindByLN(ci, ln);
        if (obj == NULL)
        {
            std::string name;
            GXHelpers::GetLogicalName(ln, name);
            obj = FindObject(ci, 0, name);
        }
        if (obj == NULL)
        {
            // "Access Error : Device reports a undefined object."
            error = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
        }
        else
        {
            // AccessSelection
            unsigned char selection, selector = 0;
            if ((ret = data.GetUInt8(&selection)) != 0)
            {
                return ret;
            }
            CGXDLMSVariant parameters;
            if (selection != 0)
            {
                if ((ret = data.GetUInt8(&selector)) != 0)
                {
                    return ret;
                }
                CGXDataInfo i ;
                if ((ret = GXHelpers::GetData(data, i, parameters)) != 0)
                {
                    return ret;
                }
            }

            CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(obj, index, selector, parameters);
            arr.push_back(e);
            Read(arr);
            if (!e->GetHandled())
            {
                if ((ret = obj->GetValue(m_Settings, *e)) != 0)
                {
                    error = DLMS_ERROR_CODE_HARDWARE_FAULT;
                }
            }
            if (error == 0)
            {
                error = e->GetError();
            }
            value = e->GetValue();
            if (e->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                // If byte array is added do not add type.
                bb.Set(value.byteArr, value.GetSize());
            }
            else if ((ret = CGXDLMS::AppendData(obj, index, bb, value)) != 0)
            {
                error = DLMS_ERROR_CODE_HARDWARE_FAULT;
            }
        }
        if (m_Settings.GetCount() != m_Settings.GetIndex()
                || CGXDLMS::MultipleBlocks(m_Settings, bb, 0))
        {
            if ((ret = CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, 2, bb,
                                         m_ReplyData, error, true, false, NULL)) != 0)
            {
                return ret;
            }
            m_Transaction = new CGXDLMSLongTransaction(arr, DLMS_COMMAND_GET_REQUEST, bb);
            arr.clear();
        }
        else
        {
            if ((ret = CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, 1, bb,
                                         m_ReplyData, error, false, false, NULL)) != 0)
            {
                return ret;
            }
        }
    }
    else if (type == 2)
    {
        // Get request for next data block
        // Get block index.
        if ((ret = data.GetUInt32(&blockIndex)) != 0)
        {
            return ret;
        }
        if (blockIndex != m_Settings.GetBlockIndex())
        {
            // Invalid block number
            if ((ret = CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, 2, bb, m_ReplyData,
                                         DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID, true,
                                         true, NULL)) != 0)
            {
                return ret;
            }
        }
        else
        {
            m_Settings.IncreaseBlockIndex();
            // If transaction is not in progress.
            if (m_Transaction == NULL)
            {
                if ((ret = CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, 2, bb,
                                             m_ReplyData, DLMS_ERROR_CODE_NO_LONG_GET_OR_READ_IN_PROGRESS,
                                             true, true, NULL)) != 0)
                {
                    return ret;
                }
            }
            else
            {
                bb.Set(&m_Transaction->GetData());
                bool moreData = false;
                if (m_Settings.GetIndex() != m_Settings.GetCount())
                {
                    // If there is multiple blocks on the buffer.
                    // This might happen when Max PDU size is very small.
                    if (CGXDLMS::MultipleBlocks(m_Settings, bb, 0))
                    {
                        moreData = true;
                    }
                    else
                    {
                        for (std::vector<CGXDLMSValueEventArg*>::iterator arg = m_Transaction->GetTargets().begin();
                                arg != m_Transaction->GetTargets().end(); ++arg)
                        {
                            if (!(*arg)->GetHandled())
                            {
                                if ((ret = (*arg)->GetTarget()->GetValue(m_Settings, *(*arg))) != 0)
                                {
                                    return ret;
                                }
                            }
                            value = (*arg)->GetValue();
                            // Add data.
                            CGXDLMS::AppendData((*arg)->GetTarget(), (*arg)->GetIndex(), bb, value);
                            moreData = m_Settings.GetIndex() != m_Settings.GetCount();
                        }
                    }
                }
                else
                {
                    moreData = CGXDLMS::MultipleBlocks(m_Settings, bb, 0);
                }
                if ((ret = CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, 2, bb,
                                             m_ReplyData, DLMS_ERROR_CODE_OK, true, !moreData,
                                             NULL)) != 0)
                {
                    return ret;
                }
                if (moreData || bb.GetSize() - bb.GetPosition() != 0)
                {
                    m_Transaction->SetData(bb);
                }
                else
                {
                    if (m_Transaction != NULL)
                    {
                        delete (m_Transaction);
                        m_Transaction = NULL;
                    }
                }
            }
        }
    }
    else if (type == 3)
    {
        // Get request with a list.
        unsigned long cnt;
        unsigned short id;
        unsigned long pos;
        if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
        {
            return ret;
        }
        GXHelpers::SetObjectCount(cnt, bb);
        CGXDLMSValueEventCollection list;
        for (pos = 0; pos != cnt; ++pos)
        {
            if ((ret = data.GetUInt16(&id)) != 0)
            {
                return ret;
            }
            DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE)id;
            CGXByteBuffer ln;
            ln.Set(&data, data.GetPosition(), 6);
            if ((ret = data.GetUInt8(&index)) != 0)
            {
                return ret;
            }
            CGXDLMSObject* obj = m_Settings.GetObjects().FindByLN(ci, ln);
            if (obj == NULL)
            {
                std::string name;
                GXHelpers::GetLogicalName(ln, name);
                obj = FindObject(ci, 0, name);
            }
            if (obj == NULL)
            {
                // "Access Error : Device reports a undefined object."
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(obj, index);
                e->SetError(DLMS_ERROR_CODE_UNDEFINED_OBJECT);
                list.push_back(e);
            }
            else
            {
                // AccessSelection
                unsigned char selection, selector = 0;
                if ((ret = data.GetUInt8(&selection)) != 0)
                {
                    return ret;
                }
                CGXDLMSVariant parameters;
                if (selection != 0)
                {
                    if ((ret = data.GetUInt8(&selection)) != 0)
                    {
                        return ret;
                    }
                    CGXDataInfo i;
                    if ((ret = GXHelpers::GetData(data, i, parameters)) != 0)
                    {
                        return ret;
                    }
                }
                CGXDLMSValueEventArg* arg = new CGXDLMSValueEventArg(obj, index, selector, parameters);
                list.push_back(arg);
            }
        }
        Read(list);
        CGXDLMSVariant value;
        pos = 0;
        for (std::vector<CGXDLMSValueEventArg*>::iterator it = list.begin(); it != list.end(); ++it)
        {
            if ((*it)->GetHandled())
            {
                value = (*it)->GetValue();
            }
            else
            {
                if ((ret = (*it)->GetTarget()->GetValue(m_Settings, *(*it))) != 0)
                {
                    return ret;
                }
                value = (*it)->GetValue();
            }
            bb.SetUInt8((*it)->GetError());
            CGXDLMS::AppendData((*it)->GetTarget(), (*it)->GetIndex(), bb, value);
            if (m_Settings.GetIndex() != m_Settings.GetCount())
            {
                CGXByteBuffer empty;
                m_Transaction = new CGXDLMSLongTransaction(list, DLMS_COMMAND_GET_REQUEST, empty);
            }
            pos++;
        }
        if ((ret = CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, 3, bb, m_ReplyData,
                                     0xFF, CGXDLMS::MultipleBlocks(m_Settings, bb, 0), true, NULL)) != 0)
        {
            return ret;
        }
    }
    else
    {
        // HandleGetRequest failed. Invalid command type.
        m_Settings.ResetBlockIndex();
        // Access Error : Device reports a hardware fault.
        bb.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
        CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_GET_RESPONSE, type, bb, m_ReplyData,
                          DLMS_ERROR_CODE_OK, false, false, NULL);
    }
    return 0;
}

/**
 * Find Short Name object.
 *
 * @param sn
 */
int CGXDLMSServer::FindSNObject(int sn, CGXSNInfo& i)
{
    unsigned char offset, count;
    for (CGXDLMSObjectCollection::iterator it = GetItems().begin(); it != GetItems().end(); ++it)
    {
        if (sn >= (*it)->GetShortName())
        {
            // If attribute is accessed.
            if (sn < (*it)->GetShortName() + (*it)->GetAttributeCount() * 8)
            {
                i.SetAction(false);
                i.SetItem(*it);
                i.SetIndex(((sn - (*it)->GetShortName()) / 8) + 1);
                break;
            }
            else
            {
                // If method is accessed.
                CGXDLMS::GetActionInfo((*it)->GetObjectType(), offset, count);
                if (sn < (*it)->GetShortName() + offset + (8 * count))
                {
                    i.SetItem(*it);
                    i.SetAction(true);
                    i.SetIndex((sn - (*it)->GetShortName() - offset) / 8 + 1);
                    break;
                }
            }
        }
    }
    if (i.GetItem() == NULL)
    {
        std::string ln;
        i.SetItem(FindObject(DLMS_OBJECT_TYPE_NONE, sn, ln));
    }
    return 0;
}

/**
 * Handle read request.
 */
int CGXDLMSServer::HandleReadRequest(CGXByteBuffer& data)
{
    int ret;
    unsigned char type;
    unsigned long cnt, pos;
    unsigned short sn;
    CGXDLMSVariant value;
    CGXDLMSValueEventCollection list;
    CGXByteBuffer bb;
    // If get next frame.
    if (data.GetSize() == 0)
    {
        bb.Set(&m_ReplyData);
        m_ReplyData.Clear();
        for (std::vector<CGXDLMSValueEventArg*>::iterator it = m_Transaction->GetTargets().begin();
                it != m_Transaction->GetTargets().end(); ++it)
        {
            list.push_back(*it);
        }
    }
    else
    {
        m_Transaction = NULL;
        if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
        {
            return ret;
        }
        GXHelpers::SetObjectCount(cnt, bb);
        CGXSNInfo info;
        std::vector<CGXDLMSValueEventArg*> reads;
        std::vector<CGXDLMSValueEventArg*> actions;
        for (pos = 0; pos != cnt; ++pos)
        {
            if ((ret = data.GetUInt8(&type)) != 0)
            {
                return ret;
            }
            if (type == 2)
            {
                // GetRequest normal
                if ((ret = data.GetUInt16(&sn)) != 0)
                {
                    return ret;
                }
                if ((ret = FindSNObject(sn, info)) != 0)
                {
                    return ret;
                }
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(info.GetItem(), info.GetIndex());
                e->SetAction(info.IsAction());
                list.push_back(e);
                if (e->IsAction())
                {
                    actions.push_back(e);
                }
                else
                {
                    reads.push_back(e);
                }
            }
            else if (type == 4)
            {
                // Parameterised access.
                unsigned short sn;
                unsigned char selector;
                if ((ret = data.GetUInt16(&sn)) != 0)
                {
                    return ret;
                }
                if ((ret = data.GetUInt8(&selector)) != 0)
                {
                    return ret;
                }
                CGXDataInfo di;
                CGXDLMSVariant parameters;
                if ((ret = GXHelpers::GetData(data, di, parameters)) != 0)
                {
                    return ret;
                }
                if ((ret = FindSNObject(sn, info)) != 0)
                {
                    return ret;
                }
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(info.GetItem(), info.GetIndex(), selector, parameters);
                e->SetAction(info.IsAction());
                list.push_back(e);
                if (e->IsAction())
                {
                    actions.push_back(e);
                }
                else
                {
                    reads.push_back(e);
                }
            }
        }
        if (reads.size() != 0)
        {
            Read(reads);
        }
        if (actions.size() != 0)
        {
            Action(actions);
        }
    }
    for (std::vector<CGXDLMSValueEventArg*>::iterator e1 = list.begin(); e1 != list.end(); ++e1)
    {
        if ((*e1)->GetHandled())
        {
            value = (*e1)->GetValue();
        }
        else
        {
            // If action.
            if ((*e1)->IsAction())
            {
                if ((ret !=  (*e1)->GetTarget()->Invoke(m_Settings, *(*e1))) != 0)
                {
                    (*e1)->SetError(DLMS_ERROR_CODE_HARDWARE_FAULT);
                }
            }
            else
            {
                if ((ret = (*e1)->GetTarget()->GetValue(m_Settings, *(*e1))) != 0)
                {
                    (*e1)->SetError(DLMS_ERROR_CODE_HARDWARE_FAULT);
                }
            }
            value = (*e1)->GetValue();
        }
        if ((*e1)->GetError() == DLMS_ERROR_CODE_OK)
        {
            // Set status.
            if (m_Transaction == NULL)
            {
                bb.SetUInt8((*e1)->GetError());
            }
            // If action.
            if ((*e1)->IsAction())
            {
                if ((*e1)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                {
                    // If byte array is added do not add type.
                    bb.Set(value.byteArr, value.GetSize());
                }
                else if ((ret = GXHelpers::SetData(bb, value.vt, value)) != 0)
                {
                    return ret;
                }
            }
            else
            {
                if ((*e1)->IsByteArray() && value.vt == DLMS_DATA_TYPE_OCTET_STRING)
                {
                    // If byte array is added do not add type.
                    bb.Set(value.byteArr, value.GetSize());
                }
                else if ((ret = CGXDLMS::AppendData((*e1)->GetTarget(),
                                                    (*e1)->GetIndex(), bb, value)) != 0)
                {
                    return ret;
                }
            }
        }
        else
        {
            bb.SetUInt8(1);
            bb.SetUInt8((*e1)->GetError());
        }
        if (m_Transaction == NULL && m_Settings.GetCount() != m_Settings.GetIndex())
        {
            m_Transaction = new CGXDLMSLongTransaction(list, DLMS_COMMAND_READ_REQUEST, bb);
            list.clear();
        }
        else if (m_Transaction != NULL)
        {
            m_ReplyData.Set(&bb, bb.GetPosition());
            return 0;
        }
    }
    return CGXDLMS::GetSNPdu(m_Settings, DLMS_COMMAND_READ_RESPONSE, bb, m_ReplyData);
}

int CGXDLMSServer::HandleWriteRequest(CGXByteBuffer& data)
{
    int ret;
    unsigned short sn;
    unsigned char type, ch;
    unsigned long cnt, pos;
    CGXDLMSVariant value;
    // Get object count.
    std::vector<CGXSNInfo> targets;
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    CGXByteBuffer results(cnt);
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = data.GetUInt8(&type)) != 0)
        {
            return ret;
        }
        if (type == 2)
        {
            if ((ret = data.GetUInt16(&sn)) != 0)
            {
                return ret;
            }
            CGXSNInfo i;
            if ((ret = FindSNObject(sn, i)) != 0)
            {
                return ret;
            }
            targets.push_back(i);
            // If target is unknown.
            if (i.GetItem() == NULL)
            {
                // Device reports a undefined object.
                results.SetUInt8(DLMS_ERROR_CODE_UNDEFINED_OBJECT);
            }
            else
            {
                results.SetUInt8(DLMS_ERROR_CODE_OK);
            }
        }
        else
        {
            // Device reports a HW error.
            results.SetUInt8(DLMS_ERROR_CODE_HARDWARE_FAULT);
        }
    }
    // Get data count.
    if ((ret = GXHelpers::GetObjectCount(data, cnt)) != 0)
    {
        return ret;
    }
    CGXDataInfo di;
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = results.GetUInt8(pos, &ch)) != 0)
        {
            return ret;
        }
        if (ch == 0)
        {
            // If object has found.
            CGXSNInfo target = targets.at(pos);
            if ((ret = GXHelpers::GetData(data, di, value)) != 0)
            {
                return ret;
            }
            if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
            {
                DLMS_DATA_TYPE dt;
                if ((ret = target.GetItem()->GetDataType(target.GetIndex(), dt)) != 0)
                {
                    return ret;
                }
                if (dt != DLMS_DATA_TYPE_NONE && dt != DLMS_DATA_TYPE_OCTET_STRING)
                {
                    CGXByteBuffer bb;
                    bb.Set(value.byteArr, value.GetSize());
                    value.Clear();
                    if ((ret = CGXDLMSClient::ChangeType(bb, dt, value)) != 0)
                    {
                        return ret;
                    }
                }
            }
            di.Clear();
            DLMS_ACCESS_MODE am = target.GetItem()->GetAccess(target.GetIndex());
            // If write is denied.
            if (am != DLMS_ACCESS_MODE_WRITE && am != DLMS_ACCESS_MODE_READ_WRITE)
            {
                results.SetUInt8(pos,
                                 DLMS_ERROR_CODE_READ_WRITE_DENIED);
            }
            else
            {
                CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(target.GetItem(), target.GetIndex());
                e->SetValue(value);
                CGXDLMSValueEventCollection arr;
                arr.push_back(e);
                Write(arr);
                if (!e->GetHandled())
                {
                    target.GetItem()->SetValue(m_Settings, *e);
                }
            }
        }
    }
    CGXByteBuffer bb((2 * cnt + 2));
    GXHelpers::SetObjectCount(cnt, bb);
    unsigned char val;
    for (pos = 0; pos != cnt; ++pos)
    {
        if ((ret = results.GetUInt8(&val)) != 0)
        {
            return ret;
        }
        // If meter returns error.
        if (val != 0)
        {
            bb.SetUInt8(1);
        }
        bb.SetUInt8(val);
    }
    return CGXDLMS::GetSNPdu(m_Settings, DLMS_COMMAND_WRITE_RESPONSE, bb, m_ReplyData);
}

int CGXDLMSServer::HandleCommand(
    DLMS_COMMAND cmd,
    CGXByteBuffer& data,
    CGXByteBuffer& reply)
{
    int ret = 0;
    unsigned char frame = 0;
    switch (cmd)
    {
    case DLMS_COMMAND_SET_REQUEST:
        ret = HandleSetRequest(data);
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
        ret = HandleWriteRequest(data);
        break;
    case DLMS_COMMAND_GET_REQUEST:
        if (data.GetSize() != 0)
        {
            ret = HandleGetRequest(data);
        }
        break;
    case DLMS_COMMAND_READ_REQUEST:
        ret = HandleReadRequest(data);
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        ret = HandleMethodRequest(data);
        break;
    case DLMS_COMMAND_SNRM:
        ret = HandleSnrmRequest(m_Settings, m_ReplyData);
        frame = DLMS_COMMAND_UA;
        break;
    case DLMS_COMMAND_AARQ:
        ret = HandleAarqRequest(data);
        m_Settings.SetConnected(true);
        Connected();
        break;
    case DLMS_COMMAND_DISCONNECT_REQUEST:
    case DLMS_COMMAND_DISC:
        ret = GenerateDisconnectRequest(m_Settings, m_ReplyData);
        m_Settings.SetConnected(false);
        Disconnected();
        frame = DLMS_COMMAND_UA;
        break;
    case DLMS_COMMAND_NONE:
        //Get next frame.
        break;
    default:
        //Invalid command.
        break;
    }

    if (ret == 0)
    {
        if (m_Settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        {
            ret = CGXDLMS::GetWrapperFrame(m_Settings, m_ReplyData, reply);
        }
        else
        {
            ret = CGXDLMS::GetHdlcFrame(m_Settings, frame, &m_ReplyData, reply);
        }
    }
    return ret;
}

/**
 * Handle action request.
 *
 * @param reply
 *            Received data from the client.
 * @return Reply.
 */
int CGXDLMSServer::HandleMethodRequest(CGXByteBuffer& data)
{
    CGXByteBuffer bb;
    DLMS_ERROR_CODE error = DLMS_ERROR_CODE_OK;
    CGXDLMSVariant parameters;
    int ret;
    unsigned char ch, id;
    unsigned short tmp;
    // Get type.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // CI
    if ((ret = data.GetUInt16(&tmp)) != 0)
    {
        return ret;
    }
    DLMS_OBJECT_TYPE ci = (DLMS_OBJECT_TYPE) tmp;
    CGXByteBuffer ln;
    ln.Set(&data, data.GetPosition(), 6);
    // Attribute
    if ((ret = data.GetUInt8(&id)) != 0)
    {
        return ret;
    }
    // Get parameters.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }

    if (ch != 0)
    {
        CGXDataInfo i;
        if ((ret = GXHelpers::GetData(data, i, parameters)) != 0)
        {
            return ret;
        }
    }
    CGXDLMSObject* obj = m_Settings.GetObjects().FindByLN(ci, ln);
    if (obj == NULL)
    {
        std::string name;
        GXHelpers::GetLogicalName(ln, name);
        obj = FindObject(ci, 0, name);
    }
    if (obj == NULL)
    {
        error = DLMS_ERROR_CODE_UNDEFINED_OBJECT;
    }
    else
    {
        CGXDLMSValueEventArg* e = new CGXDLMSValueEventArg(obj, id, 0, parameters);
        CGXDLMSValueEventCollection arr;
        arr.push_back(e);
        Action(arr);
        CGXDLMSVariant actionReply;
        if (!e->GetHandled())
        {
            if ((ret = obj->Invoke(m_Settings, *e)) != 0)
            {
                return ret;
            }
        }
        actionReply = e->GetValue();
        // Set default action reply if not given.
        if (actionReply.vt != DLMS_DATA_TYPE_NONE || e->GetError() == DLMS_ERROR_CODE_OK)
        {
            // Add return parameters
            bb.SetUInt8(1);
            //Add parameters error code.
            bb.SetUInt8(0);
            GXHelpers::SetData(bb, actionReply.vt, actionReply);
        }
        else
        {
            // Add parameters error code.
            error = e->GetError();
            bb.SetUInt8(0);
        }
    }
    return CGXDLMS::GetLNPdu(m_Settings, DLMS_COMMAND_METHOD_RESPONSE, 1, bb,
                             m_ReplyData, error, false, true, NULL);
}

int CGXDLMSServer::HandleRequest(CGXByteBuffer& data, CGXByteBuffer& reply)
{
    return HandleRequest(
               data.GetData(),
               (unsigned short) (data.GetSize() - data.GetPosition()),
               reply);
}

int CGXDLMSServer::HandleRequest(unsigned char* buff, unsigned short size, CGXByteBuffer& reply)
{
    int ret;
    reply.Clear();
    if (buff == NULL || size == 0)
    {
        return 0;
    }
    if (!m_Initialized)
    {
        //Server not Initialized.
        return DLMS_ERROR_CODE_NOT_INITIALIZED;
    }
    m_ReceivedData.Set(buff, size);
    bool first = m_Settings.GetServerAddress() == 0
                 && m_Settings.GetClientAddress() == 0;
    if ((ret = CGXDLMS::GetData(m_Settings, m_ReceivedData, m_Info)) != 0)
    {
        return ret;
    }
    // If all data is not received yet.
    if (!m_Info.IsComplete())
    {
        return 0;
    }
    m_ReceivedData.Clear();

    if (first)
    {
        // Check is data send to this server.
        if (!IsTarget(m_Settings.GetServerAddress(), m_Settings.GetClientAddress()))
        {
            m_Info.Clear();
            return 0;
        }
    }
    // If client want next frame.
    if ((m_Info.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == DLMS_DATA_REQUEST_TYPES_FRAME)
    {
        return CGXDLMS::GetHdlcFrame(m_Settings, m_Settings.GetReceiverReady(), &m_ReplyData, reply);
    }
    // Update command if transaction and next frame is asked.
    if (m_Info.GetCommand() == DLMS_COMMAND_NONE)
    {
        if (m_Transaction != NULL)
        {
            m_Info.SetCommand(m_Transaction->GetCommand());
        }
    }
    ret = HandleCommand(m_Info.GetCommand(), m_Info.GetData(), reply);
    m_Info.Clear();
    return ret;
}
