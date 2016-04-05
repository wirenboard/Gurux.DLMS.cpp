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

#include "../include/GXDLMS.h"
#include "../include/GXAPDU.h"
#include "../include/GXDLMSClient.h"
#include "../include/GXDLMSObjectFactory.h"
#include "../include/GXBytebuffer.h"

unsigned short CGXDLMS::m_FCS16Table[256];

bool CGXDLMS::IsReplyMessage(DLMS_COMMAND cmd)
{
    return cmd == DLMS_COMMAND_GET_RESPONSE ||
           cmd == DLMS_COMMAND_SET_RESPONSE ||
           cmd == DLMS_COMMAND_METHOD_RESPONSE;
}

int CGXDLMS::GetAddress(long value, unsigned long& address, int& size)
{
    if (value < 0x80)
    {
        address =  (unsigned char) (value << 1 | 1);
        size = 1;
        return 0;
    }
    else if (value < 0x4000)
    {
        address =  (unsigned short) ((value & 0x3F80) << 2 | (value & 0x7F) << 1 | 1);
        size = 2;
    }
    else if (value < 0x10000000)
    {
        address =  (unsigned long) ((value & 0xFE00000) << 4 | (value & 0x1FC000) << 3
                                    | (value & 0x3F80) << 2 | (value & 0x7F) << 1 | 1);
        size = 4;
    }
    else
    {
        //Invalid address
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::CheckInit(CGXDLMSSettings& settings)
{
    if (settings.GetClientAddress() == 0)
    {
        return ERROR_CODES_INVALID_CLIENT_ADDRESS;
    }
    if (settings.GetServerAddress() == 0)
    {
        return ERROR_CODES_INVALID_SERVER_ADDRESS;
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::ReceiverReady(
    CGXDLMSSettings& settings,
    GXDLMS_DATA_REQUEST_TYPES type,
    CGXCipher* cipher,
    CGXByteBuffer& reply)
{
    int ret;
    std::vector<CGXByteBuffer> tmp;
    if (type == GXDLMS_DATA_REQUEST_TYPES_NONE)
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    // Get next frame.
    if ((type & GXDLMS_DATA_REQUEST_TYPES_FRAME) != 0)
    {
        int id = settings.GetReceiverReady();
        if ((ret = SplitToHdlcFrames(settings, id, NULL, tmp)) != 0)
        {
            return ret;
        }
        reply.AddRange2(&tmp.at(0), 0, -1);
        return ret;
    }
    // Get next block.
    CGXByteBuffer bb(6);
    bb.SetUInt32(settings.GetBlockIndex());
    settings.IncreaseBlockIndex();
    if (settings.IsServer())
    {
        if ((ret = SplitPdu(settings, DLMS_COMMAND_GET_RESPONSE,
                            2, bb, ERROR_CODES_OK, cipher, tmp)) != 0)
        {
            return ret;
        }
        reply.AddRange2(&tmp.at(0), 0, -1);
        return ret;
    }
    if ((ret = SplitPdu(settings, DLMS_COMMAND_GET_REQUEST,
                        2, bb, ERROR_CODES_OK, cipher, tmp)) != 0)
    {
        return ret;
    }
    reply.AddRange2(&tmp.at(0), 0, -1);
    return ret;
}

int CGXDLMS::SplitPdu(
    CGXDLMSSettings& settings,
    DLMS_COMMAND command,
    int commandParameter,
    CGXByteBuffer& data,
    ERROR_CODE error,
    CGXCipher* cp,
    std::vector<CGXByteBuffer>& reply)
{
    int ret;
    CGXByteBuffer bb;
    CGXByteBuffer tmp;
    // For SN there is no need to split data for blocks.
    if (!settings.GetUseLogicalNameReferencing())
    {
        if ((ret = GetSnPdus(settings, &data, command, tmp)) != 0)
        {
            return ret;
        }
        // If Ciphering is used.
        if (cp != NULL)
        {
            if ((ret = cp->Encrypt(command, tmp, bb)) != 0)
            {
                return ret;
            }
        }
        else
        {
            bb.AddRange2(&tmp, 0, -1);
        }
        if (settings.GetInterfaceType() == GXDLMS_INTERFACETYPE_HDLC)
        {
            if (command == DLMS_COMMAND_AARQ)
            {
                if ((ret = SplitToHdlcFrames(settings, 0x10, &bb, reply)) != 0)
                {
                    return ret;
                }
            }
            else
            {
                if ((ret = SplitToHdlcFrames(settings, 0, &bb, reply)) != 0)
                {
                    return ret;
                }
            }
        }
        else
        {
            SplitToWrapperFrames(settings, &bb, reply);
        }
    }
    else
    {
        std::vector<CGXByteBuffer> pdus;
        if ((ret = GetLnPdus(settings, commandParameter, &data, command, error, pdus)) != 0)
        {
            return ret;
        }
        for (std::vector<CGXByteBuffer>::iterator it = pdus.begin(); it != pdus.end(); ++it)
        {
            // If Ciphering is used.
            if (cp != NULL)
            {
                if ((ret = cp->Encrypt(command, *it, bb)) != 0)
                {
                    return ret;
                }
            }
            else
            {
                bb.AddRange2(&(*it), 0, -1);
            }
            if (settings.GetInterfaceType() == GXDLMS_INTERFACETYPE_HDLC)
            {
                if (command == DLMS_COMMAND_AARQ)
                {
                    if ((ret = SplitToHdlcFrames(settings, 0x10, &bb, reply)) != 0)
                    {
                        return ret;
                    }
                }
                else
                {
                    if ((ret = SplitToHdlcFrames(settings, 0, &bb, reply)) != 0)
                    {
                        return ret;
                    }
                }
            }
            else
            {
                SplitToWrapperFrames(settings, &bb, reply);
            }
            bb.Clear();
        }
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::SplitToHdlcFrames(
    CGXDLMSSettings& settings,
    int frame,
    CGXByteBuffer* data,
    std::vector<CGXByteBuffer>& packets)
{
    int ret, frameSize, len;
    CGXByteBuffer primaryAddress, secondaryAddress;
    if (settings.IsServer())
    {
        if ((ret = GetAddressBytes(settings.GetClientAddress(), primaryAddress)) != 0)
        {
            return ret;
        }
        if ((ret = GetAddressBytes(settings.GetServerAddress(), secondaryAddress)) != 0)
        {
            return ret;
        }
    }
    else
    {
        if ((ret = GetAddressBytes(settings.GetServerAddress(), primaryAddress)) != 0)
        {
            return ret;
        }
        if ((ret = GetAddressBytes(settings.GetClientAddress(), secondaryAddress)) != 0)
        {
            return ret;
        }
    }

    frameSize = settings.GetLimits().GetMaxInfoTX().ToInteger();
    // Remove header size.
    frameSize -= 7 + primaryAddress.GetSize() + secondaryAddress.GetSize();
    CGXByteBuffer bb;
    // Add LLC bytes.
    unsigned char* llcBytes = NULL;
    int llcByteSize = 0;
    if ((frame & 1) == 0)
    {
        llcByteSize = 3;
        if (settings.IsServer())
        {
            llcBytes = (unsigned char*) LLC_REPLY_BYTES;
        }
        else
        {
            llcBytes = (unsigned char*) LLC_SEND_BYTES;
        }
    }
    do
    {
        // Add BOP
        bb.SetUInt8(HDLC_FRAME_START_END);
        if (data == NULL || data->GetSize() == 0)
        {
            // If no data
            bb.SetUInt8(0xA0);
            // Skip data CRC from the total size.
            len = -2;
        }
        else if (data->GetSize() - data->GetPosition() + llcByteSize <= frameSize)
        {
            // Is last packet.
            bb.SetUInt8(0xA0);
            len = data->GetSize() - data->GetPosition();
        }
        else
        {
            // More data to left.
            bb.SetUInt8(0xA8);
            len = frameSize;
        }
        // Add length.
        bb.SetUInt8(7 + primaryAddress.GetSize() + secondaryAddress.GetSize() + len + llcByteSize);
        // Add primary address.
        bb.AddRange2(&primaryAddress, 0, -1);
        // Add secondary address.
        bb.AddRange2(&secondaryAddress, 0, -1);
        // Generate frame if not generated yet.
        if (frame == 0)
        {
            frame = settings.GetNextSend();
        }
        // Add frame.
        bb.SetUInt8(frame);
        // Add header CRC.
        unsigned short crc = CGXDLMS::CountFCS16(bb, 1, bb.GetSize() - 1);
        bb.SetUInt16(crc);
        // Add LLC bytes only once.
        if (llcByteSize != 0)
        {
            bb.AddRange(llcBytes, 3);
            llcByteSize = 0;
        }
        // Add data.
        if (data != NULL && data->GetSize() != 0)
        {
            bb.AddRange2(data, data->GetPosition(), len);
            data->SetPosition(data->GetPosition() + len);
            // Add data CRC.
            crc = CGXDLMS::CountFCS16(bb, 1, bb.GetSize() - 1);
            bb.SetUInt16(crc);
        }
        // Add EOP
        bb.SetUInt8(HDLC_FRAME_START_END);
        packets.push_back(bb);
        bb.Clear();
        frame = 0;
    }
    while (data != NULL && data->GetPosition() < data->GetSize());
    return ERROR_CODES_OK;
}

void CGXDLMS::SplitToWrapperFrames(
    CGXDLMSSettings& settings,
    CGXByteBuffer *data,
    std::vector<CGXByteBuffer>& reply)
{
    CGXByteBuffer bb;
    // Add version.
    bb.SetUInt16(1);
    if (settings.IsServer())
    {
        bb.SetUInt16(settings.GetServerAddress());
        bb.SetUInt16(settings.GetClientAddress());
    }
    else
    {
        bb.SetUInt16(settings.GetClientAddress());
        bb.SetUInt16(settings.GetServerAddress());
    }
    if (data == NULL)
    {
        // Data length.
        bb.SetUInt16(0);
    }
    else
    {
        // Data length.
        bb.SetUInt16(data->GetSize());
        // Data
        bb.AddRange2(data, 0, -1);
    }
    reply.push_back(bb);
}


int CGXDLMS::GetHdlcData(
    bool server,
    CGXDLMSSettings& settings,
    CGXByteBuffer& reply,
    CGXReplyData& data,
    unsigned char& frame)
{
    unsigned char ch;
    int ret, pos, packetStartID = reply.GetPosition(), frameLen = 0;
    unsigned short crc, crcRead;
    // If whole frame is not received yet.
    if (reply.GetSize() - reply.GetPosition() < 9)
    {
        data.SetComplete(false);
        return ERROR_CODES_OK;
    }
    data.SetComplete(true);
    // Find start of HDLC frame.
    for (pos = reply.GetPosition(); pos < reply.GetSize(); ++pos)
    {
        if ((ret = reply.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch == HDLC_FRAME_START_END)
        {
            packetStartID = pos;
            break;
        }
    }
    // Not a HDLC frame.
    if (reply.GetPosition() == reply.GetSize())
    {
        //Invalid data format
        return ERROR_CODES_INVALID_DATA_FORMAT;
    }
    if ((ret = reply.GetUInt8(&frame)) != 0)
    {
        return ret;
    }
    // Check frame length.
    if ((frame & 0x7) != 0)
    {
        frameLen = ((frame & 0x7) << 8);
    }
    if ((ret = reply.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // If not enough data.
    frameLen += ch;
    if (reply.GetSize() - reply.GetPosition() + 1 < frameLen)
    {
        data.SetComplete(false);
        reply.SetPosition(packetStartID);
        // Not enough data to parse;
        return ERROR_CODES_FALSE;

    }
    int eopPos = frameLen + packetStartID + 1;
    if ((ret = reply.GetUInt8(eopPos, &ch)) != 0)
    {
        return ret;
    }
    if (ch != HDLC_FRAME_START_END)
    {
        //Invalid data format
        return ERROR_CODES_INVALID_DATA_FORMAT;
    }

    // Check addresses.
    ret = CheckHdlcAddress(server, settings, reply, data, eopPos);
    if (ret == ERROR_CODES_FALSE)
    {
        // If echo.
        return GetHdlcData(server, settings, reply, data, frame);
    }
    if (ret != 0)
    {
        return ret;
    }
    // Is there more data available.
    if ((frame & 0x8) != 0)
    {
        data.SetMoreData((GXDLMS_DATA_REQUEST_TYPES) (data.GetMoreData() | GXDLMS_DATA_REQUEST_TYPES_FRAME));
    }
    else
    {
        data.SetMoreData((GXDLMS_DATA_REQUEST_TYPES)(data.GetMoreData() & ~GXDLMS_DATA_REQUEST_TYPES_FRAME));
    }
    // Get frame type.
    if ((ret = reply.GetUInt8(&frame)) != 0)
    {
        return ret;
    }
    if (!settings.CheckFrame(frame))
    {
        //Wrong frame index.
        return ERROR_CODES_INVALID_PARAMETER;
    }
    DLMS_FRAME_TYPE type = (DLMS_FRAME_TYPE) (frame & 0xFF);
    // Check that header CRC is correct.
    crc = CountFCS16(reply, packetStartID + 1, reply.GetPosition() - packetStartID - 1);
    if ((ret = reply.GetUInt16(&crcRead)) != 0)
    {
        return ret;
    }
    if (crc != crcRead)
    {
        return ERROR_CODES_WRONG_CRC;
    }
    // Check that packet CRC match only if there is a data part.
    if (reply.GetPosition() != packetStartID + frameLen + 1)
    {
        crc = CountFCS16(reply, packetStartID + 1, frameLen - 2);
        if ((ret = reply.GetUInt16(packetStartID + frameLen - 1, &crcRead)) != 0)
        {
            return ret;
        }
        if (crc != crcRead)
        {
            return ERROR_CODES_WRONG_CRC;
        }
    }
    if (type == DLMS_FRAME_TYPE_REJECTED)
    {
        // Get EOP.
        if ((ret = reply.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        data.SetError(ERROR_CODES_FRAME_REJECTED);
    }
    else if (type == DLMS_FRAME_TYPE_DISCONNECT_REQUEST)
    {
        // Get EOP.
        if ((ret = reply.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        data.SetCommand(DLMS_COMMAND_DISCONNECT_REQUEST);
    }
    else if (type == DLMS_FRAME_TYPE_DISCONNECT_MODE)
    {
        // Get EOP.
        if ((ret = reply.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        data.SetCommand(DLMS_COMMAND_DISCONNECT_REQUEST);
    }
    else if (type == DLMS_FRAME_TYPE_UA)
    {
        if (!settings.IsServer() && reply.GetPosition() == packetStartID + frameLen + 1)
        {
            // Get EOP.
            if ((ret = reply.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
        }
        data.SetCommand(DLMS_COMMAND_UA);
    }
    else if (type == DLMS_FRAME_TYPE_SNRM)
    {
        data.SetCommand(DLMS_COMMAND_SNRM);
        // Get EOP.
        if ((ret = reply.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
    }
    else
    {
        // If Keep Alive or get next frame
        if ((frame & 0x1) == 1)
        {
            // Get EOP.
            if ((ret = reply.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            data.SetMoreData(GXDLMS_DATA_REQUEST_TYPES_FRAME);
        }
        else
        {
            GetLLCBytes(server, reply);
            if (type == DLMS_FRAME_TYPE_INFORMATION && settings.IsServer())
            {
                data.SetCommand(DLMS_COMMAND_AARQ);
            }
        }
    }
    // Skip data CRC and EOP.
    if (reply.GetPosition() != reply.GetSize())
    {
        reply.SetSize(reply.GetSize() - 3);
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::GetHDLCAddress(
    CGXByteBuffer& buff,
    int& address)
{
    unsigned char ch;
    unsigned short s;
    unsigned long l;
    int ret, size = 0;
    address = 0;
    for (int pos = buff.GetPosition(); pos != buff.GetSize(); ++pos)
    {
        ++size;
        if ((ret = buff.GetUInt8(pos, &ch)) != 0)
        {
            return ret;
        }
        if ((ch & 0x1) == 1)
        {
            break;
        }
    }
    if (size == 1)
    {
        if ((ret = buff.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        address = ((ch & 0xFE) >> 1);
    }
    else if (size == 2)
    {
        if ((ret = buff.GetUInt16(&s)) != 0)
        {
            return ret;
        }
        address = ((s & 0xFE) >> 1) | ((s & 0xFE00) >> 2);
    }
    else if (size == 4)
    {
        if ((ret = buff.GetUInt32(&l)) != 0)
        {
            return ret;
        }
        address = ((l & 0xFE) >> 1) | ((l & 0xFE00) >> 2)
                  | ((l & 0xFE0000) >> 3) | ((l & 0xFE000000) >> 4);
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::CheckHdlcAddress(
    bool server,
    CGXDLMSSettings& settings,
    CGXByteBuffer& reply,
    CGXReplyData& data,
    int index)
{
    int ret, source, target;
    // Get destination and source addresses.
    if ((ret = GetHDLCAddress(reply, target)) != 0)
    {
        return ret;
    }
    if ((ret = GetHDLCAddress(reply, source)) != 0)
    {
        return ret;
    }
    if (server)
    {
        // Check that server addresses match.
        if (settings.GetServerAddress() != 0 && settings.GetServerAddress() != target)
        {
            return ERROR_CODES_INVALID_SERVER_ADDRESS;
        }
        else
        {
            data.SetServerAddress(target);
        }

        // Check that client addresses match.
        if (settings.GetClientAddress() != 0 && settings.GetClientAddress() != source)
        {
            return ERROR_CODES_INVALID_CLIENT_ADDRESS;
        }
        else
        {
            data.SetClientAddress(source);
        }
    }
    else
    {
        // Check that client addresses match.
        if (settings.GetClientAddress() != target)
        {
            // If echo.
            if (settings.GetClientAddress() == source && settings.GetServerAddress() == target)
            {
                reply.SetPosition(index + 1);
                return ERROR_CODES_FALSE;
            }
            return ERROR_CODES_INVALID_CLIENT_ADDRESS;
        }
        // Check that server addresses match.
        if (settings.GetServerAddress() != source)
        {
            return ERROR_CODES_INVALID_CLIENT_ADDRESS;
        }
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::HandleMethodResponse(
    CGXDLMSSettings& settings,
    CGXReplyData& data)
{
    int ret;
    unsigned char ch, type;
    // Get type.
    if ((ret = data.GetData().GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0)
    {
        data.SetError(ch);
    }
    if (type == 1)
    {
        // Response normal. Get data if exists.
        if (data.GetData().GetPosition() < data.GetData().GetSize())
        {
            if ((ret = data.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if (ch != 1)
            {
                //ParseApplicationAssociationResponse failed. Invalid tag.
                return ERROR_CODES_INVALID_TAG;
            }
            if ((ret = data.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if (ch != 0)
            {
                return ch;
            }
            return GetDataFromBlock(data.GetData(), 0);
        }
    }
    else
    {
        return ERROR_CODES_INVALID_COMMAND;
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::HandlePush(CGXReplyData& reply)
{
    unsigned char ch;
    int ret;
    int index = reply.GetData().GetPosition() - 1;
    // Is last block
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }

    if ((ch & 0x80) == 0)
    {
        reply.SetMoreData((GXDLMS_DATA_REQUEST_TYPES)(reply.GetMoreData() | GXDLMS_DATA_REQUEST_TYPES_BLOCK));
    }
    else
    {
        reply.SetMoreData((GXDLMS_DATA_REQUEST_TYPES) (reply.GetMoreData() & ~GXDLMS_DATA_REQUEST_TYPES_BLOCK));
    }
    // Get block number sent.
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Get block number acknowledged
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Get APU tag.
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Addl fields
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Data-Notification
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if ((ch & 0x0F) == 0)
    {
        //Invalid data.
        return ERROR_CODES_INVALID_PARAMETER;
    }
    // Long-Invoke-Id-And-Priority
    unsigned long ul;
    if ((ret = reply.GetData().GetUInt32(&ul)) != 0)
    {
        return ret;
    }
    // Get date time and skip it if used.
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0)
    {
        reply.GetData().SetPosition(reply.GetData().GetPosition() + ch);
    }
    return GetDataFromBlock(reply.GetData(), index);
}

int CGXDLMS::HandleSetResponse(
    CGXDLMSSettings& settings,
    CGXReplyData& data)
{
    //TODO:
    return ERROR_CODES_OK;
}

int CGXDLMS::GetPdu(
    CGXDLMSSettings& settings,
    CGXReplyData& data,
    CGXCipher* cipher)
{
    int ret = ERROR_CODES_OK;
    unsigned char ch;
    DLMS_COMMAND cmd = data.GetCommand();
    if (data.GetCommand() == DLMS_COMMAND_PUSH && (data.GetMoreData() & GXDLMS_DATA_REQUEST_TYPES_FRAME) == 0)
    {
        data.SetCommand(DLMS_COMMAND_NONE);
    }
    // If header is not read yet.
    if (data.GetCommand() == DLMS_COMMAND_NONE)
    {
        int index = data.GetData().GetPosition();
        // Get command.
        if ((ret = data.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        cmd = (DLMS_COMMAND)ch;
        data.SetCommand(cmd);
        switch (cmd)
        {
        case DLMS_COMMAND_READ_RESPONSE:
            if ((ret = HandleReadResponse(data)) != 0)
            {
                return ret;
            }
            break;
        case DLMS_COMMAND_GET_RESPONSE:
            if ((ret = HandleGetResponse(settings, data, index)) != 0)
            {
                return ret;
            }
            break;
        case DLMS_COMMAND_SET_RESPONSE:
            if ((ret = HandleSetResponse(settings, data)) != 0)
            {
                return ret;
            }
            break;
        case DLMS_COMMAND_WRITE_RESPONSE:
            if ((ret = HandleWriteResponse(settings, data)) != 0)
            {
                return ret;
            }
            break;
        case DLMS_COMMAND_METHOD_RESPONSE:
            if ((ret = HandleMethodResponse(settings, data)) != 0)
            {
                return ret;
            }
            break;
        case DLMS_COMMAND_PUSH:
            if ((ret = HandlePush(data)) != 0)
            {
                return ret;
            }
            break;
        case DLMS_COMMAND_AARQ:
        case DLMS_COMMAND_AARE:
            // This is parsed later.
            data.GetData().SetPosition(data.GetData().GetPosition() - 1);
            break;
        case DLMS_COMMAND_DISCONNECT_RESPONSE:
            break;
        case DLMS_COMMAND_EXCEPTION_RESPONSE:
            //TODO: throw new GXDLMSException( StateError.forValue(data.getData().getUInt8()), ServiceError.forValue(data.getData().getUInt8()));
            return ERROR_CODES_HARDWARE_FAULT;
        case DLMS_COMMAND_GET_REQUEST:
        case DLMS_COMMAND_READ_REQUEST:
        case DLMS_COMMAND_WRITE_REQUEST:
        case DLMS_COMMAND_SET_REQUEST:
        case DLMS_COMMAND_METHOD_REQUEST:
        case DLMS_COMMAND_DISCONNECT_REQUEST:
            // Server handles this.
            break;
        case DLMS_COMMAND_GLO_GET_REQUEST:
        case DLMS_COMMAND_GLO_SET_REQUEST:
        case DLMS_COMMAND_GLO_METHOD_REQUEST:
            if (cipher == NULL)
            {
                return ERROR_CODES_SECURITY_NOT_IMPLEMENTED;
            }
            data.GetData().SetPosition(data.GetData().GetPosition() - 1);
            if ((ret = cipher->Decrypt(data.GetData())) != 0)
            {
                return ret;
            }
            // Get command.
            if ((ret = data.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            cmd = (DLMS_COMMAND)ch;
            data.SetCommand(cmd);
            // Server handles this.
            break;
        default:
            return ERROR_CODES_INVALID_COMMAND;
        }
    }
    else if ((data.GetMoreData() & GXDLMS_DATA_REQUEST_TYPES_FRAME) == 0)
    {
        // Is whole block is read and if last packet and data is not try to
        // peek.
        if (!data.GetPeek() && data.GetMoreData() == GXDLMS_DATA_REQUEST_TYPES_NONE)
        {
            data.GetData().SetPosition(0);
            settings.ResetBlockIndex();
        }
        // Get command if operating as a server.
        if (settings.IsServer())
        {
            if ((ret = data.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
        }
        else
        {
            // Client do not need a command any more.
            data.SetCommand(DLMS_COMMAND_NONE);
        }
    }

    // Get data if all data is read or we want to peek data.
    if ((cmd == DLMS_COMMAND_READ_RESPONSE || cmd == DLMS_COMMAND_GET_RESPONSE)
            && (data.GetMoreData() == GXDLMS_DATA_REQUEST_TYPES_NONE || data.GetPeek()))
    {
        ret = GetValueFromData(settings, data);
    }
    return ret;
}


int CGXDLMS::GetData(CGXDLMSSettings& settings,
                     CGXByteBuffer& reply,
                     CGXReplyData& data,
                     CGXCipher* cipher)
{
    int ret;
    unsigned char frame = 0;
    // If DLMS frame is generated.
    if (settings.GetInterfaceType() == GXDLMS_INTERFACETYPE_HDLC)
    {
        if ((ret = GetHdlcData(settings.IsServer(), settings, reply, data, frame)) != 0)
        {
            return ret;
        }
    }
    else if (settings.GetInterfaceType() == GXDLMS_INTERFACETYPE_WRAPPER)
    {
        if ((ret = GetTcpData(settings, reply, data)) != 0)
        {
            return ret;
        }
    }
    else
    {
        // Invalid Interface type.
        return ERROR_CODES_INVALID_PARAMETER;
    }
    // If all data is not read yet.
    if (!data.IsComplete())
    {
        return ERROR_CODES_FALSE;
    }

    GetDataFromFrame(reply, data.GetData());

    // If keepalive or get next frame request.
    if ((frame & 0x1) != 0)
    {
        return ERROR_CODES_OK;
    }

    // Get data.
    if (frame != DLMS_FRAME_TYPE_SNRM
            && frame != DLMS_FRAME_TYPE_UA
            && data.GetCommand() != DLMS_COMMAND_AARQ
            && data.GetCommand() != DLMS_COMMAND_AARE
            && frame != DLMS_FRAME_TYPE_DISCONNECT_MODE)
    {
        return GetPdu(settings, data, cipher);
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::HandleGetResponse(
    CGXDLMSSettings& settings,
    CGXReplyData& reply,
    int index)
{
    int ret;
    unsigned long number;
    unsigned long count;
    unsigned char ch, type;
    // Get type.
    if ((ret = reply.GetData().GetUInt8(&type)) != 0)
    {
        return ret;
    }
    // Get invoke ID and priority.
    if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Response normal
    if (type == 1)
    {
        // Result
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            reply.SetError(ch);
        }
        if ((ret = GetDataFromBlock(reply.GetData(), 0)) != 0)
        {
            return ret;
        }
    }
    else if (type == 2)
    {
        // GetResponsewithDataBlock
        // Is Last block.
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch == 0)
        {
            reply.SetMoreData((GXDLMS_DATA_REQUEST_TYPES) (reply.GetMoreData() | GXDLMS_DATA_REQUEST_TYPES_BLOCK));
        }
        else
        {
            reply.SetMoreData((GXDLMS_DATA_REQUEST_TYPES) (reply.GetMoreData() & ~GXDLMS_DATA_REQUEST_TYPES_BLOCK));
        }
        // Get Block number.
        if ((ret = reply.GetData().GetUInt32(&number)) != 0)
        {
            return ret;
        }
        unsigned long expectedIndex = settings.GetBlockIndex();
        if (number != expectedIndex)
        {
            return ERROR_CODES_INVALID_BLOCK_NUMBER;
        }
        // Get status.
        if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            if ((ret = reply.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            reply.SetError(ch);
        }
        else
        {
            // Get data size.
            if ((ret = GXHelpers::GetObjectCount(reply.GetData(), count)) != 0)
            {
                return ret;
            }
            reply.SetBlockLength(count);
            // if whole block is read.
            if ((reply.GetMoreData() & GXDLMS_DATA_REQUEST_TYPES_FRAME) == 0)
            {
                // Check Block length.
                if (reply.GetBlockLength() > reply.GetData().GetSize() - reply.GetData().GetPosition())
                {
                    return ERROR_CODES_OUTOFMEMORY;
                }
                reply.SetCommand(DLMS_COMMAND_NONE);
            }
            if ((ret = GetDataFromBlock(reply.GetData(), index)) != 0)
            {
                return ret;
            }
            // If last packet and data is not try to peek.
            if (reply.GetMoreData() == GXDLMS_DATA_REQUEST_TYPES_NONE)
            {
                if (!reply.GetPeek())
                {
                    reply.GetData().SetPosition(0);
                    settings.ResetBlockIndex();
                }
            }
        }
    }
    else if (type == 3)
    {
        // Get response with list.
        if ((ret = GetDataFromBlock(reply.GetData(), 0)) != 0)
        {
            return ret;
        }
        return ERROR_CODES_FALSE;
    }
    else
    {
        // Invalid Get response.
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::HandleWriteResponse(
    CGXDLMSSettings& settings,
    CGXReplyData& data)
{
    unsigned char ch;
    int ret;
    unsigned long count;
    if ((ret = GXHelpers::GetObjectCount(data.GetData(), count)) != 0)
    {
        return ret;
    }
    for (unsigned long pos = 0; pos != count; ++pos)
    {
        if ((ret = data.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            if ((ret = data.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            data.SetError(ch);
            return ret;
        }
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::HandleReadResponse(
    CGXReplyData& data)
{
    unsigned long count;
    int ret;
    int pos = data.GetData().GetPosition();
    if ((ret = GXHelpers::GetObjectCount(data.GetData(), count)) != 0)
    {
        return ret;
    }
    if (count != 1)
    {
        data.GetData().SetPosition(pos);
        return ERROR_CODES_FALSE;
    }
    unsigned char ch;
    // Get status code.
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch == 0)
    {
        data.SetError((unsigned char) 0);
        if ((ret = GetDataFromBlock(data.GetData(), 0)) != 0)
        {
            return ret;
        }
    }
    else
    {
        // Get error code.
        if ((ret = data.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        data.SetError(ch);
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::GetTcpData(
    CGXDLMSSettings& settings,
    CGXByteBuffer& buff,
    CGXReplyData& data)
{
    int ret;
    // If whole frame is not received yet.
    if (buff.GetSize() - buff.GetPosition() < 8)
    {
        data.SetComplete(false);
        return ERROR_CODES_OK;
    }
    int pos = buff.GetPosition();
    unsigned short value;
    // Get version
    if ((ret = buff.GetUInt16(&value)) != 0)
    {
        return ret;
    }
    if (value != 1)
    {
        //Unknown version.
        return ERROR_CODES_INVALID_PARAMETER;
    }

    // Check TCP/IP addresses.
    CheckWrapperAddress(settings, buff, data);
    // Get length.
    if ((ret = buff.GetUInt16(&value)) != 0)
    {
        return ret;
    }
    bool compleate = !((buff.GetSize() - buff.GetPosition()) < value);
    data.SetComplete(compleate);
    if (!compleate)
    {
        buff.SetPosition(pos);
        return ERROR_CODES_FALSE;
    }
    return ERROR_CODES_OK;
}

int CGXDLMS::GetAddressBytes(unsigned long value, CGXByteBuffer& bytes)
{
    int ret;
    unsigned long address;
    int size;
    if ((ret = GetAddress(value, address, size)) != 0)
    {
        return ret;
    }
    if (size == 1)
    {
        bytes.Capacity(1);
        bytes.SetUInt8((unsigned char) address);
    }
    else if (size == 2)
    {
        bytes.Capacity(2);
        bytes.SetUInt16((unsigned short) address);
    }
    else if (size == 4)
    {
        bytes.Capacity(4);
        bytes.SetUInt32((unsigned long) address);
    }
    else
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    return ERROR_CODES_OK;
}

unsigned char CGXDLMS::GetInvokeIDPriority(CGXDLMSSettings& settings)
{
    unsigned char value = 0;
    if (settings.GetPriority() == GXDLMS_PRIORITY_HIGH)
    {
        value |= 0x80;
    }
    if (settings.GetServiceClass() == GXDLMS_SERVICECLASS_CONFIRMED)
    {
        value |= 0x40;
    }
    value |= settings.GetInvokeID();
    return value;
}

int CGXDLMS::GetLnPdus(CGXDLMSSettings& settings, int commandParameter,
                       CGXByteBuffer *buff, DLMS_COMMAND cmd,
                       ERROR_CODE error,  std::vector<CGXByteBuffer>& reply)
{
    int len;
    if (buff == NULL)
    {
        len = 0;
    }
    else
    {
        len = settings.GetMaxReceivePDUSize();
        if (settings.GetInterfaceType() == GXDLMS_INTERFACETYPE_HDLC)
        {
            len -= 7;
        }
        else
        {
            // Wrapper header size is 12.
            len -= 12;
        }
    }
    CGXByteBuffer bb(len);
    int index = settings.GetBlockIndex() - 1;
    bool multibleBlocks = buff->GetSize() > len;
    do
    {
        // If last packet.
        if (buff->GetSize() - buff->GetPosition() < len)
        {
            len = buff->GetSize() - buff->GetPosition();
        }
        if (cmd != DLMS_COMMAND_AARQ && cmd != DLMS_COMMAND_AARE)
        {
            // Add command.
            bb.SetUInt8(cmd);
            // If all data is not fit to one PDU.
            if (multibleBlocks)
            {
                bb.SetUInt8(2);
                // Add Invoke Id And Priority.
                bb.SetUInt8(GetInvokeIDPriority(settings));
                // Is last packet.
                if (buff->GetPosition() + len < buff->GetSize())
                {
                    bb.SetUInt8(0);
                }
                else
                {
                    bb.SetUInt8(1);
                }
                // Block index.
                bb.SetUInt32(++index);
                // Add error code if reply.
                if (IsReplyMessage(cmd))
                {
                    bb.SetUInt8(0);
                }
                // Block length.
                GXHelpers::SetObjectCount(len, bb);
            }
            else
            {
                bb.SetUInt8(commandParameter);
                // Add Invoke Id And Priority.
                bb.SetUInt8(GetInvokeIDPriority(settings));
                // Add error code if reply and not Get Response With List.
                if (IsReplyMessage(cmd) && !(cmd == DLMS_COMMAND_GET_RESPONSE
                                             && commandParameter == 3))
                {
                    bb.SetUInt8(error);
                }
            }
        }

        bb.AddRange2(buff, buff->GetPosition(), len);
        buff->SetPosition(buff->GetPosition() + len);
        reply.push_back(bb);
        bb.Clear();
    }
    while (buff->GetPosition() < buff->GetSize());
    return ERROR_CODES_OK;
}

int CGXDLMS::GetValueFromData(CGXDLMSSettings& settings, CGXReplyData& reply)
{
    int ret;
    CGXDataInfo info;
    if (reply.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
    {
        info.SetType(DLMS_DATA_TYPE_ARRAY);
        info.SetCount(reply.GetTotalCount());
        info.SetIndex(reply.GetCount());
    }
    CGXDLMSVariant value;
    int index = reply.GetData().GetPosition();
    reply.GetData().SetPosition(reply.GetReadPosition());
    if ((ret = GXHelpers::GetData(reply.GetData(), info, value)) != 0)
    {
        return ret;
    }
    // If new data.
    if (value.vt != DLMS_DATA_TYPE_ARRAY)
    {
        reply.SetValueType(info.GetType());
        reply.SetValue(value);
        reply.SetTotalCount(0);
    }
    else
    {
        if (value.Arr.size() != 0)
        {
            if (reply.GetValue().vt == DLMS_DATA_TYPE_NONE)
            {
                reply.SetValue(value);
            }
            else
            {
                CGXDLMSVariant tmp = reply.GetValue();
                tmp.Arr.insert(tmp.Arr.end(), value.Arr.begin(), value.Arr.end());
                reply.SetValue(tmp);
            }
        }
        reply.SetReadPosition(reply.GetData().GetPosition());
        // Element count.
        reply.SetTotalCount(info.GetCount());
    }
    reply.GetData().SetPosition(index);

    // If last data frame of the data block is read.
    if (reply.GetMoreData() == GXDLMS_DATA_REQUEST_TYPES_NONE)
    {
        // If all blocks are read.
        settings.ResetBlockIndex();
        reply.GetData().SetPosition(0);
    }
    return 0;
}

void CGXDLMS::GetDataFromFrame(CGXByteBuffer& reply, CGXByteBuffer& data)
{
    int offset = data.GetSize();
    int cnt = reply.GetSize() - reply.GetPosition();
    if (cnt != 0)
    {
        data.Capacity(offset + cnt);
        data.AddRange2(&reply, reply.GetPosition(), cnt);
    }
    // Set position to begin of new data.
    data.SetPosition(offset);
}

int CGXDLMS::GetDataFromBlock(CGXByteBuffer& data, int index)
{
    int len = data.GetPosition() - index;
    if (len < 0)
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
    data.Move(data.GetPosition(), data.GetPosition() - len, data.GetSize() - data.GetPosition());
    data.SetSize(data.GetSize() - len);
    data.SetPosition(data.GetPosition() - len);
    return 0;
}
int CGXDLMS::GetSnPdus(CGXDLMSSettings& settings, CGXByteBuffer* buff,
                       DLMS_COMMAND cmd, CGXByteBuffer& reply)
{
    if (buff != NULL && buff->GetSize() != 0)
    {
        // Add command.
        if (cmd != DLMS_COMMAND_AARQ && cmd != DLMS_COMMAND_AARE)
        {
            reply.SetUInt8(cmd);
        }
        // Add data.
        reply.AddRange2(buff, 0, -1);
    }
    return ERROR_CODES_OK;
}

void CGXDLMS::GetLLCBytes(bool server, CGXByteBuffer& data)
{
    if (server)
    {
        data.Compare((unsigned char*) LLC_SEND_BYTES, 3);
    }
    else
    {
        data.Compare((unsigned char*) LLC_REPLY_BYTES, 3);
    }
}

int CGXDLMS::CheckWrapperAddress(CGXDLMSSettings& settings,
                                 CGXByteBuffer& buff, CGXReplyData& data)
{
    int ret;
    unsigned short value;
    if (settings.IsServer())
    {
        if ((ret = buff.GetUInt16(&value)) != 0)
        {
            return ret;
        }
        // Check that client addresses match.
        if (settings.GetClientAddress() != 0
                && settings.GetClientAddress() != value)
        {
            return ERROR_CODES_INVALID_CLIENT_ADDRESS;
        }
        else
        {
            data.SetClientAddress(value);
        }

        if ((ret = buff.GetUInt16(&value)) != 0)
        {
            return ret;
        }
        // Check that server addresses match.
        if (settings.GetServerAddress() != 0
                && settings.GetServerAddress() != value)
        {
            return ERROR_CODES_INVALID_SERVER_ADDRESS;
        }
        else
        {
            data.SetServerAddress(value);
        }
    }
    else
    {
        if ((ret = buff.GetUInt16(&value)) != 0)
        {
            return ret;
        }
        // Check that server addresses match.
        if (settings.GetServerAddress() != 0
                && settings.GetServerAddress() != value)
        {
            return ERROR_CODES_INVALID_SERVER_ADDRESS;
        }
        else
        {
            data.SetServerAddress(value);
        }

        if ((ret = buff.GetUInt16(&value)) != 0)
        {
            return ret;
        }
        // Check that client addresses match.
        if (settings.GetClientAddress() != 0
                && settings.GetClientAddress() != value)
        {
            return ERROR_CODES_INVALID_CLIENT_ADDRESS;
        }
        else
        {
            data.SetClientAddress(value);
        }
    }
    return ERROR_CODES_OK;
}
void CGXDLMS::GenerateFCS16Table()
{
#define P       0x8408
    unsigned int b, v;
    int i;
    for (b = 0; ; )
    {
        v = b;
        for (i = 8; i--;)
        {
            v = v & 1 ? (v >> 1) ^ P : v >> 1;
        }
        m_FCS16Table[b] = v & 0xFFFF;
        if (++b == 256)
        {
            break;
        }
    }
}

unsigned short CGXDLMS::CountFCS16(CGXByteBuffer& buff, int index, int count)
{
    int ret;
    unsigned char ch;
    unsigned short fcs16 = 0xFFFF;
    for(short pos = 0; pos < count; ++pos)
    {
        if ((ret = buff.GetUInt8(index + pos, &ch)) != 0)
        {
            return ret;
        }
        fcs16 = (fcs16 >> 8) ^ m_FCS16Table[(fcs16 ^ ch) & 0xFF];
    }
    fcs16 = ~fcs16;
    fcs16 = ((fcs16 >> 8) & 0xFF) | (fcs16 << 8);
    return fcs16;
}

int CGXDLMS::GetActionInfo(OBJECT_TYPE objectType, int& value, int& count)
{
    switch (objectType)
    {
    case OBJECT_TYPE_DATA:
    case OBJECT_TYPE_ACTION_SCHEDULE:
    case OBJECT_TYPE_ALL:
    case OBJECT_TYPE_AUTO_ANSWER:
    case OBJECT_TYPE_AUTO_CONNECT:
    case OBJECT_TYPE_MAC_ADDRESS_SETUP:
    case OBJECT_TYPE_GPRS_SETUP:
    case OBJECT_TYPE_IEC_HDLC_SETUP:
    case OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
    case OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
    case OBJECT_TYPE_MODEM_CONFIGURATION:
    case OBJECT_TYPE_PPP_SETUP:
    case OBJECT_TYPE_REGISTER_MONITOR:
    case OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
    case OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
    case OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
    case OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL:
    case OBJECT_TYPE_SCHEDULE:
    case OBJECT_TYPE_SMTP_SETUP:
    case OBJECT_TYPE_STATUS_MAPPING:
    case OBJECT_TYPE_TCP_UDP_SETUP:
    case OBJECT_TYPE_UTILITY_TABLES:
        return ERROR_CODES_INVALID_PARAMETER;
    case OBJECT_TYPE_IMAGE_TRANSFER:
        value = 0x40;
        count = 4;
        break;
    case OBJECT_TYPE_ACTIVITY_CALENDAR:
        value = 0x50;
        count = 1;
        break;
    case OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        value = 0x60;
        count = 4;
        break;
    case OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        value = 0x20;
        count = 8;
        break;
    case OBJECT_TYPE_CLOCK:
        value = 0x60;
        count = 6;
        break;
    case OBJECT_TYPE_DEMAND_REGISTER:
        value = 0x48;
        count = 2;
        break;
    case OBJECT_TYPE_EXTENDED_REGISTER:
        value = 0x38;
        count = 1;
        break;
    case OBJECT_TYPE_IP4_SETUP:
        value = 0x60;
        count = 3;
        break;
    case OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        value = 0x60;
        count = 8;
        break;
    case OBJECT_TYPE_PROFILE_GENERIC:
        value = 0x58;
        count = 4;
        break;
    case OBJECT_TYPE_REGISTER:
        value = 0x28;
        count = 1;
        break;
    case OBJECT_TYPE_REGISTER_ACTIVATION:
        value = 0x30;
        count = 3;
        break;
    case OBJECT_TYPE_REGISTER_TABLE:
        value = 0x28;
        count = 2;
        break;
    case OBJECT_TYPE_SAP_ASSIGNMENT:
    case OBJECT_TYPE_SCRIPT_TABLE:
        value = 0x20;
        count = 1;
        break;
    case OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        value = 0x10;
        count = 2;
        break;
    default:
        count = value = 0;
        break;
    }
    return ERROR_CODES_OK;
}
