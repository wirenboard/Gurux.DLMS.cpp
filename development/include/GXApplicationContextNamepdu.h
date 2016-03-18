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

#pragma once

#include "GXDLMSSettings.h"

const unsigned char LOGICAL_NAME_OBJECT_ID[7] = {0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x01};
const unsigned char SHORT_NAME_OBJECT_ID[7] = {0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x02};
const unsigned char LOGICAL_NAME_OBJECT_ID_WITH_CIPHERING[7] = {0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x03};
const unsigned char SHORT_NAME_OBJECT_ID_WITH_CIPHERING[7] = {0x60, 0x85, 0x74, 0x05, 0x08, 0x01, 0x04};

class CGXApplicationContextNamePdu
{
private:
    //Constructor
    CGXApplicationContextNamePdu()
    {

    }
public:
    /**
    * Code application context name.
    *
    * @param data
    */
    static void CodeData(CGXDLMSSettings& settings, CGXByteBuffer& data, unsigned char ciphered)
    {
        // Application context name tag
        data.SetUInt8(0xA1);
        // Length of Application context name.
        data.SetUInt8(0x09);
        data.SetUInt8(0x06);
        data.SetUInt8(0x07);
        if (settings.GetUseLogicalNameReferencing())
        {
            if (ciphered)
            {
                data.AddRange(LOGICAL_NAME_OBJECT_ID_WITH_CIPHERING, sizeof(LOGICAL_NAME_OBJECT_ID_WITH_CIPHERING));
            }
            else
            {
                data.AddRange(LOGICAL_NAME_OBJECT_ID, sizeof(LOGICAL_NAME_OBJECT_ID));
            }
        }
        else
        {
            if (ciphered)
            {
                data.AddRange(SHORT_NAME_OBJECT_ID_WITH_CIPHERING, sizeof(SHORT_NAME_OBJECT_ID_WITH_CIPHERING));
            }
            else
            {
                data.AddRange(SHORT_NAME_OBJECT_ID, sizeof(SHORT_NAME_OBJECT_ID));
            }
        }
    }

    /**
     * Encode application context name.
     *
     * @param settings
     *            DMLS settings.
     * @param buff
     *            Byte buffer where data is encoded.
     */
    static int EncodeData(CGXDLMSSettings& settings, CGXByteBuffer& buff)
    {
        int ret;
        unsigned char tag, len;
        if ((ret = buff.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 0xA1)
        {
            return ERROR_CODES_INVALID_TAG;
        }
        // Get length.
        if ((ret = buff.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        if (buff.GetSize() - buff.GetPosition() < len)
        {
            return ERROR_CODES_OUTOFMEMORY;
        }
        if ((ret = buff.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        // Get length.
        if ((ret = buff.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        if (settings.GetUseLogicalNameReferencing())
        {
            if (buff.Compare((unsigned char*) LOGICAL_NAME_OBJECT_ID, sizeof(LOGICAL_NAME_OBJECT_ID)))
            {
                return ERROR_CODES_OK;
            }
            // If ciphering is used.
            if (buff.Compare((unsigned char*) LOGICAL_NAME_OBJECT_ID_WITH_CIPHERING,
                             sizeof(LOGICAL_NAME_OBJECT_ID_WITH_CIPHERING)))
            {
                return ERROR_CODES_OK;
            }
            return ERROR_CODES_INVALID_PARAMETER;
        }
        if (buff.Compare((unsigned char*) SHORT_NAME_OBJECT_ID, sizeof(SHORT_NAME_OBJECT_ID)))
        {
            return ERROR_CODES_OK;
        }
        // If ciphering is used.
        if (buff.Compare((unsigned char*) SHORT_NAME_OBJECT_ID_WITH_CIPHERING, sizeof(SHORT_NAME_OBJECT_ID_WITH_CIPHERING)))
        {
            return ERROR_CODES_OK;
        }
        return ERROR_CODES_INVALID_PARAMETER;
    }
    /*
    int CodeData(CGXDLMSSettings& settings,
            CGXByteBuffer& buff,
            GXDLMS_ASSOCIATION_RESULT result,
            GXDLMS_SOURCE_DIAGNOSTIC diagnostic,
            bool ciphering)
    {
        buff.SetUInt8(0xA1); //Application context name tag
    	buff.SetUInt8(0x09); //Len
    	buff.SetUInt8(0x6);
    	buff.SetUInt8(7); //Len
    	if (UseLN)
    	{
            buff.AddRange(LogicalNameObjectID, 7);
    	}
    	else
    	{
            buff.AddRange(ShortNameObjectID, 7);
    	}
    	return ERROR_CODES_OK;
    }

    //Encode data.
    int EncodeData(unsigned char*& pBuff, int& size)
    {
    	unsigned char* pStart = pBuff;
    	if (pBuff == NULL)
    	{
    		return ERROR_CODES_INVALID_PARAMETER;
    	}
    	int tag = (pBuff[0] & 0xFF);
    	if (tag != 0xA1)
    	{
    		return ERROR_CODES_INVALID_PARAMETER;
    	}
    	++pBuff;
    	//Get length.
    	int len = *pBuff++ & 0xFF;
    	if (size < len)
    	{
    		return ERROR_CODES_ENCODING_FAILED;
    	}
    	pBuff++;
    	len = *pBuff++ & 0xFF;
    	UseLN = memcmp(pBuff, LogicalNameObjectID, 7) == 0;
    	pBuff += 7;
    	size -= (pBuff - pStart);
    	return ERROR_CODES_OK;
    }

    bool UseLN;
    */
};
