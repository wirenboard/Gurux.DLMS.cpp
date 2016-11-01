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
#define closesocket close
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
#include <netdb.h>
#endif

#include "../include/GXDLMSBase.h"

#include "../../development/include/GXTime.h"
#include "../../development/include/GXDate.h"
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
                closesocket(socket);
#if defined(_WIN32) || defined(_WIN64)//If Windows
                socket = INVALID_SOCKET;
#else //If Linux
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
                printf("-> %s\r\n", bb.ToHexString().c_str());

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
                    printf("<- %s\r\n", reply.ToHexString().c_str());
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
    struct hostent *phe;
    char ac[80];
    if ((ret = gethostname(ac, sizeof(ac))) == 0)
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
    return ret;
}

///////////////////////////////////////////////////////////////////////
//Add Logical Device Name. 123456 is meter serial number.
///////////////////////////////////////////////////////////////////////
// COSEM Logical Device Name is defined as an octet-string of 16 octets.
// The first three octets uniquely identify the manufacturer of the device and it corresponds
// to the manufacturer's identification in IEC 62056-21.
// The following 13 octets are assigned by the manufacturer.
//The manufacturer is responsible for guaranteeing the uniqueness of these octets.
CGXDLMSData* AddLogicalDeviceName(CGXDLMSObjectCollection& items, unsigned long sn)
{
    char buff[17];
    sprintf(buff, "GRX%.13d", sn);
    CGXDLMSVariant id;
    id.Add((const char*) buff, 16);
    CGXDLMSData* ldn = new CGXDLMSData("0.0.42.0.0.255", id);
    items.push_back(ldn);
    return ldn;
}

/*
* Add firmware version.
*/
void AddFirmwareVersion(CGXDLMSObjectCollection& items)
{
    CGXDLMSVariant version;
    version = "Gurux FW 0.0.1";
    CGXDLMSData* fw = new CGXDLMSData("1.0.0.2.0.255");
    fw->SetValue(version);
    items.push_back(fw);
}

/*
* Add Electricity ID 1.
*/
void AddElectricityID1(CGXDLMSObjectCollection& items, unsigned long sn)
{
    char buff[17];
    sprintf(buff, "GRX%.13d", sn);
    CGXDLMSVariant id;
    id.Add((const char*) buff, 16);
    CGXDLMSData* d = new CGXDLMSData("1.1.0.0.0.255", id);
    d->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_STRING));
    items.push_back(d);
}

/*
* Add Electricity ID 2.
*/
void AddElectricityID2(CGXDLMSObjectCollection& items, unsigned long sn)
{
    CGXDLMSVariant id2(sn);
    CGXDLMSData* d = new CGXDLMSData("1.1.0.0.1.255", id2);
    d->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_UINT32));
    items.push_back(d);
}

/*
* Add Auto connect object.
*/
void AddAutoConnect(CGXDLMSObjectCollection& items)
{
    CGXDLMSAutoConnect* pAC = new CGXDLMSAutoConnect();
    pAC->SetMode(AUTO_CONNECT_MODE_AUTO_DIALLING_ALLOWED_ANYTIME);
    pAC->SetRepetitions(10);
    pAC->SetRepetitionDelay(60);
    //Calling is allowed between 1am to 6am.
    pAC->GetCallingWindow().push_back(std::make_pair(CGXTime(1, 0, 0, -1), CGXTime(6, 0, 0, -1)));
    pAC->GetDestinations().push_back("www.gurux.org");
    items.push_back(pAC);
}

/*
* Add Activity Calendar object.
*/
void AddActivityCalendar(CGXDLMSObjectCollection& items)
{
    CGXDLMSActivityCalendar* pActivity = new CGXDLMSActivityCalendar();
    pActivity->SetCalendarNameActive("Active");
    pActivity->GetSeasonProfileActive().push_back(CGXDLMSSeasonProfile("Summer time", CGXDate(-1, 3, 31), ""));
    pActivity->GetWeekProfileTableActive().push_back(CGXDLMSWeekProfile("Monday", 1, 1, 1, 1, 1, 1, 1));
    CGXDLMSDayProfile aDp;
    aDp.SetDayId(1);
    aDp.GetDaySchedules().push_back(CGXDLMSDayProfileAction(CGXDateTime::Now(), "test", 1));
    pActivity->GetDayProfileTableActive().push_back(aDp);
    pActivity->SetCalendarNamePassive("Passive");
    pActivity->GetSeasonProfilePassive().push_back(CGXDLMSSeasonProfile("Winter time", CGXDate(-1, 10, 30), ""));
    pActivity->GetWeekProfileTablePassive().push_back(CGXDLMSWeekProfile("Tuesday", 1, 1, 1, 1, 1, 1, 1));

    CGXDLMSDayProfile passive;
    passive.SetDayId(1);
    passive.GetDaySchedules().push_back(CGXDLMSDayProfileAction(CGXDateTime::Now(), "0.0.1.0.0.255", 1));
    pActivity->GetDayProfileTablePassive().push_back(passive);
    CGXDateTime dt(CGXDateTime::Now());
    pActivity->SetTime(dt);
    items.push_back(pActivity);
}

/*
* Add Optical Port Setup object.
*/
void AddOpticalPortSetup(CGXDLMSObjectCollection& items)
{
    CGXDLMSIECOpticalPortSetup* pOptical = new CGXDLMSIECOpticalPortSetup();
    pOptical->SetDefaultMode(DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT);
    pOptical->SetProposedBaudrate(DLMS_BAUD_RATE_9600);
    pOptical->SetDefaultBaudrate(DLMS_BAUD_RATE_300);
    pOptical->SetResponseTime(DLMS_LOCAL_PORT_RESPONSE_TIME_200_MS);
    pOptical->SetDeviceAddress("Gurux");
    pOptical->SetPassword1("Gurux1");
    pOptical->SetPassword2("Gurux2");
    pOptical->SetPassword5("Gurux5");
    items.push_back(pOptical);
}

/*
* Add Demand Register object.
*/
void AddDemandRegister(CGXDLMSObjectCollection& items)
{
    CGXDLMSDemandRegister* pDr = new CGXDLMSDemandRegister("0.0.1.0.0.255");
    pDr->SetCurrentAvarageValue(10);
    pDr->SetLastAvarageValue(20);
    pDr->SetStatus(1);
    pDr->SetStartTimeCurrent(CGXDateTime::Now());
    pDr->SetCaptureTime(CGXDateTime::Now());
    pDr->SetPeriod(10);
    pDr->SetNumberOfPeriods(1);
    items.push_back(pDr);
}

/*
* Add Register Monitor object.
*/
void AddRegisterMonitor(CGXDLMSObjectCollection& items, CGXDLMSRegister* pRegister)
{
    CGXDLMSRegisterMonitor* pRm = new CGXDLMSRegisterMonitor("0.0.1.0.0.255");
    CGXDLMSVariant threshold;
    vector<CGXDLMSVariant> thresholds;
    threshold.Add("Gurux1", 6);
    thresholds.push_back(threshold);
    threshold.Clear();
    threshold.Add("Gurux2", 6);
    thresholds.push_back(threshold);
    pRm->SetThresholds(thresholds);
    CGXDLMSMonitoredValue mv;
    mv.Update(pRegister, 2);
    pRm->SetMonitoredValue(mv);
    CGXDLMSActionSet action;
    string ln;
    pRm->GetLogicalName(ln);
    action.GetActionDown().SetLogicalName(ln);
    action.GetActionDown().SetScriptSelector(1);
    pRm->GetLogicalName(ln);
    action.GetActionUp().SetLogicalName(ln);
    action.GetActionUp().SetScriptSelector(1);
    vector<CGXDLMSActionSet> actions;
    actions.push_back(action);
    pRm->SetActions(actions);
    items.push_back(pRm);
}

/*
* Add action schedule object.
*/
void AddActionSchedule(CGXDLMSObjectCollection& items)
{
    CGXDLMSActionSchedule* pActionS = new CGXDLMSActionSchedule("0.0.1.0.0.255");
    pActionS->SetExecutedScriptLogicalName("1.2.3.4.5.6");
    pActionS->SetExecutedScriptSelector(1);
    pActionS->SetType(DLMS_SINGLE_ACTION_SCHEDULE_TYPE1);
    pActionS->GetExecutionTime().push_back(CGXDateTime::Now());
    items.push_back(pActionS);
}

/*
* Add SAP Assignment object.
*/
void AddSapAssignment(CGXDLMSObjectCollection& items)
{
    CGXDLMSSapAssignment* pSap = new CGXDLMSSapAssignment();
    std::map<int, basic_string<char> > list;
    list[1] = "Gurux";
    list[16] = "Gurux-2";
    pSap->SetSapAssignmentList(list);
    items.push_back(pSap);
}

/**
* Add Auto Answer object.
*/
void AddAutoAnswer(CGXDLMSObjectCollection& items)
{
    CGXDLMSAutoAnswer* pAa = new CGXDLMSAutoAnswer();
    pAa->SetMode(AUTO_CONNECT_MODE_EMAIL_SENDING);
    pAa->GetListeningWindow().push_back(std::pair<CGXDateTime, CGXDateTime>(CGXDateTime(-1, -1, -1, 6, -1, -1, -1), CGXDateTime(-1, -1, -1, 8, -1, -1, -1)));
    pAa->SetStatus(AUTO_ANSWER_STATUS_INACTIVE);
    pAa->SetNumberOfCalls(0);
    pAa->SetNumberOfRingsInListeningWindow(1);
    pAa->SetNumberOfRingsOutListeningWindow(2);
    items.push_back(pAa);
}

/*
* Add Modem Configuration object.
*/
void AddModemConfiguration(CGXDLMSObjectCollection& items)
{
    CGXDLMSModemConfiguration* pMc = new CGXDLMSModemConfiguration();
    pMc->SetCommunicationSpeed(DLMS_BAUD_RATE_38400);
    CGXDLMSModemInitialisation init;
    vector<CGXDLMSModemInitialisation> initialisationStrings;
    init.SetRequest("AT");
    init.SetResponse("OK");
    init.SetDelay(0);
    initialisationStrings.push_back(init);
    pMc->SetInitialisationStrings(initialisationStrings);
    items.push_back(pMc);
}

/**
* Add MAC Address Setup object.
*/
void AddMacAddressSetup(CGXDLMSObjectCollection& items)
{
    CGXDLMSMacAddressSetup* pMac = new CGXDLMSMacAddressSetup();
    pMac->SetMacAddress("00:11:22:33:44:55:66");
    items.push_back(pMac);
}

/**
* Add IP4 setup object.
*/
CGXDLMSIp4Setup* AddIp4Setup(CGXDLMSObjectCollection& items, std::string& address)
{
    CGXDLMSIp4Setup* pIp4 = new CGXDLMSIp4Setup();
    pIp4->SetIPAddress(address);
    items.push_back(pIp4);
    return pIp4;
}

/*
* Generic initialize for all servers.
*/
int CGXDLMSBase::Init(int port)
{
    int ret;
    if ((ret = StartServer(port)) != 0)
    {
        return ret;
    }
    //Get local IP address.
    std::string address;
    GetIpAddress(address);

    unsigned long sn = 123456;
    CGXDLMSData* ldn = AddLogicalDeviceName(GetItems(), sn);
    //Add firmaware.
    AddFirmwareVersion(GetItems());
    AddElectricityID1(GetItems(), sn);
    AddElectricityID2(GetItems(), sn);

    //Add Last avarage.
    CGXDLMSRegister* pRegister = new CGXDLMSRegister("1.1.21.25.0.255");
    //Set access right. Client can't change Device name.
    pRegister->SetAccess(2, DLMS_ACCESS_MODE_READ);
    GetItems().push_back(pRegister);
    //Add default clock. Clock's Logical Name is 0.0.1.0.0.255.
    CGXDLMSClock* pClock = new CGXDLMSClock();
    CGXDateTime begin(-1, 9, 1, -1, -1, -1, -1);
    pClock->SetBegin(begin);
    CGXDateTime end(-1, 3, 1, -1, -1, -1, -1);
    pClock->SetEnd(end);
    GetItems().push_back(pClock);
    //Add Tcp/Udp setup. Default Logical Name is 0.0.25.0.0.255.
    GetItems().push_back(new CGXDLMSTcpUdpSetup());

    ///////////////////////////////////////////////////////////////////////
    //Add profile generic (historical data) object.
    CGXDLMSProfileGeneric* profileGeneric = new CGXDLMSProfileGeneric("1.0.99.1.0.255");
    //Set capture period to 60 second.
    profileGeneric->SetCapturePeriod(60);
    //Maximum row count.
    profileGeneric->SetProfileEntries(100);
    profileGeneric->SetSortMethod(DLMS_SORT_METHOD_FIFO);
    profileGeneric->SetSortObject(pClock);
    //Add colums.
    //Set saved attribute index.
    CGXDLMSCaptureObject * capture = new CGXDLMSCaptureObject(2, 0);
    profileGeneric->GetCaptureObjects().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*>(pClock, capture));
    //Set saved attribute index.
    capture = new CGXDLMSCaptureObject(2, 0);
    profileGeneric->GetCaptureObjects().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*>(pRegister, capture));
    GetItems().push_back(profileGeneric);

    ///////////////////////////////////////////////////////////////////////
    //Add Auto connect object.
    AddAutoConnect(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Activity Calendar object.
    AddActivityCalendar(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Optical Port Setup object.
    AddOpticalPortSetup(GetItems());
    ///////////////////////////////////////////////////////////////////////
    //Add Demand Register object.
    AddDemandRegister(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Register Monitor object.
    AddRegisterMonitor(GetItems(), pRegister);

    ///////////////////////////////////////////////////////////////////////
    //Add action schedule object.
    AddActionSchedule(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add SAP Assignment object.
    AddSapAssignment(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Auto Answer object.
    AddAutoAnswer(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Modem Configuration object.
    AddModemConfiguration(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Mac Address Setup object.
    AddMacAddressSetup(GetItems());
    ///////////////////////////////////////////////////////////////////////
    //Add IP4 Setup object.
    CGXDLMSIp4Setup* pIp4 = AddIp4Setup(GetItems(), address);

    ///////////////////////////////////////////////////////////////////////
    //Add Push Setup object.
    CGXDLMSPushSetup* pPush = new CGXDLMSPushSetup();
    pPush->SetDestination(address);
    GetItems().push_back(pPush);

    // Add push object itself. This is needed to tell structure of data to
    // the Push listener.
    pPush->GetPushObjectList().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(pPush, CGXDLMSCaptureObject(2, 0)));
    //Add logical device name.
    pPush->GetPushObjectList().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(ldn, CGXDLMSCaptureObject(2, 0)));
    // Add 0.0.25.1.0.255 Ch. 0 IPv4 setup IP address.
    pPush->GetPushObjectList().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(pIp4, CGXDLMSCaptureObject(3, 0)));
    ///////////////////////////////////////////////////////////////////////
    //Server must initialize after all objects are added.
    ret = Initialize();
    if (ret != DLMS_ERROR_CODE_OK)
    {
        return ret;
    }

    //Add rows after Initialize.
    std::vector<CGXDLMSVariant> row;
    CGXDateTime tmp = CGXDateTime::Now();
    row.push_back(tmp);
    row.push_back(10);
    profileGeneric->GetBuffer().push_back(row);
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
    char EXPECTED_PASSWORD[] = "Gurux";
    if (authentication == DLMS_AUTHENTICATION_NONE)
    {
        //Uncomment this if authentication is always required.
        //return DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED;
    }
    //Check Low Level security..
    if (authentication == DLMS_AUTHENTICATION_LOW)
    {
        if (password.GetSize() != strlen(EXPECTED_PASSWORD) &&
                memcmp(EXPECTED_PASSWORD, password.GetData(), strlen(EXPECTED_PASSWORD)) != 0)
        {
            return DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE;
        }
    }
    //High Level security is checked later.
    return DLMS_SOURCE_DIAGNOSTIC_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Connected(
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    printf("Connected.\r\n");
}

void CGXDLMSBase::InvalidConnection(
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    printf("InvalidConnection.\r\n");

}
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CGXDLMSBase::Disconnected(
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    printf("Disconnected.\r\n");
}
