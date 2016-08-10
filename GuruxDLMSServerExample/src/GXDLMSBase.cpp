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


#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <tchar.h>
#include <conio.h>
#include <Winsock.h> //Add support for sockets	
#include <time.h>
#include <process.h>//Add support for threads
#else //Linux includes.
#include <stdio.h>
#include <pthread.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/socket.h> //Add support for sockets
#include <unistd.h> //Add support for sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>
#endif

#include "../include/GXDLMSBase.h"
#include "../../development/include/GXDLMSClient.h"
#include "../../development/include/GXDLMSData.h"
#include "../../development/include/GXDLMSRegister.h"
#include "../../development/include/GXDLMSClock.h"
#include "../../development/include/GXDLMSTcpUdpSetup.h"
#include "../../development/include/GXDLMSProfileGeneric.h"
#include "../../development/include/GXDLMSAutoConnect.h"
#include "../../development/include/GXDLMSIECOpticalPortSetup.h"
#include "../../development/include/GXDLMSActivityCalendar.h"
#include "../../development/include/GXDLMSDemandRegister.h"
#include "../../development/include/GXDLMSRegisterMonitor.h"
#include "../../development/include/GXDLMSActionSchedule.h"
#include "../../development/include/GXDLMSSapAssignment.h"
#include "../../development/include/GXDLMSAutoAnswer.h"
#include "../../development/include/GXDLMSModemConfiguration.h"
#include "../../development/include/GXDLMSMacAddressSetup.h"
#include "../../development/include/GXDLMSModemInitialisation.h"
#include "../../development/include/GXDLMSActionSet.h"
#include "../../development/include/GXDLMSIp4Setup.h"
#include "../../development/include/GXDLMSPushSetup.h"

using namespace std;

void ListenerThread(void* pVoid)
{
    CGXByteBuffer reply;
    CGXDLMSBase* server = (CGXDLMSBase*) pVoid;
    sockaddr_in add = {0};
    int ret;
    char tmp[10];
    CGXByteBuffer bb;
    bb.Capacity(2048);
#if defined(_WIN32) || defined(_WIN64)//If Windows
    int len;
    int AddrLen = sizeof(add);
#else //If Linux
    socklen_t len;
    socklen_t AddrLen = sizeof(add);
#endif
    struct sockaddr_in client;
    memset(&client,0,sizeof(client));
    //Get buffer data
    basic_string<char> senderInfo;
    while(server->IsConnected())
    {
        len = sizeof(client);
        senderInfo.clear();
        int socket = accept(server->GetSocket(),(struct sockaddr*)&client, &len);
        if (server->IsConnected())
        {
            if ((ret = getpeername(socket, (sockaddr*) &add, &AddrLen)) == -1)
            {
#if defined(_WIN32) || defined(_WIN64)//If Windows
                closesocket(socket);
                socket = INVALID_SOCKET;
#else //If Linux
                close(socket);
                socket = -1;
#endif
                continue;
                //Notify error.
            }
            senderInfo = inet_ntoa(add.sin_addr);
            senderInfo.append(":");
#if _MSC_VER > 1000
            _ltoa_s(add.sin_port, tmp, 10, 10);
#else
            sprintf(tmp, "%d", add.sin_port);
#endif
            senderInfo.append(tmp);
            while (server->IsConnected())
            {
                //If client is left wait for next client.
                if ((ret = recv(socket, (char*)
                                bb.GetData() + bb.GetSize(),
                                bb.Capacity() - bb.GetSize(), 0)) == -1)
                {
                    //Notify error.
                    server->Reset();
#if defined(_WIN32) || defined(_WIN64)//If Windows
                    closesocket(socket);
                    socket = INVALID_SOCKET;
#else //If Linux
                    close(socket);
                    socket = -1;
#endif
                    break;
                }
                //If client is closed the connection.
                if (ret == 0)
                {
                    server->Reset();
#if defined(_WIN32) || defined(_WIN64)//If Windows
                    closesocket(socket);
                    socket = INVALID_SOCKET;
#else //If Linux
                    close(socket);
                    socket = -1;
#endif
                    break;
                }
                bb.SetSize(bb.GetSize() + ret);
                if (server->HandleRequest(bb, reply) != 0)
                {
#if defined(_WIN32) || defined(_WIN64)//If Windows
                    closesocket(socket);
                    socket = INVALID_SOCKET;
#else //If Linux
                    close(socket);
                    socket = -1;
#endif
                }
                bb.SetSize(0);
                if (reply.GetSize() != 0)
                {
                    if (send(socket, (const char*) reply.GetData(), reply.GetSize() - reply.GetPosition(), 0) == -1)
                    {
                        //If error has occured
                        server->Reset();
#if defined(_WIN32) || defined(_WIN64)//If Windows
                        closesocket(socket);
                        socket = INVALID_SOCKET;
#else //If Linux
                        close(socket);
                        socket = -1;
#endif
                    }
                    reply.Clear();
                }
            }
            server->Reset();
        }
    }
}

#if defined(_WIN32) || defined(_WIN64)//If Windows
#else //If Linux
void * UnixListenerThread(void * pVoid)
{
    ListenerThread(pVoid);
    return NULL;
}
#endif

bool CGXDLMSBase::IsConnected()
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    return m_ServerSocket != INVALID_SOCKET;
#else //If Linux
    return m_ServerSocket != -1;
#endif
}

int CGXDLMSBase::GetSocket()
{
    return m_ServerSocket;
}

int CGXDLMSBase::StartServer(int port)
{
    int ret;
    if ((ret = StopServer()) != 0)
    {
        return ret;
    }
    m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IsConnected())
    {
        //socket creation.
        return -1;
    }
    int fFlag = 1;
    if (setsockopt(m_ServerSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag)) == -1)
    {
        //setsockopt.
        return -1;
    }
    sockaddr_in add = {0};
    add.sin_port = htons(port);
    add.sin_addr.s_addr = htonl(INADDR_ANY);
#if defined(_WIN32) || defined(_WIN64)//Windows includes
    add.sin_family = AF_INET;
#else
    add.sin_family = AF_INET;
#endif
    if((ret = ::bind(m_ServerSocket, (sockaddr*) &add, sizeof(add))) == -1)
    {
        //bind;
        return -1;
    }
    if((ret = listen(m_ServerSocket, 1)) == -1)
    {
        //socket listen failed.
        return -1;
    }
#if defined(_WIN32) || defined(_WIN64)//Windows includes
    m_ReceiverThread = (HANDLE) _beginthread(ListenerThread, 0, (LPVOID) this);
#else
    ret = pthread_create(&m_ReceiverThread, NULL, UnixListenerThread, (void *) this);
#endif
    return ret;
}

int CGXDLMSBase::StopServer()
{
    if (IsConnected())
    {
#if defined(_WIN32) || defined(_WIN64)//Windows includes
        closesocket(m_ServerSocket);
        m_ServerSocket = INVALID_SOCKET;
        if (m_ReceiverThread != INVALID_HANDLE_VALUE)
        {
            int ret = ::WaitForSingleObject(m_ReceiverThread, 5000);
            m_ReceiverThread = INVALID_HANDLE_VALUE;
        }
#else
        close(m_ServerSocket);
        m_ServerSocket = -1;
        void *res;
        pthread_join(m_ReceiverThread, (void **)&res);
        free(res);
#endif
    }
    return 0;
}

int GetIpAddress(std::string& address)
{
    int ret;
#if defined(_WIN32) || defined(_WIN64)//If Windows 
    struct hostent *phe;
    char ac[80];
    if ((ret = gethostname(ac, sizeof(ac))) != SOCKET_ERROR)
    {
        phe = gethostbyname(ac);
        if (phe == 0)
        {
            ret = -1;
        }
        else
        {
            struct in_addr* addr = (struct in_addr*)phe->h_addr_list[0];
            address = inet_ntoa(*addr);
        }
    }
#else if defined(__linux__)

#endif
    return ret;
}

/// <summary>
/// Generic initialize for all servers.
/// </summary>
/// <param name="server"></param>
int CGXDLMSBase::Init(int port)
{
    int ret;
    if ((ret = StartServer(port)) != 0)
    {
        return ret;
    }
    CGXDLMSRegister *MYOBJ = new CGXDLMSRegister("1.1.12.27.0.255");//object 1
    CGXDLMSVariant MYValues;
    MYValues = (unsigned short) 22;
    MYOBJ->SetScaler(0);
    MYOBJ->SetUnit(35);
    MYOBJ->SetValue(MYValues);
    MYOBJ->SetAccess(2, DLMS_ACCESS_MODE_READ);
    GetItems().push_back(MYOBJ);
    CGXDLMSRegister *MYOBJ1 = new CGXDLMSRegister("1.1.12.27.22.255");//object2
    CGXDLMSVariant MYValues1;
    MYValues1 = (unsigned short) 55;
    MYOBJ1->SetScaler(0);
    MYOBJ1->SetUnit(33);
    MYOBJ1->SetValue(MYValues1);
    MYOBJ1->SetAccess(2, DLMS_ACCESS_MODE_READ);
    GetItems().push_back(MYOBJ1);
    ///////////////////////////////////////////////////////////////////////
    //Server must initialize after all objects are added.
    ret = Initialize();
    if (ret != DLMS_ERROR_CODE_OK)
    {
        return ret;
    }
    return DLMS_ERROR_CODE_OK;
}

CGXDLMSObject* CGXDLMSBase::FindObject(
    DLMS_OBJECT_TYPE objectType,
    int sn,
    std::string& ln)
{
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Read(std::vector<CGXDLMSValueEventArg*>& args)
{
    CGXDLMSVariant value;
    CGXDLMSObject* pObj;
    int ret, index;
    DLMS_OBJECT_TYPE type;
    std::string ln;
    for(std::vector<CGXDLMSValueEventArg*>::iterator it = args.begin(); it != args.end(); ++it)
    {
        //Let framework handle Logical Name read.
        if ((*it)->GetIndex() == 1)
        {
            continue;
        }
        //Get attribute index.
        index = (*it)->GetIndex();
        pObj = (*it)->GetTarget();
        //Get target type.
        type = pObj->GetObjectType();
        if (type == DLMS_OBJECT_TYPE_PROFILE_GENERIC && index == 2)
        {
            CGXDLMSProfileGeneric* pg = (CGXDLMSProfileGeneric*) pObj;
            std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> > columns;
            if ((ret = pg->GetSelectedColumns((*it)->GetSelector(), (*it)->GetParameters(), columns)) != 0)
            {
                continue;
            }
            //Show columns.
            for(std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it = columns.begin();
                    it != columns.end(); ++it)
            {
                it->first->GetLogicalName(ln);
                printf("%s : %s, ", CGXDLMSClient::ObjectTypeToString(it->first->GetObjectType()).c_str(), ln.c_str());
            }
            if (columns.size() != 0)
            {
                printf("\r\n");
            }
        }
        //Framework will handle Association objects automatically.
        if ( type== DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME ||
                type == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME ||
                //Framework will handle profile generic automatically.
                type == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            continue;
        }
        DLMS_DATA_TYPE ui, dt;
        (*it)->GetTarget()->GetUIDataType(index, ui);
        (*it)->GetTarget()->GetDataType(index, dt);
        //Update date and time of clock object.
        if (type == DLMS_OBJECT_TYPE_CLOCK && index == 2)
        {
            CGXDateTime tm = CGXDateTime::Now();
            ((CGXDLMSClock*)pObj)->SetTime(tm);
            continue;
        }
        else if (type == DLMS_OBJECT_TYPE_REGISTER_MONITOR)
        {
            CGXDLMSRegisterMonitor* pRm = (CGXDLMSRegisterMonitor*) pObj;
            if (index == 2)
            {
                //Initialize random seed.
                srand ((unsigned int)time(NULL));
                pRm->GetThresholds().clear();
                pRm->GetThresholds().push_back(rand() % 100 + 1);
                continue;
            }
        }
        else
        {
            CGXDLMSVariant null;
            CGXDLMSValueEventArg e(pObj, index);
            ret = ((IGXDLMSBase*) pObj)->GetValue(m_Settings, e);
            if (ret != DLMS_ERROR_CODE_OK)
            {
                //TODO: Show error.
                continue;
            }
            //If data is not assigned and value type is unknown return number.
            DLMS_DATA_TYPE tp = e.GetValue().vt;
            if (tp == DLMS_DATA_TYPE_INT8 ||
                    tp == DLMS_DATA_TYPE_INT16 ||
                    tp == DLMS_DATA_TYPE_INT32 ||
                    tp == DLMS_DATA_TYPE_INT64 ||
                    tp == DLMS_DATA_TYPE_UINT8 ||
                    tp == DLMS_DATA_TYPE_UINT16 ||
                    tp == DLMS_DATA_TYPE_UINT32 ||
                    tp == DLMS_DATA_TYPE_UINT64)
            {
                //Initialize random seed.
                srand ((unsigned int)time(NULL));
                value = rand() % 100 + 1;
                value.vt = tp;
                e.SetValue(value);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Write(std::vector<CGXDLMSValueEventArg*>& args)
{
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Action(std::vector<CGXDLMSValueEventArg*>& args)
{
}

bool CGXDLMSBase::IsTarget(
    unsigned long int serverAddress,
    unsigned long clientAddress)
{
    return true;
}

DLMS_SOURCE_DIAGNOSTIC CGXDLMSBase::ValidateAuthentication(
    DLMS_AUTHENTICATION authentication,
    CGXByteBuffer& password)
{
    return DLMS_SOURCE_DIAGNOSTIC_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Connected()
{
    printf("Connected. PDU size: %d\r\n", m_Settings.GetMaxReceivePDUSize());
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Disconnected()
{
    printf("Disconnected.\r\n");
}
