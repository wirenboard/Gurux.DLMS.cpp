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

#ifndef GXUSERINFORMATION_H
#define GXUSERINFORMATION_H

#include "GXDLMSSettings.h"

const char INITIAL_REQUEST = 0x1;
const char INITIAL_RESPONSE = 0x8;

//User Information field is optional.
class CGXUserInformation
{
private:
    CGXUserInformation()
    {
    }
public:
    //User information code is special case...
    static int CodeData(CGXDLMSSettings& settings, CGXByteBuffer& data)
    {
        data.SetUInt8(0xBE); //Tag
        data.SetUInt8(0x10); //Length for AARQ user field
        data.SetUInt8(0x04); //Coding the choice for user-information (Octet STRING, universal)
        data.SetUInt8(0x0E); //Length
        data.SetUInt8(INITIAL_REQUEST); // Tag for xDLMS-Initiate request
        data.SetUInt8(0x00); // Usage field for dedicated-key component not used
        data.SetUInt8(0x00); // Usage field for the response allowed component not used
        data.SetUInt8(0x00); // Usage field of the proposed-quality-of-service component not used
        data.SetUInt8(settings.GetDlmsVersionNumber());
        // Tag for conformance block
        data.SetUInt8(0x5F);
        data.SetUInt8(0x1F);
        // length of the conformance block
        data.SetUInt8(0x04);
        // encoding the number of unused bits in the bit std::string
        data.SetUInt8(0x00);
        if (settings.GetUseLogicalNameReferencing())
        {
            data.AddRange(settings.GetLnSettings().m_ConformanceBlock, 3);
        }
        else
        {
            data.AddRange(settings.GetSnSettings().m_ConformanceBlock, 3);
        }
        data.SetUInt16(settings.GetMaxReceivePDUSize());
        return ERROR_CODES_OK;
    }

    static int EncodeData(CGXDLMSSettings& settings, CGXByteBuffer& data)
    {
        int ret;
        unsigned short pduSize;
        unsigned char tag, len;
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 0xBE)
        {
            return ERROR_CODES_INVALID_TAG;
        }
        if ((ret = data.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        if (data.GetSize() - data.GetPosition() < len)
        {
            //Not enought data.
            return ERROR_CODES_OUTOFMEMORY;
        }
        // Encoding the choice for user information
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 0x4)
        {
            return ERROR_CODES_INVALID_TAG;
        }
        // Skip
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        // Tag for xDLMS-Initate.response
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }

        unsigned char response = tag == INITIAL_RESPONSE;
        if (response)
        {
            // Optional usage field of the negotiated quality of service component
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            // Skip if used.
            if (tag != 0)
            {
                if ((ret = data.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                data.SetPosition(data.GetPosition() + len);
            }
        }
        else if (tag == INITIAL_REQUEST)
        {
            // Optional usage field of the negotiated quality of service
            // component
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            // Skip if used.
            if (tag != 0)
            {
                if ((ret = data.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                data.SetPosition(data.GetPosition() + len);
            }
            // Optional usage field of the negotiated quality of service
            // component
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            // Skip if used.
            if (tag != 0)
            {
                if ((ret = data.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                data.SetPosition(data.GetPosition() + len);
            }
            // Optional usage field of the negotiated quality of service
            // component
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
            // Skip if used.
            if (tag != 0)
            {
                if ((ret = data.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                data.SetPosition(data.GetPosition() + len);
            }
        }
        else
        {
            return ERROR_CODES_INVALID_PARAMETER;
        }
        // Get DLMS version number.
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 6)
        {
            //Invalid DLMS version number.
            return ERROR_CODES_INVALID_VERSION_NUMBER;
        }
        // Tag for conformance block
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != 0x5F)
        {
            return ERROR_CODES_INVALID_TAG;
        }
        // Old Way...
        if ((ret = data.GetUInt8(data.GetPosition(), &tag)) != 0)
        {
            return ret;
        }
        if (tag == 0x1F)
        {
            if ((ret = data.GetUInt8(&tag)) != 0)
            {
                return ret;
            }
        }
        // The number of unused bits in the bit std::string.
        if ((ret = data.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        // Get tag
        if ((ret = data.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (settings.GetUseLogicalNameReferencing())
        {
            if (settings.IsServer())
            {
                // Skip settings what client asks.
                // All server settings are always returned.
                unsigned char tmp[3];
                data.Get(tmp, 3);
            }
            else
            {
                data.Get(settings.GetLnSettings().m_ConformanceBlock, 3);
            }
        }
        else
        {
            if (settings.IsServer())
            {
                // Skip settings what client asks.
                // All server settings are always returned.
                unsigned char tmp[3];
                data.Get(tmp, 3);
            }
            else
            {
                data.Get(settings.GetLnSettings().m_ConformanceBlock, 3);
            }
        }
        if ((ret = data.GetUInt16(&pduSize)) != 0)
        {
            return ret;
        }
        if (!settings.IsServer())
        {
            settings.SetMaxReceivePDUSize(pduSize);
        }
        // get VAA Name
        if (response)
        {
            if ((ret = data.GetUInt16(&pduSize)) != 0)
            {
                return ret;
            }
            if (pduSize == 0x0007)
            {
                // If LN
                if (!settings.GetUseLogicalNameReferencing())
                {
                    return ERROR_CODES_INVALID_PARAMETER;
                }
            }
            else if (pduSize == 0xFA00)
            {
                // If SN
                if (settings.GetUseLogicalNameReferencing())
                {
                    return ERROR_CODES_INVALID_PARAMETER;
                }
            }
            else
            {
                // Unknown VAA.
                return ERROR_CODES_INVALID_PARAMETER;
            }
        }
        return ERROR_CODES_OK;
    }
};

#endif //GXUSERINFORMATION_H