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

#ifndef GXAPDU_H
#define GXAPDU_H

#include <vector>
#include "enums.h"
#include "errorcodes.h"
#include "GXHelpers.h"
#include "GXApplicationContextNamepdu.h"
#include "GXUserInformation.h"
#include "GXDLMSSettings.h"

/////////////////////////////////////////////////////////////////////////////
// The services to access the attributes and methods of COSEM objects are
// determined on DLMS/COSEM Application layer. The services are carried by
// Application Protocol Data Units (APDUs).
// <p />
// In DLMS/COSEM the meter is primarily a server, and the controlling system is
// a client. Also unsolicited (received without a request) messages are
// available.
/////////////////////////////////////////////////////////////////////////////
class GXAPDU
{
private :
    GXDLMS_ASSOCIATION_RESULT m_ResultValue;
    GXDLMS_SOURCE_DIAGNOSTIC m_ResultDiagnostic;
public:
    /////////////////////////////////////////////////////////////////////////////
    // Constructor.
    /////////////////////////////////////////////////////////////////////////////
    GXAPDU()
    {
        m_ResultValue = GXDLMS_ASSOCIATION_RESULT_ACCEPTED;
        m_ResultDiagnostic = GXDLMS_SOURCE_DIAGNOSTIC_NULL;
    }
    /////////////////////////////////////////////////////////////////////////////
    // AssociationResult
    /////////////////////////////////////////////////////////////////////////////
    GXDLMS_ASSOCIATION_RESULT GetResultComponent()
    {
        return m_ResultValue;
    }

    /////////////////////////////////////////////////////////////////////////////
    // SourceDiagnostic
    /////////////////////////////////////////////////////////////////////////////
    GXDLMS_SOURCE_DIAGNOSTIC GetResultDiagnostic()
    {
        return m_ResultDiagnostic;
    }

    /////////////////////////////////////////////////////////////////////////////
    // SourceDiagnostic
    /////////////////////////////////////////////////////////////////////////////
    void SetResultDiagnosticValue(GXDLMS_SOURCE_DIAGNOSTIC value)
    {
        m_ResultDiagnostic = value;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Retrieves the std::string that indicates the level of authentication, if any.
    /////////////////////////////////////////////////////////////////////////////
    static void GetAuthenticationString(
        CGXDLMSSettings& settings,
        CGXByteBuffer& data,
        CGXByteBuffer& challenge)
    {
        // If authentication is used.
        if (settings.GetAuthentication() != GXDLMS_AUTHENTICATION_NONE)
        {
            // Add sender ACSE-requirenents field component.
            data.SetUInt8(0x8A);
            data.SetUInt8(2);
            data.SetUInt16(0x0780);
            data.SetUInt8(0x8B);
            data.SetUInt8(7);
            unsigned char p[] = { 0x60, 0x85, 0x74, 0x05, 0x08, 0x02};
            data.AddRange(p, sizeof(p));
            data.SetUInt8(settings.GetAuthentication());
            // Add Calling authentication information.
            int len = 0;
            if (settings.GetPassword().GetSize())
            {
                len = settings.GetPassword().GetSize();
            }
            if (settings.GetAuthentication() == GXDLMS_AUTHENTICATION_LOW)
            {
                len = settings.GetPassword().GetSize();
            }
            else
            {
                len = challenge.GetSize();
            }
            data.SetUInt8(0xAC);
            data.SetUInt8((2 + len));
            // Add authentication information.
            data.SetUInt8(0x80);
            data.SetUInt8(len);
            if (challenge.GetSize() != 0)
            {
                data.AddRange2(&challenge, 0, -1);
            }
            else if (settings.GetPassword().GetSize() != 0)
            {
                data.AddRange2(&settings.GetPassword(), 0, -1);
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////
    // codeData
    //
    /////////////////////////////////////////////////////////////////////////////
    void CodeData(
        CGXDLMSSettings& settings,
        bool ciphering,
        CGXByteBuffer& data)
    {
        // AARQ APDU Tag
        data.SetUInt8(AARQ_TAG);
        // Length
        int lenPos = data.GetSize();
        data.SetUInt8(0);
        ///////////////////////////////////////////
        // Add Application context name.
        CGXApplicationContextNamePdu::CodeData(settings, data, ciphering);
        GetAuthenticationString(settings, data, settings.GetCtoSChallenge());
        CGXUserInformation::CodeData(settings, data);
        data.SetUInt8(lenPos, data.GetSize() - lenPos - 1);
    }

    /////////////////////////////////////////////////////////////////////////////
    // EncodeData
    /////////////////////////////////////////////////////////////////////////////
    int EncodeData(CGXDLMSSettings& settings, CGXByteBuffer& buff)
    {
        int ret;
        unsigned char tag, len;
        // Get AARE tag and length
        if ((ret = buff.GetUInt8(&tag)) != 0)
        {
            return ret;
        }
        if (tag != AARE_TAG && tag != AARQ_TAG &&
                tag != 0x81 && tag != 0x80)
        {
            return ERROR_CODES_INVALID_TAG;
        }
        if ((ret = buff.GetUInt8(&len)) != 0)
        {
            return ret;
        }
        int size = buff.GetSize() - buff.GetPosition();
        if (len > size)
        {
            return ERROR_CODES_OUTOFMEMORY;
        }
        while (buff.GetPosition() < buff.GetSize())
        {
            if ((ret = buff.GetUInt8(buff.GetPosition(), &tag)) != 0)
            {
                return ret;
            }
            if (tag == 0xA1)
            {
                if ((ret = HandleApplicationContextName(settings, buff)) != 0)
                {
                    return ret;
                }
            }
            else if (tag == 0xBE)
            {
                CGXUserInformation::EncodeData(settings, buff);
            }
            else if (tag == 0xA2)
            {
                // Result
                // Get tag
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                // Get length.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                // Choice for result (INTEGER, universal)
                // Get tag
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                // Get length.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                m_ResultValue = (GXDLMS_ASSOCIATION_RESULT) tag;
            }
            else if (tag == 0xA3)
            {
                // SourceDiagnostic
                // Get tag
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                // Get length.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                // ACSE service user tag.
                // Get tag
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                // Get length.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                // Result source diagnostic component.
                // Get tag
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                // Get length.
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                m_ResultDiagnostic = (GXDLMS_SOURCE_DIAGNOSTIC) tag;
            }
            else if (tag == 0x8A || tag == 0x88)
            {
                // GXDLMS_AUTHENTICATION_
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                // Get sender ACSE-requirenents field component.
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 2)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                unsigned short val;
                if ((ret = buff.GetUInt16(&val)) != 0)
                {
                    return ret;
                }

                if (val != 0x0780 && val != 0x0680)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
            }
            else if (tag == 0xAA)
            {
                // Server Challenge.
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                CGXByteBuffer challenge;
                challenge.AddRange2(&buff, 0, len);
                settings.SetCtoSChallenge(challenge);
            }
            else if (tag == 0x8B || tag == 0x89)
            {
                // Authentication.
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 0x60)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 0x85)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 0x74)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 0x05)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 0x08)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag != 0x02)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if (tag < 0 || tag > 5)
                {
                    return ERROR_CODES_INVALID_TAG;
                }
                GXDLMS_AUTHENTICATION auth = (GXDLMS_AUTHENTICATION) tag;
                settings.SetAuthentication(auth);
                if (tag != 0)
                {
                    if ((ret = buff.GetUInt8(&tag)) != 0)
                    {
                        return ret;
                    }
                    if (tag != 0xAC && tag != 0xAA)
                    {
                        return ERROR_CODES_INVALID_TAG;
                    }
                    if ((ret = buff.GetUInt8(&len)) != 0)
                    {
                        return ret;
                    }
                    // Get authentication information.
                    if ((ret = buff.GetUInt8(&tag)) != 0)
                    {
                        return ret;
                    }
                    if (tag != 0x80)
                    {
                        return ERROR_CODES_INVALID_TAG;
                    }
                    if ((ret = buff.GetUInt8(&len)) != 0)
                    {
                        return ret;
                    }
                    CGXByteBuffer tmp2;
                    tmp2.AddRange2(&buff, 0, len);
                    if (auth < 2)
                    {
                        settings.SetPassword(tmp2);
                    }
                    else
                    {
                        if (settings.IsServer())
                        {
                            settings.SetCtoSChallenge(tmp2);
                        }
                        else
                        {
                            settings.SetStoCChallenge(tmp2);
                        }
                    }
                }
            }
            else
            {
                // Unknown tags.
                if ((ret = buff.GetUInt8(&tag)) != 0)
                {
                    return ret;
                }
                if ((ret = buff.GetUInt8(&len)) != 0)
                {
                    return ret;
                }
                buff.SetPosition(buff.GetPosition() + len);
            }
        }
        return 0;
    }

    int HandleApplicationContextName(CGXDLMSSettings& settings, CGXByteBuffer& buff)
    {
        int ret;
        if ((ret = CGXApplicationContextNamePdu::EncodeData(settings, buff)) != ERROR_CODES_OK)
        {
            m_ResultValue = GXDLMS_ASSOCIATION_RESULT_REJECTED_PERMAMENT;
            m_ResultDiagnostic = GXDLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE;
            return ret;
        }
        return ret;
    }

    /////////////////////////////////////////////////////////////////////////////
    // Server generates AARE message.
    /////////////////////////////////////////////////////////////////////////////
    void GenerateAare(CGXDLMSSettings& settings,
                      CGXByteBuffer& data,
                      GXDLMS_ASSOCIATION_RESULT result,
                      GXDLMS_SOURCE_DIAGNOSTIC diagnostic,
                      bool ciphering)
    {
        int offset = data.GetPosition();
        // Set AARE tag and length
        data.SetUInt8(AARE_TAG);
        data.SetUInt8(0); // Length is updated later.
        CGXApplicationContextNamePdu::CodeData(settings, data, ciphering);
        // Result
        data.SetUInt8(0xA2);
        data.SetUInt8(3); // len
        data.SetUInt8(2); // Tag
        // Choice for result (INTEGER, universal)
        data.SetUInt8(1); // Len
        data.SetUInt8(result); // ResultValue
        // SourceDiagnostic
        data.SetUInt8(0xA3);
        data.SetUInt8(5); // len
        data.SetUInt8(0xA1); // Tag
        data.SetUInt8(3); // len
        data.SetUInt8(2); // Tag
        // Choice for result (INTEGER, universal)
        data.SetUInt8(1); // Len
        data.SetUInt8(diagnostic); // diagnostic
        if (result != GXDLMS_ASSOCIATION_RESULT_REJECTED_PERMAMENT
                && diagnostic == GXDLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_REQUIRED)
        {
            // Add server ACSE-requirenents field component.
            data.SetUInt8(0x88);
            // Length.
            data.SetUInt8(0x02);
            data.SetUInt16(0x0780);
            // Add tag.
            data.SetUInt8(0x89);
            // Length.
            data.SetUInt8(0x07);
            data.SetUInt8(0x60);
            data.SetUInt8(0x85);
            data.SetUInt8(0x74);
            data.SetUInt8(0x05);
            data.SetUInt8(0x08);
            data.SetUInt8(0x02);
            data.SetUInt8(settings.GetAuthentication());
            // Add tag.
            data.SetUInt8(0xAA);
            // Tag length.
            data.SetUInt8((2 + settings.GetStoCChallenge().GetSize()));
            data.SetUInt8(0x80);
            // Challenge Length.
            data.SetUInt8(settings.GetStoCChallenge().GetSize());
            data.AddRange2(&settings.GetStoCChallenge(), 0, -1);
        }
        // Add User Information Tag
        data.SetUInt8(0xBE);
        // Length for AARQ user field
        data.SetUInt8(0x10);
        // Coding the choice for user-information (Octet STRING, universal)
        data.SetUInt8(0x04);
        // Length
        data.SetUInt8(0xE);
        // Tag for xDLMS-Initiate response
        data.SetUInt8(INITIAL_RESPONSE);
        // Usage field for the response allowed component not used
        data.SetUInt8(0x00);
        data.SetUInt8(6); // DLMSVersioNumber
        data.SetUInt8(0x5F);
        data.SetUInt8(0x1F);
        // Length of the conformance block.
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
        // VAA Name VAA name (0x0007 for LN referencing and 0xFA00 for SN)
        if (settings.GetUseLogicalNameReferencing())
        {
            data.SetUInt16(0x0007);
        }
        else
        {
            data.SetUInt16(0xFA00);
        }
        data.SetUInt32ByIndex(offset + 1, data.GetSize() - offset - 2);
    }
};

#endif //GXAPDU_H