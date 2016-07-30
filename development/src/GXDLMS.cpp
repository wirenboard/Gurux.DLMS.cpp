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

//CRC table.
static unsigned short FCS16Table[256] =
{
    0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF,
    0x8C48, 0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7,
    0x1081, 0x0108, 0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E,
    0x9CC9, 0x8D40, 0xBFDB, 0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876,
    0x2102, 0x308B, 0x0210, 0x1399, 0x6726, 0x76AF, 0x4434, 0x55BD,
    0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E, 0xFAE7, 0xC87C, 0xD9F5,
    0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E, 0x54B5, 0x453C,
    0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD, 0xC974,
    0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
    0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3,
    0x5285, 0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A,
    0xDECD, 0xCF44, 0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72,
    0x6306, 0x728F, 0x4014, 0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9,
    0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5, 0xA96A, 0xB8E3, 0x8A78, 0x9BF1,
    0x7387, 0x620E, 0x5095, 0x411C, 0x35A3, 0x242A, 0x16B1, 0x0738,
    0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862, 0x9AF9, 0x8B70,
    0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E, 0xF0B7,
    0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
    0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036,
    0x18C1, 0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E,
    0xA50A, 0xB483, 0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5,
    0x2942, 0x38CB, 0x0A50, 0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD,
    0xB58B, 0xA402, 0x9699, 0x8710, 0xF3AF, 0xE226, 0xD0BD, 0xC134,
    0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7, 0x6E6E, 0x5CF5, 0x4D7C,
    0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1, 0xA33A, 0xB2B3,
    0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72, 0x3EFB,
    0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
    0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A,
    0xE70E, 0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1,
    0x6B46, 0x7ACF, 0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9,
    0xF78F, 0xE606, 0xD49D, 0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330,
    0x7BC7, 0x6A4E, 0x58D5, 0x495C, 0x3DE3, 0x2C6A, 0x1EF1, 0x0F78
};

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
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMS::CheckInit(CGXDLMSSettings& settings)
{
    if (settings.GetClientAddress() == 0)
    {
        return DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS;
    }
    if (settings.GetServerAddress() == 0)
    {
        return DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS;
    }
    return DLMS_ERROR_CODE_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Get data from Block.
/////////////////////////////////////////////////////////////////////////////
// data : Stored data block.
// index : Position where data starts.
// Returns : Amount of removed bytes.
/////////////////////////////////////////////////////////////////////////////
int GetDataFromBlock(CGXByteBuffer& data, int index)
{
    if (data.GetSize() == data.GetPosition())
    {
        data.Clear();
        return 0;
    }
    int len = data.GetPosition() - index;
    if (len < 0)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    unsigned long pos = data.GetPosition();
    data.SetPosition(pos - len);
    data.Move(pos, pos - len, data.GetSize() - pos);
    return 0;
}

int CGXDLMS::ReceiverReady(
    CGXDLMSSettings& settings,
    DLMS_DATA_REQUEST_TYPES type,
    CGXCipher* cipher,
    CGXByteBuffer& reply)
{
    int ret;
    reply.Clear();
    if (type == DLMS_DATA_REQUEST_TYPES_NONE)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    // Get next frame.
    if ((type & DLMS_DATA_REQUEST_TYPES_FRAME) != 0)
    {
        if ((ret = CGXDLMS::GetHdlcFrame(settings, settings.GetReceiverReady(), NULL, reply)) != 0)
        {
            return ret;
        }
        return ret;
    }
    // Get next block.
    CGXByteBuffer bb(6);
    bb.SetUInt32(settings.GetBlockIndex());
    settings.IncreaseBlockIndex();
    std::vector<CGXByteBuffer> tmp;
    if (settings.IsServer())
    {
        if ((ret = CGXDLMS::GetMessages(settings, DLMS_COMMAND_GET_RESPONSE, 2, bb, NULL, tmp)) != 0)
        {
            return ret;
        }
        return ret;
    }
    if ((ret = CGXDLMS::GetMessages(settings, DLMS_COMMAND_GET_REQUEST, 2, bb, NULL, tmp)) != 0)
    {
        return ret;
    }
    reply.Set(&tmp.at(0), 0, -1);
    return DLMS_ERROR_CODE_OK;
}

/**
     * Split DLMS PDU to wrapper frames.
     *
     * @param settings
     *            DLMS settings.
     * @param data
     *            Wrapped data.
     * @return Wrapper frames.
*/
int CGXDLMS::GetWrapperFrame(
    CGXDLMSSettings& settings,
    CGXByteBuffer& data,
    CGXByteBuffer& reply)
{
    reply.Clear();
    // Add version.
    reply.SetUInt16(1);
    if (settings.IsServer())
    {
        reply.SetUInt16(settings.GetServerAddress());
        reply.SetUInt16(settings.GetClientAddress());
    }
    else
    {
        reply.SetUInt16(settings.GetClientAddress());
        reply.SetUInt16(settings.GetServerAddress());
    }
    // Data length.
    reply.SetUInt16((unsigned short) data.GetSize());
    // Data
    reply.Set(&data, data.GetPosition(), -1);

    // Remove sent data in server side.
    if (settings.IsServer())
    {
        if (data.GetSize() == data.GetPosition())
        {
            data.Clear();
        }
        else
        {
            data.Move(data.GetPosition(), 0, data.GetSize() - data.GetPosition());
            data.SetPosition(0);
        }
    }
    return DLMS_ERROR_CODE_OK;
}

/**
 * Get HDLC frame for data.
 *
 * @param settings
 *            DLMS settings.
 * @param frame
 *            Frame ID. If zero new is generated.
 * @param data
 *            Data to add.
 * @return HDLC frame.
 */
int CGXDLMS::GetHdlcFrame(
    CGXDLMSSettings& settings,
    unsigned char frame,
    CGXByteBuffer* data,
    CGXByteBuffer& reply)
{
    reply.Clear();
    unsigned short frameSize;
    int ret, len = 0;
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

    // Add BOP
    reply.SetUInt8(HDLC_FRAME_START_END);
    frameSize = settings.GetLimits().GetMaxInfoTX().ToInteger();
    // If no data
    if (data == NULL || data->GetSize() == 0)
    {
        reply.SetUInt8(0xA0);
    }
    else if (data->GetSize() - data->GetPosition() <= frameSize)
    {
        // Is last packet.
        reply.SetUInt8(0xA0);
        len = data->GetSize() - data->GetPosition();
    }
    else
    {
        // More data to left.
        reply.SetUInt8(0xA8);
        len = frameSize;
    }
    // Frame len.
    if (len == 0)
    {
        reply.SetUInt8((unsigned char) (5 + primaryAddress.GetSize() +
                                        secondaryAddress.GetSize() + len));
    }
    else
    {
        reply.SetUInt8((unsigned char)(7 + primaryAddress.GetSize() +
                                       secondaryAddress.GetSize() + len));
    }
    // Add primary address.
    reply.Set(&primaryAddress);
    // Add secondary address.
    reply.Set(&secondaryAddress);

    // Add frame ID.
    if (frame == 0)
    {
        reply.SetUInt8(settings.GetNextSend());
    }
    else
    {
        reply.SetUInt8(frame);
    }
    // Add header CRC.
    int crc = CountFCS16(reply, 1, reply.GetSize() - 1);
    reply.SetUInt16(crc);
    if (len != 0)
    {
        // Add data.
        reply.Set(data, data->GetPosition(), len);
        // Add data CRC.
        crc = CountFCS16(reply, 1, reply.GetSize() - 1);
        reply.SetUInt16(crc);
    }
    // Add EOP
    reply.SetUInt8(HDLC_FRAME_START_END);
    // Remove sent data in server side.
    if (settings.IsServer())
    {
        if (data != NULL)
        {
            if (data->GetSize() == data->GetPosition())
            {
                data->Clear();
            }
            else
            {
                data->Move(data->GetPosition(), 0, data->GetSize() - data->GetPosition());
                data->SetPosition(0);
            }
        }
    }
    return DLMS_ERROR_CODE_OK;
}

/**
* Get used glo message.
*
* @param command
*            Executed DLMS_COMMAND_
* @return Integer value of glo message.
*/
unsigned char GetGloMessage(DLMS_COMMAND command)
{
    unsigned char cmd;
    switch (command)
    {
    case DLMS_COMMAND_READ_REQUEST:
    case DLMS_COMMAND_GET_REQUEST:
        cmd = DLMS_COMMAND_GLO_GET_REQUEST;
        break;
    case DLMS_COMMAND_WRITE_REQUEST:
    case DLMS_COMMAND_SET_REQUEST:
        cmd = DLMS_COMMAND_GLO_SET_REQUEST;
        break;
    case DLMS_COMMAND_METHOD_REQUEST:
        cmd = DLMS_COMMAND_GLO_METHOD_REQUEST;
        break;
    case DLMS_COMMAND_READ_RESPONSE:
    case DLMS_COMMAND_GET_RESPONSE:
        cmd = DLMS_COMMAND_GLO_GET_RESPONSE;
        break;
    case DLMS_COMMAND_WRITE_RESPONSE:
    case DLMS_COMMAND_SET_RESPONSE:
        cmd = DLMS_COMMAND_GLO_SET_RESPONSE;
        break;
    case DLMS_COMMAND_METHOD_RESPONSE:
        cmd = DLMS_COMMAND_GLO_METHOD_RESPONSE;
        break;
    default:
        cmd = DLMS_COMMAND_NONE;
    }
    return cmd;
}

bool CGXDLMS::MultipleBlocks(
    CGXDLMSSettings& settings,
    CGXByteBuffer& bb)
{
    if (!settings.GetUseLogicalNameReferencing())
    {
        return false;
    }
    return bb.GetSize() - bb.GetPosition() > settings.GetMaxReceivePDUSize();
}

unsigned char GetInvokeIDPriority(CGXDLMSSettings& settings)
{
    unsigned char value = 0;
    if (settings.GetPriority() == DLMS_PRIORITY_HIGH)
    {
        value |= 0x80;
    }
    if (settings.GetServiceClass() == DLMS_SERVICE_CLASS_CONFIRMED)
    {
        value |= 0x40;
    }
    value |= settings.GetInvokeID();
    return value;
}

/**
     * Generates Invoke ID and priority.
     *
     * @param settings
     *            DLMS settings.
     * @return Invoke ID and priority.
     */
long GetLongInvokeIDPriority(CGXDLMSSettings& settings)
{
    long value = 0;
    if (settings.GetPriority() == DLMS_PRIORITY_HIGH)
    {
        value = 0x80000000;
    }
    if (settings.GetServiceClass() == DLMS_SERVICE_CLASS_CONFIRMED)
    {
        value |= 0x40000000;
    }
    value |= (settings.GetLongInvokeID() & 0xFFFFFF);
    settings.SetLongInvokeID(settings.GetLongInvokeID() + 1);
    return value;
}
int GetMaxPduSize(
    CGXDLMSSettings& settings,
    CGXByteBuffer& data,
    CGXByteBuffer* bb)
{
    int size = data.GetSize() - data.GetPosition();
    int offset = 0;
    if (bb != NULL)
    {
        offset = bb->GetSize();
    }
    if (size + offset > settings.GetMaxReceivePDUSize())
    {
        size = settings.GetMaxReceivePDUSize() - offset;
        size -= GXHelpers::GetObjectCountSizeInBytes(size);
    }
    else if (size + GXHelpers::GetObjectCountSizeInBytes(size) > settings.GetMaxReceivePDUSize())
    {
        size = (size - GXHelpers::GetObjectCountSizeInBytes(size));
    }
    return size;
}

int CGXDLMS::GetLNPdu(CGXDLMSSettings& settings,
                      DLMS_COMMAND command,
                      unsigned char commandType,
                      CGXByteBuffer& data,
                      CGXByteBuffer& bb,
                      unsigned char status,
                      bool multipleBlocks,
                      bool lastBlock,
                      struct tm* date)
{
    int ret;
    unsigned long len = 0;
    bool ciphering = settings.GetCipher() != NULL && settings.GetCipher()->GetSecurity() != DLMS_SECURITY_NONE;
    int offset = 0;
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC)
    {
        if (settings.IsServer())
        {
            bb.Set(LLC_REPLY_BYTES, 3);
        }
        else
        {
            bb.Set(LLC_SEND_BYTES, 3);
        }
        offset = 3;
    }
    if (multipleBlocks && settings.GetLnSettings().GetGeneralBlockTransfer())
    {
        bb.SetUInt8(DLMS_COMMAND_GENERAL_BLOCK_TRANSFER);
        // If multiple blocks.
        if (multipleBlocks)
        {
            // If this is a last block make sure that all data is fit to it.
            if (lastBlock)
            {
                lastBlock = !CGXDLMS::MultipleBlocks(settings, data);
            }
        }
        // Is last block
        if (!lastBlock)
        {
            bb.SetUInt8(0);
        }
        else
        {
            bb.SetUInt8(0x80);
        }
        // Set block number sent.
        bb.SetUInt8(0);
        // Set block number acknowledged
        bb.SetUInt8((unsigned char) settings.GetBlockIndex());
        settings.IncreaseBlockIndex();
        // Add APU tag.
        bb.SetUInt8(0);
        // Add Addl fields
        bb.SetUInt8(0);
    }
    // Add command.
    bb.SetUInt8(command);

    if (command != DLMS_COMMAND_DATA_NOTIFICATION)
    {
        bb.SetUInt8(commandType);
        // Add Invoke Id And Priority.
        bb.SetUInt8(GetInvokeIDPriority(settings));
    }
    else
    {
        // Add Long-Invoke-Id-And-Priority
        bb.SetUInt32(GetLongInvokeIDPriority(settings));
        // Add date time.
        if (date == NULL)
        {
            bb.SetUInt8(DLMS_DATA_TYPE_NONE);
        }
        else
        {
            int pos = bb.GetPosition();
            CGXDLMSVariant tmp = *date;
            if ((ret = GXHelpers::SetData(bb, DLMS_DATA_TYPE_OCTET_STRING, tmp)) != 0)
            {
                return ret;
            }
            if ((ret = bb.Move(pos + 1, pos, bb.GetSize() - pos - 1)) != 0)
            {
                return ret;
            }
        }
    }

    if (command != DLMS_COMMAND_DATA_NOTIFICATION && !settings.GetLnSettings().GetGeneralBlockTransfer())
    {
        // If multiple blocks.
        if (multipleBlocks)
        {
            // If this is a last block make sure that all data is fit to it.
            if (lastBlock)
            {
                lastBlock = !CGXDLMS::MultipleBlocks(settings, data);
            }
            // Is last block.
            if (lastBlock)
            {
                bb.SetUInt8(1);
                settings.SetCount(0);
                settings.SetIndex(0);
            }
            else
            {
                bb.SetUInt8(0);
            }
            // Block index.
            bb.SetUInt32(settings.GetBlockIndex());
            if (status != 0)
            {
                bb.SetUInt8(1);
            }
            bb.SetUInt8(status);
            // Block size.
            if (bb.GetSize() != 0)
            {
                len = GetMaxPduSize(settings, data, &bb);
                GXHelpers::SetObjectCount(len, bb);
            }
        }
        else if (status != 0xFF)
        {
            // If error has occurred.
            if (status != 0 && command != DLMS_COMMAND_METHOD_RESPONSE && command != DLMS_COMMAND_SET_RESPONSE)
            {
                bb.SetUInt8(1);
            }
            bb.SetUInt8(status);
        }
    }
    else if (bb.GetSize() != 0)
    {
        // Block size.
        len = GetMaxPduSize(settings, data, &bb);
    }
    // Add data
    if (data.GetSize() != 0)
    {
        if (len == 0)
        {
            len = data.GetSize() - data.GetPosition();
            if (len > settings.GetMaxReceivePDUSize() - bb.GetSize())
            {
                len = settings.GetMaxReceivePDUSize() - bb.GetSize();
            }
        }
        bb.Set(&data, 0, len);
    }
    if (ciphering)
    {
        CGXByteBuffer tmp, crypted;
        if ((ret = bb.SubArray(offset, bb.GetSize() - offset, tmp)) != 0)
        {
            return ret;
        }
        if ((ret = settings.GetCipher()->Encrypt(
                       settings.GetCipher()->GetSecurity(),
                       DLMS_COUNT_TYPE_PACKET,
                       settings.GetCipher()->GetFrameCounter() + 1,
                       GetGloMessage(command),
                       settings.GetCipher()->GetSystemTitle(),
                       tmp,
                       crypted)) != 0)
        {
            return ret;
        }
        settings.GetCipher()->SetFrameCounter(settings.GetCipher()->GetFrameCounter() + 1);
        bb.SetSize(offset);
        bb.Set(&crypted);
    }
    return 0;
}

int GetLNMessages(
    CGXDLMSSettings& settings,
    DLMS_COMMAND command,
    unsigned char commandType,
    CGXByteBuffer& data,
    struct tm* time,
    std::vector<CGXByteBuffer>& messages)
{
    int ret;
    CGXByteBuffer bb;
    unsigned char frame = 0;
    if (command == DLMS_COMMAND_AARQ)
    {
        frame = 0x10;
    }
    bool multipleBlocks = CGXDLMS::MultipleBlocks(settings, data);
    do
    {
        if (command == DLMS_COMMAND_AARQ)
        {
            if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC)
            {
                bb.Set(LLC_SEND_BYTES, 3);
            }
            bb.Set(&data);
        }
        else
        {
            if ((ret = CGXDLMS::GetLNPdu(settings, command, commandType, data, bb, 0xFF,
                                         multipleBlocks, true, time)) != 0)
            {
                return ret;
            }
        }
        while (bb.GetPosition() != bb.GetSize())
        {
            CGXByteBuffer reply;
            if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
            {
                if ((ret = CGXDLMS::GetWrapperFrame(settings, bb, reply)) != 0)
                {
                    return ret;
                }
                messages.push_back(reply);
            }
            else
            {
                if ((ret = CGXDLMS::GetHdlcFrame(settings, frame, &bb, reply)) != 0)
                {
                    return ret;
                }
                messages.push_back(reply);
                frame = 0;
            }
        }
        bb.Clear();
    }
    while (data.GetPosition() != data.GetSize());
    return 0;
}

int CGXDLMS::GetSNPdu(CGXDLMSSettings& settings,
                      DLMS_COMMAND command,
                      CGXByteBuffer& data,
                      CGXByteBuffer& bb)
{
    int ret;
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC &&
            bb.GetSize() == 0)
    {
        if (settings.IsServer())
        {
            bb.Set(LLC_REPLY_BYTES, 3);
        }
        else
        {
            bb.Set(LLC_SEND_BYTES, 3);
        }
    }
    // Add DLMS_COMMAND_
    CGXByteBuffer tmp;
    if (command != DLMS_COMMAND_AARQ && command != DLMS_COMMAND_AARE)
    {
        tmp.SetUInt8(command);
    }
    // Add data.
    tmp.Set(&data);
    // If Ciphering is used.
    if (settings.GetCipher() != NULL
            && settings.GetCipher()->GetSecurity() != DLMS_SECURITY_NONE
            && command != DLMS_COMMAND_AARQ && command != DLMS_COMMAND_AARE)
    {
        CGXByteBuffer crypted;
        if ((ret = settings.GetCipher()->Encrypt(settings.GetCipher()->GetSecurity(),
                   DLMS_COUNT_TYPE_PACKET,
                   settings.GetCipher()->GetFrameCounter(),
                   GetGloMessage(command),
                   settings.GetCipher()->GetSystemTitle(), tmp, crypted)) != 0)
        {
            return ret;
        }
        bb.Set(&crypted);
    }
    else
    {
        bb.Set(&tmp);
    }
    return 0;
}

int GetSNMessages(
    CGXDLMSSettings& settings,
    DLMS_COMMAND command,
    unsigned char commandType,
    CGXByteBuffer& data,
    struct tm* time,
    std::vector<CGXByteBuffer>& reply)
{
    int ret;
    CGXByteBuffer bb;
    unsigned char frame = 0x0;
    reply.clear();
    if ((ret = CGXDLMS::GetSNPdu(settings, command, data, bb)) != 0)
    {
        return ret;
    }
    if (command == DLMS_COMMAND_AARQ)
    {
        frame = 0x10;
    }
    while (bb.GetPosition() != bb.GetSize())
    {
        data.Clear();
        if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        {
            if ((ret = CGXDLMS::GetWrapperFrame(settings, bb, data)) != 0)
            {
                return ret;
            }
        }
        else
        {
            if ((ret = CGXDLMS::GetHdlcFrame(settings, frame, &bb, data)) != 0)
            {
                return ret;
            }
            frame = 0;
        }
        reply.push_back(data);
    }
    return 0;
}


int CGXDLMS::GetMessages(
    CGXDLMSSettings& settings,
    DLMS_COMMAND command,
    int commandType,
    CGXByteBuffer& data,
    struct tm* time,
    std::vector<CGXByteBuffer>& reply)
{
    // Save original position.
    int pos = data.GetPosition();
    int ret;
    if (settings.GetUseLogicalNameReferencing())
    {
        ret = GetLNMessages(settings, command, commandType, data, time, reply);
    }
    else
    {
        ret = GetSNMessages(settings, command, commandType, data, time, reply);
    }
    data.SetPosition(pos);
    return ret;
}

int CGXDLMS::GetHdlcData(
    bool server,
    CGXDLMSSettings& settings,
    CGXByteBuffer& reply,
    CGXReplyData& data,
    unsigned char& frame)
{
    unsigned long packetStartID = reply.GetPosition(), frameLen = 0;
    unsigned long pos;
    unsigned char ch;
    int ret;
    unsigned short crc, crcRead;
    // If whole frame is not received yet.
    if (reply.GetSize() - reply.GetPosition() < 9)
    {
        data.SetComplete(false);
        return 0;
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
    // Sometimes meters can send some strange data between DLMS frames.
    if (reply.GetPosition() == reply.GetSize())
    {
        data.SetComplete(false);
        // Not enough data to parse;
        return 0;
    }
    if ((ret = reply.GetUInt8(&frame)) != 0)
    {
        return ret;
    }
    if ((frame & 0xF0) != 0xA0)
    {
        // If same data.
        return GetHdlcData(server, settings, reply, data, frame);
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
        return 0;
    }
    int eopPos = frameLen + packetStartID + 1;
    if ((ret = reply.GetUInt8(eopPos, &ch)) != 0)
    {
        return ret;
    }
    if (ch != HDLC_FRAME_START_END)
    {
        return DLMS_ERROR_CODE_NOT_REPLY;
    }

    // Check addresses.
    ret = CheckHdlcAddress(server, settings, reply, data, eopPos);
    if (ret != 0)
    {
        if (ret == DLMS_ERROR_CODE_FALSE)
        {
            // If echo,
            return GetHdlcData(server, settings, reply, data, frame);
        }
        return ret;
    }

    // Is there more data available.
    if ((frame & 0x8) != 0)
    {
        data.SetMoreData((DLMS_DATA_REQUEST_TYPES) (data.GetMoreData() | DLMS_DATA_REQUEST_TYPES_FRAME));
    }
    else
    {
        data.SetMoreData((DLMS_DATA_REQUEST_TYPES) (data.GetMoreData() & ~DLMS_DATA_REQUEST_TYPES_FRAME));
    }
    // Get frame type.
    if ((ret = reply.GetUInt8(&frame)) != 0)
    {
        return ret;
    }
    if (!settings.CheckFrame(frame))
    {
        reply.SetPosition(eopPos + 1);
        return GetHdlcData(server, settings, reply, data, frame);
    }
    // Check that header CRC is correct.
    crc = CountFCS16(reply, packetStartID + 1,
                     reply.GetPosition() - packetStartID - 1);

    if ((ret = reply.GetUInt16(&crcRead)) != 0)
    {
        return ret;
    }
    if (crc != crcRead)
    {
        return DLMS_ERROR_CODE_WRONG_CRC;
    }
    // Check that packet CRC match only if there is a data part.
    if (reply.GetPosition() != packetStartID + frameLen + 1)
    {
        crc = CountFCS16(reply, packetStartID + 1,
                         frameLen - 2);
        if ((ret = reply.GetUInt16(packetStartID + frameLen - 1, &crcRead)) != 0)
        {
            return ret;
        }
        if (crc != crcRead)
        {
            return DLMS_ERROR_CODE_WRONG_CRC;
        }
        // Remove CRC and EOP from packet length.
        data.SetPacketLength(eopPos - 2);
    }
    else
    {
        data.SetPacketLength(reply.GetSize());
    }

    if ((frame & HDLC_FRAME_TYPE_U_FRAME) == HDLC_FRAME_TYPE_U_FRAME)
    {
        // Get Eop if there is no data.
        if (reply.GetPosition() == packetStartID + frameLen + 1)
        {
            // Get EOP.
            if ((ret = reply.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
        }
        data.SetCommand((DLMS_COMMAND) frame);
    }
    else if ((frame & HDLC_FRAME_TYPE_S_FRAME) == HDLC_FRAME_TYPE_S_FRAME)
    {
        // If S-frame
        int tmp = (frame >> 2) & 0x3;
        // If frame is rejected.
        if (tmp == HDLC_CONTROL_FRAME_REJECT)
        {
            return DLMS_ERROR_CODE_REJECTED;
        }
        else if (tmp == HDLC_CONTROL_FRAME_RECEIVE_NOT_READY)
        {
            return DLMS_ERROR_CODE_REJECTED;
        }
        else if (tmp == HDLC_CONTROL_FRAME_RECEIVE_READY)
        {
            // Get next frame.
        }
        // Get Eop if there is no data.
        if (reply.GetPosition() == packetStartID + frameLen + 1)
        {
            // Get EOP.
            if ((ret = reply.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
        }
    }
    else
    {
        // I-frame
        // Get Eop if there is no data.
        if (reply.GetPosition() == packetStartID + frameLen + 1)
        {
            // Get EOP.
            if ((ret = reply.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if ((frame & 0x1) == 0x1)
            {
                data.SetMoreData(DLMS_DATA_REQUEST_TYPES_FRAME);
            }
        }
        else
        {
            GetLLCBytes(server, reply);
        }
    }
    return DLMS_ERROR_CODE_OK;
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
    for (unsigned long pos = buff.GetPosition(); pos != buff.GetSize(); ++pos)
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
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
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
            return DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS;
        }
        else
        {
            settings.SetServerAddress(target);
        }

        // Check that client addresses match.
        if (settings.GetClientAddress() != 0 && settings.GetClientAddress() != source)
        {
            return DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS;
        }
        else
        {
            settings.SetClientAddress(source);
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
            }
            return DLMS_ERROR_CODE_FALSE;
        }
        // Check that server addresses match.
        if (settings.GetServerAddress() != source)
        {
            return DLMS_ERROR_CODE_FALSE;
        }
    }
    return DLMS_ERROR_CODE_OK;
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
    //Action-Response-Normal
    if (type == 1)
    {
        if ((ret = data.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            return ch;
        }
        // Response normal. Get data if exists.
        if (data.GetData().GetPosition() < data.GetData().GetSize())
        {
            if ((ret = data.GetData().GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            if (ch == 0)
            {
                GetDataFromBlock(data.GetData(), 0);
            }
            else if (ch == 1)
            {
                //Get Data-Access-Result
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
                    return ch;
                }
                GetDataFromBlock(data.GetData(), 0);
            }
            else
            {
                //Invalid tag.
                return DLMS_ERROR_CODE_INVALID_TAG;
            }
        }
    }
    else if  (type == 2)
    {
        //Action-Response-With-Pblock
        return DLMS_ERROR_CODE_INVALID_COMMAND;
    }
    else if  (type == 3)
    {
        // Action-Response-With-List.
        return DLMS_ERROR_CODE_INVALID_COMMAND;
    }
    else if  (type == 4)
    {
        //Action-Response-Next-Pblock
        return DLMS_ERROR_CODE_INVALID_COMMAND;
    }
    else
    {
        return DLMS_ERROR_CODE_INVALID_COMMAND;
    }
    return DLMS_ERROR_CODE_OK;
}

/**
    * Handle data notification get data from block and/or update error status.
    *
    * @param settings
    *            DLMS settings.
    * @param reply
    *            Received data from the client.
    */
int CGXDLMS::HandleDataNotification(
    CGXDLMSSettings& settings,
    CGXReplyData& reply)
{
    unsigned long id;
    int ret;
    int start = reply.GetData().GetPosition() - 1;
    // Get invoke id.
    if ((ret = reply.GetData().GetUInt32(&id)) != 0)
    {
        return ret;
    }
    // Get date time.
    CGXDataInfo info;
    reply.SetTime(NULL);
    unsigned char len;
    if ((ret = reply.GetData().GetUInt8(&len)) != 0)
    {
        return ret;
    }
    if (len != 0)
    {
        CGXByteBuffer tmp;
        CGXDLMSVariant t;
        tmp.Set(&reply.GetData(), reply.GetData().GetPosition(), len);
        if ((ret = CGXDLMSClient::ChangeType(tmp, DLMS_DATA_TYPE_DATETIME, t)) != 0)
        {
            return ret;
        }
        reply.SetTime(&t.dateTime.GetValue());
    }
    if ((ret = GetDataFromBlock(reply.GetData(), start)) != 0)
    {
        return ret;
    }
    return GetValueFromData(settings, reply);
}

int CGXDLMS::HandleSetResponse(
    CGXDLMSSettings& settings,
    CGXReplyData& data)
{
    unsigned char ch;
    int ret;
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    //SetResponseNormal
    if (ch == 1)
    {
        //Invoke ID and priority.
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
            return ch;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMS::HandleGbt(CGXDLMSSettings& settings, CGXReplyData& data)
{
    int ret;
    unsigned char ch, bn, bna;
    data.SetGbt(true);
    int index = data.GetData().GetPosition() - 1;
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Is streaming active.
    //TODO: bool streaming = (ch & 0x40) == 1;
    unsigned char window = (ch & 0x3F);
    // Block number.
    if ((ret = data.GetData().GetUInt8(&bn)) != 0)
    {
        return ret;
    }
    // Block number acknowledged.
    if ((ret = data.GetData().GetUInt8(&bna)) != 0)
    {
        return ret;
    }
    // Get APU tag.
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0)
    {
        //Invalid APU.
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    // Get Addl tag.
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0)
    {
        //Invalid APU.
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch != 0)
    {
        return DLMS_ERROR_CODE_INVALID_TAG;
    }
    data.SetCommand(DLMS_COMMAND_NONE);
    if (window != 0)
    {
        unsigned long len;
        if ((ret = GXHelpers::GetObjectCount(data.GetData(), len)) != 0)
        {
            return ret;
        }
        if (len != (unsigned long)(data.GetData().GetSize() - data.GetData().GetPosition()))
        {
            data.SetComplete(false);
            return 0;
        }
    }

    if ((ret = GetDataFromBlock(data.GetData(), index)) != 0 ||
            (ret = CGXDLMS::GetPdu(settings, data)) != 0)
    {
        return ret;
    }
    // Is Last block,
    if ((ch & 0x80) == 0)
    {
        data.SetMoreData((DLMS_DATA_REQUEST_TYPES) (data.GetMoreData() | DLMS_DATA_REQUEST_TYPES_BLOCK));
    }
    else
    {
        data.SetMoreData((DLMS_DATA_REQUEST_TYPES) (data.GetMoreData() & ~DLMS_DATA_REQUEST_TYPES_BLOCK));
    }
    // Get data if all data is read or we want to peek data.
    if (data.GetData().GetPosition() != data.GetData().GetSize()
            && (data.GetCommand() == DLMS_COMMAND_READ_RESPONSE
                || data.GetCommand() == DLMS_COMMAND_GET_RESPONSE)
            && (data.GetMoreData() == DLMS_DATA_REQUEST_TYPES_NONE
                || data.GetPeek()))
    {
        data.GetData().SetPosition(0);
        ret = CGXDLMS::GetValueFromData(settings, data);
    }
    return ret;
}

int CGXDLMS::GetPdu(
    CGXDLMSSettings& settings,
    CGXReplyData& data)
{
    int ret = DLMS_ERROR_CODE_OK;
    unsigned char ch;
    DLMS_COMMAND cmd = data.GetCommand();
    // If header is not read yet or GBT message.
    if (cmd == DLMS_COMMAND_NONE || data.GetGbt())
    {
        // If PDU is missing.
        if (data.GetData().GetSize() - data.GetData().GetPosition() == 0)
        {
            // Invalid PDU.
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
        int index = data.GetData().GetPosition();
        // Get Command.
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
                if (ret == DLMS_ERROR_CODE_FALSE)
                {
                    return 0;
                }
                return ret;
            }
            break;
        case DLMS_COMMAND_GET_RESPONSE:
            if ((ret = HandleGetResponse(settings, data, index)) != 0)
            {
                if (ret == DLMS_ERROR_CODE_FALSE)
                {
                    return 0;
                }
                return ret;
            }
            break;
        case DLMS_COMMAND_SET_RESPONSE:
            ret = HandleSetResponse(settings, data);
            break;
        case DLMS_COMMAND_WRITE_RESPONSE:
            ret = HandleWriteResponse(data);
            break;
        case DLMS_COMMAND_METHOD_RESPONSE:
            ret = HandleMethodResponse(settings, data);
            break;
        case DLMS_COMMAND_GENERAL_BLOCK_TRANSFER:
            ret = HandleGbt(settings, data);
            break;
        case DLMS_COMMAND_AARQ:
        case DLMS_COMMAND_AARE:
            // This is parsed later.
            data.GetData().SetPosition(data.GetData().GetPosition() - 1);
            break;
        case DLMS_COMMAND_DISCONNECT_RESPONSE:
            break;
        case DLMS_COMMAND_EXCEPTION_RESPONSE:
        /* TODO:
        throw new GXDLMSException(
            StateError.values()[data.getData().getUInt8() - 1],
            ServiceError.values()[data.getData().getUInt8() - 1]);
            */
        case DLMS_COMMAND_GET_REQUEST:
        case DLMS_COMMAND_READ_REQUEST:
        case DLMS_COMMAND_WRITE_REQUEST:
        case DLMS_COMMAND_SET_REQUEST:
        case DLMS_COMMAND_METHOD_REQUEST:
        case DLMS_COMMAND_DISCONNECT_REQUEST:
            // Server handles this.
            if ((data.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) != 0)
            {
                break;
            }
            break;
        case DLMS_COMMAND_GLO_GET_REQUEST:
        case DLMS_COMMAND_GLO_SET_REQUEST:
        case DLMS_COMMAND_GLO_METHOD_REQUEST:
            if (settings.GetCipher() == NULL)
            {
                //Secure connection is not supported.
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            // If all frames are read.
            if ((data.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == 0)
            {
                data.GetData().SetPosition(data.GetData().GetPosition() - 1);
                DLMS_SECURITY security;
                if ((ret = settings.GetCipher()->Decrypt(settings.GetSourceSystemTitle(), data.GetData(), security)) != 0)
                {
                    return ret;
                }
                // Get command
                if ((ret = data.GetData().GetUInt8(&ch)) != 0)
                {
                    return ret;
                }
                cmd = (DLMS_COMMAND) ch;
                data.SetCommand(cmd);
            }
            else
            {
                data.GetData().SetPosition(data.GetData().GetPosition() - 1);
            }
            // Server handles this.
            break;
        case DLMS_COMMAND_GLO_GET_RESPONSE:
        case DLMS_COMMAND_GLO_SET_RESPONSE:
        case DLMS_COMMAND_GLO_METHOD_RESPONSE:
            if (settings.GetCipher() == NULL)
            {
                //Secure connection is not supported.
                return DLMS_ERROR_CODE_INVALID_PARAMETER;
            }
            // If all frames are read.
            if ((data.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == 0)
            {
                data.GetData().SetPosition(data.GetData().GetPosition() - 1);
                CGXByteBuffer bb(data.GetData());
                data.GetData().SetPosition(index);
                data.GetData().SetSize(index);
                DLMS_SECURITY security;
                if ((ret = settings.GetCipher()->Decrypt(settings.GetSourceSystemTitle(), bb, security)) != 0)
                {
                    return ret;
                }
                data.GetData().Set(&bb, bb.GetPosition(), bb.GetSize() - bb.GetPosition());
                data.SetCommand(DLMS_COMMAND_NONE);
                ret = GetPdu(settings, data);
                data.SetCipherIndex((unsigned short) data.GetData().GetSize());
            }
            break;
        case DLMS_COMMAND_DATA_NOTIFICATION:
            ret = HandleDataNotification(settings, data);
            // Client handles this.
            break;
        default:
            // Invalid DLMS_COMMAND_
            return DLMS_ERROR_CODE_INVALID_PARAMETER;
        }
    }
    else if ((data.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == 0)
    {
        // Is whole block is read and if last packet and data is not try to
        // peek.
        if (!data.GetPeek() && data.GetMoreData() == DLMS_DATA_REQUEST_TYPES_NONE)
        {
            if (data.GetCommand() == DLMS_COMMAND_AARE
                    || data.GetCommand() == DLMS_COMMAND_AARQ)
            {
                data.GetData().SetPosition(0);
            }
            else
            {
                data.GetData().SetPosition(1);
            }
            settings.ResetBlockIndex();
        }
        // Get command if operating as a server.
        if (settings.IsServer())
        {
            // Ciphered messages are handled after whole PDU is received.
            switch (cmd)
            {
            case DLMS_COMMAND_GLO_GET_REQUEST:
            case DLMS_COMMAND_GLO_SET_REQUEST:
            case DLMS_COMMAND_GLO_METHOD_REQUEST:
                data.SetCommand(DLMS_COMMAND_NONE);
                data.GetData().SetPosition(data.GetCipherIndex());
                ret = GetPdu(settings, data);
                break;
            default:
                break;
            }
        }
        else
        {
            // Client do not need a command any more.
            data.SetCommand(DLMS_COMMAND_NONE);
            // Ciphered messages are handled after whole PDU is received.
            switch (cmd)
            {
            case DLMS_COMMAND_GLO_GET_RESPONSE:
            case DLMS_COMMAND_GLO_SET_RESPONSE:
            case DLMS_COMMAND_GLO_METHOD_RESPONSE:
                data.GetData().SetPosition(data.GetCipherIndex());
                ret = GetPdu(settings, data);
                break;
            default:
                break;
            }
        }
    }

// Get data if all data is read or we want to peek data.
    if (data.GetData().GetPosition() != data.GetData().GetSize()
            && (cmd == DLMS_COMMAND_READ_RESPONSE || cmd == DLMS_COMMAND_GET_RESPONSE)
            && (data.GetMoreData() == DLMS_DATA_REQUEST_TYPES_NONE
                || data.GetPeek()))
    {
        ret = GetValueFromData(settings, data);
    }
    return ret;
}

int CGXDLMS::GetData(CGXDLMSSettings& settings,
                     CGXByteBuffer& reply,
                     CGXReplyData& data)
{
    int ret;
    unsigned char frame = 0;
    // If DLMS frame is generated.
    if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_HDLC)
    {
        if ((ret = GetHdlcData(settings.IsServer(), settings, reply, data, frame)) != 0)
        {
            return ret;
        }
    }
    else if (settings.GetInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
    {
        if ((ret = GetTcpData(settings, reply, data)) != 0)
        {
            return ret;
        }
    }
    else
    {
        // Invalid Interface type.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    // If all data is not read yet.
    if (!data.IsComplete())
    {
        return DLMS_ERROR_CODE_FALSE;
    }
    GetDataFromFrame(reply, data);
    // If keepalive or get next frame request.
    if ((frame & 0x1) != 0)
    {
        return DLMS_ERROR_CODE_OK;
    }
    if ((ret = GetPdu(settings, data)) != 0)
    {
        return ret;
    }

    if (data.GetCommand() == DLMS_COMMAND_DATA_NOTIFICATION)
    {
        // Check is there more messages left. This is Push message special
        // case.
        if (reply.GetPosition() == reply.GetSize())
        {
            reply.SetSize(0);
        }
        else
        {
            int cnt = reply.GetSize() - reply.GetPosition();
            if ((ret = reply.Move(reply.GetPosition(), 0, cnt)) != 0)
            {
                return ret;
            }
            reply.SetPosition(0);
        }
    }
    return 0;
}

int CGXDLMS::HandleGetResponse(
    CGXDLMSSettings& settings,
    CGXReplyData& reply,
    int index)
{
    int ret;
    unsigned char ch;
    unsigned long number;
    short type;
    unsigned long count;
    CGXByteBuffer& data = reply.GetData();

    // Get type.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    type = ch;
    // Get invoke ID and priority.
    if ((ret = data.GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    // Response normal
    if (type == 1)
    {
        // Result
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            return ch;
        }
        ret = GetDataFromBlock(data, 0);
    }
    else if (type == 2)
    {
        // GetResponsewithDataBlock
        // Is Last block.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch == 0)
        {
            reply.SetMoreData(
                (DLMS_DATA_REQUEST_TYPES)(reply.GetMoreData() | DLMS_DATA_REQUEST_TYPES_BLOCK));
        }
        else
        {
            reply.SetMoreData(
                (DLMS_DATA_REQUEST_TYPES)(reply.GetMoreData() & ~DLMS_DATA_REQUEST_TYPES_BLOCK));
        }
        // Get Block number.
        if ((ret = data.GetUInt32(&number)) != 0)
        {
            return ret;
        }
        // If meter's block index is zero based or Actaris is read.
        // Actaris SL7000 might return wrong block index sometimes.
        // It's not reseted to 1.
        if (number != 1 && settings.GetBlockIndex() == 1)
        {
            settings.SetBlockIndex(number);
        }
        if (number != settings.GetBlockIndex())
        {
            return DLMS_ERROR_CODE_DATA_BLOCK_NUMBER_INVALID;
        }
        // Get status.
        if ((ret = data.GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        if (ch != 0)
        {
            if ((ret = data.GetUInt8(&ch)) != 0)
            {
                return ret;
            }
            return ch;
        }
        else
        {
            // Get data size.
            GXHelpers::GetObjectCount(data, count);
            // if whole block is read.
            if ((reply.GetMoreData() & DLMS_DATA_REQUEST_TYPES_FRAME) == 0)
            {
                // Check Block length.
                if (count > (unsigned long) (data.GetSize() - data.GetPosition()))
                {
                    return DLMS_ERROR_CODE_OUTOFMEMORY;
                }
                reply.SetCommand(DLMS_COMMAND_NONE);
            }
            if ((ret = GetDataFromBlock(data, index)) != 0)
            {
                return ret;
            }
            // If last packet and data is not try to peek.
            if (reply.GetMoreData() == DLMS_DATA_REQUEST_TYPES_NONE)
            {
                if (!reply.GetPeek())
                {
                    data.SetPosition(0);
                    settings.ResetBlockIndex();
                }
            }
        }
    }
    else if (type == 3)
    {
        // Get response with list.
        //Get count.
        if ((ret = GXHelpers::GetObjectCount(data, count)) != 0)
        {
            return ret;
        }
        GetDataFromBlock(data, 0);
        return DLMS_ERROR_CODE_FALSE;
    }
    else
    {
        //Invalid Get response.
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return ret;
}

int CGXDLMS::HandleWriteResponse(CGXReplyData& data)
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
            return ch;
        }
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMS::HandleReadResponse(
    CGXReplyData& data)
{
    unsigned char ch;
    unsigned long count;
    int ret;
    if ((ret = GXHelpers::GetObjectCount(data.GetData(), count)) != 0)
    {
        return ret;
    }
    if (count != 1)
    {
        GetDataFromBlock(data.GetData(), 0);
        return DLMS_ERROR_CODE_FALSE;
    }
    // Get status code.
    if ((ret = data.GetData().GetUInt8(&ch)) != 0)
    {
        return ret;
    }
    if (ch == 0)
    {
        GetDataFromBlock(data.GetData(), 0);
    }
    else
    {
        // Get error code.
        if ((ret = data.GetData().GetUInt8(&ch)) != 0)
        {
            return ret;
        }
        return ch;
    }
    return DLMS_ERROR_CODE_OK;
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
        return DLMS_ERROR_CODE_OK;
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
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    // Check TCP/IP addresses.
    CheckWrapperAddress(settings, buff);
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
        return DLMS_ERROR_CODE_FALSE;
    }
    else
    {
        data.SetPacketLength(buff.GetPosition() + value);
    }
    return DLMS_ERROR_CODE_OK;
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
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    return DLMS_ERROR_CODE_OK;
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
    if (value.vt != DLMS_DATA_TYPE_NONE)
    {
        if (value.vt != DLMS_DATA_TYPE_ARRAY)
        {
            reply.SetValueType(info.GetType());
            reply.SetValue(value);
            reply.SetTotalCount(0);
            if (reply.GetCommand() == DLMS_COMMAND_DATA_NOTIFICATION)
            {
                reply.SetReadPosition(reply.GetData().GetPosition());
            }
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
    }
    else if (info.IsCompleate()
             && reply.GetCommand() == DLMS_COMMAND_DATA_NOTIFICATION)
    {
        // If last item is null. This is a special case.
        reply.SetReadPosition(reply.GetData().GetPosition());
    }
    reply.GetData().SetPosition(index);

    // If last data frame of the data block is read.
    if (reply.GetCommand() != DLMS_COMMAND_DATA_NOTIFICATION
            && info.IsCompleate()
            && reply.GetMoreData() == DLMS_DATA_REQUEST_TYPES_NONE)
    {
        // If all blocks are read.
        settings.ResetBlockIndex();
        reply.GetData().SetPosition(0);
    }
    return 0;
}

void CGXDLMS::GetDataFromFrame(CGXByteBuffer& reply, CGXReplyData& info)
{
    CGXByteBuffer& data = info.GetData();
    int offset = data.GetSize();
    int cnt = info.GetPacketLength() - reply.GetPosition();
    if (cnt != 0)
    {
        data.Capacity(offset + cnt);
        data.Set(&reply, reply.GetPosition(), cnt);
    }
    // Set position to begin of new data.
    data.SetPosition(offset);
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

int CGXDLMS::CheckWrapperAddress(
    CGXDLMSSettings& settings,
    CGXByteBuffer& buff)
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
            return DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS;
        }
        else
        {
            settings.SetClientAddress(value);
        }

        if ((ret = buff.GetUInt16(&value)) != 0)
        {
            return ret;
        }
        // Check that server addresses match.
        if (settings.GetServerAddress() != 0
                && settings.GetServerAddress() != value)
        {
            return DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS;
        }
        else
        {
            settings.SetServerAddress(value);
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
            return DLMS_ERROR_CODE_INVALID_SERVER_ADDRESS;
        }
        else
        {
            settings.SetServerAddress(value);
        }

        if ((ret = buff.GetUInt16(&value)) != 0)
        {
            return ret;
        }
        // Check that client addresses match.
        if (settings.GetClientAddress() != 0
                && settings.GetClientAddress() != value)
        {
            return DLMS_ERROR_CODE_INVALID_CLIENT_ADDRESS;
        }
        else
        {
            settings.SetClientAddress(value);
        }
    }
    return DLMS_ERROR_CODE_OK;
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
        fcs16 = (fcs16 >> 8) ^ FCS16Table[(fcs16 ^ ch) & 0xFF];
    }
    fcs16 = ~fcs16;
    fcs16 = ((fcs16 >> 8) & 0xFF) | (fcs16 << 8);
    return fcs16;
}

int CGXDLMS::GetActionInfo(DLMS_OBJECT_TYPE objectType, unsigned char& value, unsigned char& count)
{
    switch (objectType)
    {
    case DLMS_OBJECT_TYPE_DATA:
    case DLMS_OBJECT_TYPE_ACTION_SCHEDULE:
    case DLMS_OBJECT_TYPE_ALL:
    case DLMS_OBJECT_TYPE_AUTO_ANSWER:
    case DLMS_OBJECT_TYPE_AUTO_CONNECT:
    case DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP:
    case DLMS_OBJECT_TYPE_GPRS_SETUP:
    case DLMS_OBJECT_TYPE_IEC_HDLC_SETUP:
    case DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP:
    case DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP:
    case DLMS_OBJECT_TYPE_MODEM_CONFIGURATION:
    case DLMS_OBJECT_TYPE_PPP_SETUP:
    case DLMS_OBJECT_TYPE_REGISTER_MONITOR:
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP:
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN:
    case DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION:
    case DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL:
    case DLMS_OBJECT_TYPE_SCHEDULE:
    case DLMS_OBJECT_TYPE_SMTP_SETUP:
    case DLMS_OBJECT_TYPE_STATUS_MAPPING:
    case DLMS_OBJECT_TYPE_TCP_UDP_SETUP:
    case DLMS_OBJECT_TYPE_UTILITY_TABLES:
        value = 00;
        count = 0;
        break;
    case DLMS_OBJECT_TYPE_IMAGE_TRANSFER:
        value = 0x40;
        count = 4;
        break;
    case DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR:
        value = 0x50;
        count = 1;
        break;
    case DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME:
        value = 0x60;
        count = 4;
        break;
    case DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME:
        value = 0x20;
        count = 8;
        break;
    case DLMS_OBJECT_TYPE_CLOCK:
        value = 0x60;
        count = 6;
        break;
    case DLMS_OBJECT_TYPE_DEMAND_REGISTER:
        value = 0x48;
        count = 2;
        break;
    case DLMS_OBJECT_TYPE_EXTENDED_REGISTER:
        value = 0x38;
        count = 1;
        break;
    case DLMS_OBJECT_TYPE_IP4_SETUP:
        value = 0x60;
        count = 3;
        break;
    case DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP:
        value = 0x60;
        count = 8;
        break;
    case DLMS_OBJECT_TYPE_PROFILE_GENERIC:
        value = 0x58;
        count = 4;
        break;
    case DLMS_OBJECT_TYPE_REGISTER:
        value = 0x28;
        count = 1;
        break;
    case DLMS_OBJECT_TYPE_REGISTER_ACTIVATION:
        value = 0x30;
        count = 3;
        break;
    case DLMS_OBJECT_TYPE_REGISTER_TABLE:
        value = 0x28;
        count = 2;
        break;
    case DLMS_OBJECT_TYPE_SAP_ASSIGNMENT:
    case DLMS_OBJECT_TYPE_SCRIPT_TABLE:
        value = 0x20;
        count = 1;
        break;
    case DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE:
        value = 0x10;
        count = 2;
        break;
    default:
        count = value = 0;
        break;
    }
    return DLMS_ERROR_CODE_OK;
}

int CGXDLMS::AppendData(
    CGXDLMSObject* obj,
    unsigned char index,
    CGXByteBuffer& bb,
    CGXDLMSVariant& value)
{

    int ret;
    DLMS_DATA_TYPE tp;
    if ((ret = obj->GetDataType(index, tp)) != 0)
    {
        return ret;
    }
    if (tp == DLMS_DATA_TYPE_ARRAY)
    {
        if (value.vt == DLMS_DATA_TYPE_OCTET_STRING)
        {
            bb.Set(value.byteArr, value.GetSize());
            return 0;
        }
    }
    else
    {
        if (tp == DLMS_DATA_TYPE_NONE)
        {
            tp = value.vt;
            // If data type is not defined for Date Time it is write as
            // octet string.
            if (tp == DLMS_DATA_TYPE_DATETIME)
            {
                tp = DLMS_DATA_TYPE_OCTET_STRING;
            }
        }
    }
    return GXHelpers::SetData(bb, tp, value);
}
