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

#ifndef GXREPLYDATA_H
#define GXREPLYDATA_H

#include "GXDLMSConverter.h"

class CGXReplyData
{
private:
    /**
     * Is more data available.
     */
    GXDLMS_DATA_REQUEST_TYPES m_MoreData;
    /**
     * Received command.
     */
    DLMS_COMMAND m_Command;
    /**
     * Received data.
     */
    CGXByteBuffer m_Data;
    /**
     * Is frame complete.
     */
    bool m_Complete;
    /**
     * Received error.
     */
    short m_Error;

    /**
     * Read value.
     */
    CGXDLMSVariant m_DataValue;

    /**
     * Expected count of element in the array.
     */
    int m_TotalCount;

    /**
     * Last read position. This is used in peek to solve how far data is read.
     */
    int m_ReadPosition;

    /**
     * Block length.
     */
    int m_BlockLength;

    /**
     * Try Get value.
     */
    bool m_Peek;

    /**
     * Client address.
     */
    int m_ClientAddress;

    /**
     * Server address.
     */
    int m_ServerAddress;

    DLMS_DATA_TYPE m_DataType;

public:
    /**
     * Constructor.
     *
     * @param more
     *            Is more data available.
     * @param cmd
     *            Received command.
     * @param buff
     *            Received data.
     * @param forComplete
     *            Is frame complete.
     * @param err
     *            Received error ID.
     */
    CGXReplyData(
        GXDLMS_DATA_REQUEST_TYPES more,
        DLMS_COMMAND cmd,
        CGXByteBuffer& buff,
        bool complete,
        unsigned char error)
    {
        Clear();
        m_DataType = DLMS_DATA_TYPE_NONE;
        m_MoreData = more;
        m_Command = cmd;
        m_Data = buff;
        m_Complete = complete;
        m_Error = error;
    }

    /**
     * Constructor.
     */
    CGXReplyData()
    {
        Clear();
    }

    DLMS_DATA_TYPE GetValueType()
    {
        return m_DataType;
    }

    void SetValueType(DLMS_DATA_TYPE value)
    {
        m_DataType = value;
    }

    CGXDLMSVariant& GetValue()
    {
        return m_DataValue;
    }

    void SetValue(CGXDLMSVariant& value)
    {
        m_DataValue = value;
    }

    int GetReadPosition()
    {
        return m_ReadPosition;
    }

    void SetReadPosition(int value)
    {
        m_ReadPosition = value;
    }

    int GetBlockLength()
    {
        return m_BlockLength;
    }

    void SetBlockLength(int value)
    {
        m_BlockLength = value;
    }

    int GetClientAddress()
    {
        return m_ClientAddress;
    }

    void SetClientAddress(int value)
    {
        m_ClientAddress = value;
    }

    int GetServerAddress()
    {
        return m_ServerAddress;
    }

    void SetServerAddress(int value)
    {
        m_ServerAddress = value;
    }

    void SetCommand(DLMS_COMMAND value)
    {
        m_Command = value;
    }

    void SetData(CGXByteBuffer& value)
    {
        m_Data = value;
    }

    void SetComplete(bool value)
    {
        m_Complete = value;
    }

    void SetError(short value)
    {
        m_Error = value;
    }

    void SetTotalCount(int value)
    {
        m_TotalCount = value;
    }

    /**
     * Reset data values to default.
     */
    void Clear()
    {
        m_MoreData = GXDLMS_DATA_REQUEST_TYPES_NONE;
        m_Command = DLMS_COMMAND_NONE;
        m_Data.Clear();
        m_Complete = false;
        m_Peek = false;
        m_Error = 0;
        m_TotalCount = 0;
        m_DataValue.Clear();
        m_ReadPosition = 0;
        m_BlockLength = 0;
        m_DataType = DLMS_DATA_TYPE_NONE;
    }

    /**
     * @return Is more data available.
     */
    bool IsMoreData()
    {
        return m_MoreData != GXDLMS_DATA_REQUEST_TYPES_NONE && m_Error == 0;
    }

    /**
     * Is more data available.
     *
     * @return Return None if more data is not available or Frame or Block type.
     */
    GXDLMS_DATA_REQUEST_TYPES GetMoreData()
    {
        return m_MoreData;
    }

    void SetMoreData(GXDLMS_DATA_REQUEST_TYPES value)
    {
        m_MoreData = value;
    }

    /**
     * Get received command.
     *
     * @return Received command.
     */
    DLMS_COMMAND GetCommand()
    {
        return m_Command;
    }

    /**
     * Get received data.
     *
     * @return Received data.
     */
    CGXByteBuffer& GetData()
    {
        return m_Data;
    }

    /**
     * Is frame complete.
     *
     * @return Returns true if frame is complete or false if bytes is missing.
     */
    bool IsComplete()
    {
        return m_Complete;
    }

    /**
     * Get Received error. Value is zero if no error has occurred.
     *
     * @return Received error.
     */
    short GetError()
    {
        return m_Error;
    }

    std::string GetErrorMessage()
    {
        return CGXDLMSConverter::GetErrorMessage(m_Error);
    }

    /**
     * Get total count of element in the array. If this method is used peek must
     * be Set true.
     *
     * @return Count of element in the array.
     * @see peek
     * @see GetCount
     */
    int GetTotalCount()
    {
        return m_TotalCount;
    }

    /**
     * Get count of read elements. If this method is used peek must be Set true.
     *
     * @return Count of read elements.
     * @see peek
     * @see GetTotalCount
     */
    int GetCount()
    {
        if (m_DataValue.vt == DLMS_DATA_TYPE_ARRAY)
        {
            return m_DataValue.Arr.size();
        }
        return 0;
    }

    /**
     * Get is value try to peek.
     *
     * @return Is value try to peek.
     * @see GetCount
     * @see GetTotalCount
     */
    bool GetPeek()
    {
        return m_Peek;
    }

    /**
     * Set is value try to peek.
     *
     * @param value
     *            Is value try to peek.
     */
    void SetPeek(bool value)
    {
        m_Peek = value;
    }
};

#endif //GXREPLYDATA_H