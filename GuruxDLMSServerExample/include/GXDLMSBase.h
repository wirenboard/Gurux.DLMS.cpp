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

#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <Winsock.h> //Add support for sockets	
#endif

#include "../../development/include/GXDLMSServer.h"

class CGXDLMSBase : public CGXDLMSServer
{
private:
    SOCKET m_ServerSocket;
public:
    HANDLE m_ReceiverThread;
    SOCKET GetSocket();

    /////////////////////////////////////////////////////////////////////////
    //Constructor.
    /////////////////////////////////////////////////////////////////////////
    CGXDLMSBase(
        bool UseLogicalNameReferencing = true,
        DLMS_INTERFACE_TYPE IntefaceType = DLMS_INTERFACE_TYPE_HDLC) :
        CGXDLMSServer(UseLogicalNameReferencing, IntefaceType), m_ServerSocket(INVALID_SOCKET)
    {
        m_ReceiverThread = INVALID_HANDLE_VALUE;
    }


    /////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////
    ~CGXDLMSBase(void)
    {
        StopServer();
    }

    int StartServer(int port);

    int StopServer();

    int Init(int port);

    CGXDLMSObject* FindObject(
        DLMS_OBJECT_TYPE objectType,
        int sn,
        std::string& ln);

    void Read(
        std::vector<CGXDLMSValueEventArgs*>& args);

    void Write(
        std::vector<CGXDLMSValueEventArgs*>& args);

    void Action(
        std::vector<CGXDLMSValueEventArgs*>& args);

    bool IsTarget(
        unsigned long int serverAddress,
        unsigned long clientAddress);

    DLMS_SOURCE_DIAGNOSTIC ValidateAuthentication(
        DLMS_AUTHENTICATION authentication,
        CGXByteBuffer& password);

    /**
    * Accepted connection is made for the server. All initialization is done
    * here.
    */
    void Connected();

    /**
     * Server has close the connection. All clean up is made here.
     */
    void Disconnected();
};
