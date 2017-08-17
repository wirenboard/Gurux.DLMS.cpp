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

// GuruxDLMSClientExample.cpp : Defines the entry point for the Gurux DLMS Client example.
//
#include "../include/getopt.h"
#include "../include/communication.h"
#include "../../development/include/GXDLMSConverter.h"
#include "../../development/include/GXDLMSProfileGeneric.h"
#include "../../development/include/GXDLMSDemandRegister.h"

static void WriteValue(std::string line)
{
    printf(line.c_str());
    GXHelpers::Write("LogFile.txt", line);
}

static void ShowHelp()
{
    printf("GuruxDlmsSample reads data from the DLMS/COSEM device.\r\n");
    printf("GuruxDlmsSample -h [Meter IP Address] -p [Meter Port No] -c 16 -s 1 -r SN\r\n");
    printf(" -h \t host name or IP address.\r\n");
    printf(" -p \t port number or name (Example: 1000).\r\n");
    printf(" -S \t serial port.\r\n");
    printf(" -i IEC is a start protocol.\r\n");
    printf(" -a \t Authentication (None, Low, High).\r\n");
    printf(" -P \t Password for authentication.\r\n");
    printf(" -c \t Client address. (Default: 16)\r\n");
    printf(" -s \t Server address. (Default: 1)\r\n");
    printf(" -n \t Server address as serial number.\r\n");
    printf(" -r [sn, sn]\t Short name or Logican Name (default) referencing is used.\r\n");
    printf(" -w WRAPPER profile is used. HDLC is default.\r\n");
    printf(" -t Trace messages.\r\n");
    printf(" -g \"0.0.1.0.0.255:1; 0.0.1.0.0.255:2\" Get selected object(s) with given attribute index.\r\n");
    printf("Example:\r\n");
    printf("Read LG device using TCP/IP connection.\r\n");
    printf("GuruxDlmsSample -r SN -c 16 -s 1 -h [Meter IP Address] -p [Meter Port No]\r\n");
    printf("Read LG device using serial port connection.\r\n");
    printf("GuruxDlmsSample -r SN -c 16 -s 1 -sp COM1 -i\r\n");
    printf("Read Indian device using serial port connection.\r\n");
    printf("GuruxDlmsSample -S COM1 -c 16 -s 1 -a Low -P [password]\r\n");
}

#if defined(_WIN32) || defined(_WIN64)//Windows
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    {
#if defined(_WIN32) || defined(_WIN64)//Windows
        //Use user locale settings. This is causing errors in some Linux distros. Example Fedora
        std::locale::global(std::locale(""));
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            // Tell the user that we could not find a usable WinSock DLL.
            return 1;
        }
#endif
        int ret;

        //Remove trace file if exists.
        remove("trace.txt");
        remove("LogFile.txt");
        bool trace = false, useLogicalNameReferencing = true;
        int clientAddress = 16, serverAddress = 1;
        DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE;
        DLMS_INTERFACE_TYPE interfaceType = DLMS_INTERFACE_TYPE_HDLC;
        char *password = NULL;

        int opt = 0;
        int port = 0;
        char* address = NULL;
        char* serialPort = NULL;
        bool iec = false;
        while ((opt = getopt(argc, argv, "h:p:c:s:r:ita:wP:")) != -1)
        {
            switch (opt)
            {
            case 'w':
                interfaceType = DLMS_INTERFACE_TYPE_WRAPPER;
                break;
            case 'r':
                if (strcmp("sn", optarg) == 0)
                {
                    useLogicalNameReferencing = false;
                }
                else if (strcmp("ln", optarg) == 0)
                {
                    useLogicalNameReferencing = 1;
                }
                else
                {
                    printf("Invalid reference option.\n");
                    return 1;
                }
                break;
            case 'h':
                //Host address.
                address = optarg;
                break;
            case 't':
                //Trace.
                trace = 1;
                break;
            case 'p':
                //Port.
                port = atoi(optarg);
                break;
            case 'P':
                password = optarg;
                break;
            case 'i':
                //IEC.
                iec = 1;
                break;
            case 'S':
                serialPort = optarg;
                break;
            case 'a':
                if (strcmp("None", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_NONE;
                }
                else if (strcmp("Low", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_LOW;
                }
                else if (strcmp("High", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_HIGH;
                }
                else if (strcmp("HighMd5", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_HIGH_MD5;
                }
                else if (strcmp("HighSha1", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_HIGH_SHA1;
                }
                else if (strcmp("HighGmac", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_HIGH_GMAC;
                }
                else if (strcmp("HighSha256", optarg) == 0)
                {
                    authentication = DLMS_AUTHENTICATION_HIGH_SHA256;
                }
                else
                {
                    printf("Invalid Authentication option. (None, Low, High, HighMd5, HighSha1, HighGmac, HighSha256)\n");
                    return 1;
                }
                break;
            case 'o':
                break;
            case 'c':
                clientAddress = atoi(optarg);
                break;
            case 's':
                serverAddress = atoi(optarg);
                break;
            case '?':
            {
                if (optarg[0] == 'c') {
                    printf("Missing mandatory client option.\n");
                }
                else if (optarg[0] == 's') {
                    printf("Missing mandatory server option.\n");
                }
                else if (optarg[0] == 'h') {
                    printf("Missing mandatory host name option.\n");
                }
                else if (optarg[0] == 'p') {
                    printf("Missing mandatory port option.\n");
                }
                else if (optarg[0] == 'r') {
                    printf("Missing mandatory reference option.\n");
                }
                else if (optarg[0] == 'a') {
                    printf("Missing mandatory authentication option.\n");
                }
                else if (optarg[0] == 'S') {
                    printf("Missing mandatory Serial port option.\n");
                }
                else
                {
                    ShowHelp();
                    return 1;
                }
            }
            break;
            default:
                ShowHelp();
                return 1;
            }
        }
        CGXDLMSSecureClient cl(useLogicalNameReferencing, clientAddress, serverAddress, authentication, password, interfaceType);
        CGXCommunication comm(&cl, 5000, trace);

        if (port != 0 || address != NULL)
        {
            if (port == 0)
            {
                printf("Missing mandatory port option.\n");
                return 1;
            }
            if (address == NULL)
            {
                printf("Missing mandatory host option.\n");
                return 1;
            }
            if ((ret = comm.Connect(address, port)) != 0)
            {
                TRACE("Connect failed %s.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
                return 1;
            }
        }
        else if (serialPort != NULL)
        {
            if ((ret = comm.Open(serialPort, iec)) != 0)
            {
                TRACE("Connect failed %S.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
                return 1;
            }
        }
        else
        {
            printf("Missing mandatory connection information for TCP/IP or serial port connection.\n");
            return 1;
        }
        if ((ret = comm.InitializeConnection()) != 0)
        {
            TRACE("InitializeConnection failed %s.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
            return 1;
        }
        CGXDLMSObjectCollection Objects;
        if ((ret = comm.GetObjects(Objects)) != 0)
        {
            TRACE("InitializeConnection failed %s.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
            return 1;
        }

        CGXDLMSRegister* pReg = new CGXDLMSRegister("1.0.32.7.0.255");
        Objects.push_back(pReg);

        std::string value;
        std::string str;
        std::string ln;
        std::vector<std::pair<CGXDLMSObject*, unsigned char> > list;
        if ((cl.GetNegotiatedConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) != 0)
        {
            // Read scalers and units from the device.
            for (std::vector<CGXDLMSObject*>::iterator it = Objects.begin(); it != Objects.end(); ++it)
            {
                if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER ||
                    (*it)->GetObjectType() == DLMS_OBJECT_TYPE_EXTENDED_REGISTER)
                {
                    list.push_back(std::make_pair(*it, 3));
                }
                else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_DEMAND_REGISTER)
                {
                    list.push_back(std::make_pair(*it, 4));
                }
            }
            if ((ret = comm.ReadList(list)) != 0)
            {
                TRACE("Err! Failed to read register: %s", CGXDLMSConverter::GetErrorMessage(ret));
                cl.SetNegotiatedConformance((DLMS_CONFORMANCE)(cl.GetNegotiatedConformance() & ~DLMS_CONFORMANCE_MULTIPLE_REFERENCES));
            }
        }
        if ((cl.GetNegotiatedConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) == 0)
        {
            //If readlist is not supported read one value at the time.
            for (std::vector<CGXDLMSObject*>::iterator it = Objects.begin(); it != Objects.end(); ++it)
            {
                if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER ||
                    (*it)->GetObjectType() == DLMS_OBJECT_TYPE_EXTENDED_REGISTER)
                {
                    (*it)->GetLogicalName(ln);
                    TRACE("%s\r\n", ln.c_str());
                    if ((ret = comm.Read(*it, 3, value)) != 0)
                    {
                        TRACE("Err! Failed to read register: %s", CGXDLMSConverter::GetErrorMessage(ret));
                        //Continue reading.
                        continue;
                    }
                }
                else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_DEMAND_REGISTER)
                {
                    (*it)->GetLogicalName(ln);
                    TRACE("%s\r\n", ln.c_str());
                    if ((ret = comm.Read(*it, 4, value)) != 0)
                    {
                        TRACE("Err! Failed to read demand register: %s", CGXDLMSConverter::GetErrorMessage(ret));
                        //Continue reading.
                        continue;
                    }
                }
            }
        }
        //Read columns.
        CGXDLMSObjectCollection profileGenerics;
        Objects.GetObjects(DLMS_OBJECT_TYPE_PROFILE_GENERIC, profileGenerics);
        for (std::vector<CGXDLMSObject*>::iterator it = profileGenerics.begin(); it != profileGenerics.end(); ++it)
        {
            //Read Profile Generic columns first.
            CGXDLMSProfileGeneric* pg = (CGXDLMSProfileGeneric*)*it;
            if ((ret = comm.Read(pg, 3, value)) != 0)
            {
                TRACE("Err! Failed to read columns: %s", CGXDLMSConverter::GetErrorMessage(ret));
                //Continue reading.
                continue;
            }

            //Update columns scalers.
            DLMS_OBJECT_TYPE ot;
            CGXDLMSObject* obj;
            for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it2 = pg->GetCaptureObjects().begin(); it2 != pg->GetCaptureObjects().end(); ++it2)
            {
                ot = it2->first->GetObjectType();
                if (ot == DLMS_OBJECT_TYPE_REGISTER ||
                    ot == DLMS_OBJECT_TYPE_EXTENDED_REGISTER ||
                    ot == DLMS_OBJECT_TYPE_DEMAND_REGISTER)
                {
                    it2->first->GetLogicalName(ln);
                    obj = Objects.FindByLN(ot, ln);
                    if (obj != NULL)
                    {
                        if (ot == DLMS_OBJECT_TYPE_REGISTER || ot == DLMS_OBJECT_TYPE_EXTENDED_REGISTER)
                        {
                            ((CGXDLMSRegister*)it2->first)->SetScaler(((CGXDLMSRegister*)obj)->GetScaler());
                            ((CGXDLMSRegister*)it2->first)->SetUnit(((CGXDLMSRegister*)obj)->GetUnit());
                        }
                        else if (ot == DLMS_OBJECT_TYPE_DEMAND_REGISTER)
                        {
                            ((CGXDLMSDemandRegister*)it2->first)->SetScaler(((CGXDLMSDemandRegister*)obj)->GetScaler());
                            ((CGXDLMSDemandRegister*)it2->first)->SetUnit(((CGXDLMSDemandRegister*)obj)->GetUnit());
                        }
                    }
                }
            }
            WriteValue("Profile Generic " + (*it)->GetName().ToString() + " Columns:\r\n");
            std::string str;
            for (std::vector<std::pair<CGXDLMSObject*, CGXDLMSCaptureObject*> >::iterator it2 = pg->GetCaptureObjects().begin(); it2 != pg->GetCaptureObjects().end(); ++it2)
            {
                if (str.size() != 0)
                {
                    str.append(" | ");
                }
                str.append((*it2).first->GetName().ToString());
                str.append(" ");
                str.append((*it2).first->GetDescription());
            }
            str.append("\r\n");
            WriteValue(str);
        }

        for (std::vector<CGXDLMSObject*>::iterator it = Objects.begin(); it != Objects.end(); ++it)
        {
            // Profile generics are read later because they are special cases.
            // (There might be so lots of data and we so not want waste time to read all the data.)
            if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
            {
                continue;
            }

            if (dynamic_cast<CGXDLMSCustomObject*>((*it)) != NULL)
            {
                //If interface is not implemented.
                //Example manufacturer specific interface.
                printf("Unknown Interface: %d\r\n", (*it)->GetObjectType());
                continue;
            }
            char buff[200];
#if _MSC_VER > 1000
            sprintf_s(buff, 200, "-------- Reading %s %s %s\r\n", CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str(), (*it)->GetName().ToString().c_str(), (*it)->GetDescription().c_str());
#else
            sprintf(buff, "-------- Reading %s %s %s\r\n", CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str(), (*it)->GetName().ToString().c_str(), (*it)->GetDescription().c_str());
#endif
            WriteValue(buff);
            std::vector<int> attributes;
            (*it)->GetAttributeIndexToRead(attributes);
            for (std::vector<int>::iterator pos = attributes.begin(); pos != attributes.end(); ++pos)
            {
                value.clear();
                if ((ret = comm.Read(*it, *pos, value)) != DLMS_ERROR_CODE_OK)
                {
#if _MSC_VER > 1000
                    sprintf_s(buff, 100, "Error! Index: %d %s\r\n", *pos, CGXDLMSConverter::GetErrorMessage(ret));
#else
                    sprintf(buff, "Error! Index: %d read failed: %s\r\n", *pos, CGXDLMSConverter::GetErrorMessage(ret));
#endif
                    WriteValue(buff);
                    //Continue reading.
                }
                else
                {
#if _MSC_VER > 1000
                    sprintf_s(buff, 100, "Index: %d Value: ", *pos);
#else
                    sprintf(buff, "Index: %d Value: ", *pos);
#endif
                    WriteValue(buff);
                    WriteValue(value.c_str());
                    WriteValue("\r\n");
                }
            }
        }

        //Find profile generics and read them.
        CGXDLMSObjectCollection pgs;
        Objects.GetObjects(DLMS_OBJECT_TYPE_PROFILE_GENERIC, pgs);
        for (std::vector<CGXDLMSObject*>::iterator it = pgs.begin(); it != pgs.end(); ++it)
        {
            char buff[200];
#if _MSC_VER > 1000
            sprintf_s(buff, 200, "-------- Reading %s %s %s\r\n", CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str(), (*it)->GetName().ToString().c_str(), (*it)->GetDescription().c_str());
#else
            sprintf(buff, "-------- Reading %s %s %s\r\n", CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str(), (*it)->GetName().ToString().c_str(), (*it)->GetDescription().c_str());
#endif
            WriteValue(buff);

            if ((ret = comm.Read(*it, 7, value)) != DLMS_ERROR_CODE_OK)
            {
#if _MSC_VER > 1000
                sprintf_s(buff, 100, "Error! Index: %d: %s\r\n", 7, CGXDLMSConverter::GetErrorMessage(ret));
#else
                sprintf(buff, "Error! Index: %d: %s\r\n", 7, CGXDLMSConverter::GetErrorMessage(ret));
#endif
                WriteValue(buff);
                //Continue reading.
            }

            std::string entriesInUse = value;
            if ((ret = comm.Read(*it, 8, value)) != DLMS_ERROR_CODE_OK)
            {
#if _MSC_VER > 1000
                sprintf_s(buff, 100, "Error! Index: %d: %s\r\n", 8, CGXDLMSConverter::GetErrorMessage(ret));
#else
                sprintf(buff, "Error! Index: %d: %s\r\n", 8, CGXDLMSConverter::GetErrorMessage(ret));
#endif
                WriteValue(buff);
                //Continue reading.
            }
            std::string entries = value;
            str = "Entries: ";
            str += entriesInUse;
            str += "/";
            str += entries;
            str += "\r\n";
            WriteValue(str);
            //If there are no columns or rows.
            if (((CGXDLMSProfileGeneric*)*it)->GetEntriesInUse() == 0 || ((CGXDLMSProfileGeneric*)*it)->GetCaptureObjects().size() == 0)
            {
                continue;
            }
            //All meters are not supporting parameterized read.
            CGXDLMSVariant rows;
            if ((cl.GetNegotiatedConformance() & (DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | DLMS_CONFORMANCE_SELECTIVE_ACCESS)) != 0)
            {
                //Read first row from Profile Generic.
                if ((ret = comm.ReadRowsByEntry((CGXDLMSProfileGeneric*)*it, 1, 1, rows)) != 0)
                {
                    str = "Error! Failed to read first row:";
                    str += CGXDLMSConverter::GetErrorMessage(ret);
                    str += "\r\n";
                    WriteValue(str);
                    //Continue reading.
                }
                else
                {
                    //////////////////////////////////////////////////////////////////////////////
                    //Show rows.
                    WriteValue(rows.ToString());
                }
            }

            //All meters are not supporting parameterized read.
            if ((cl.GetNegotiatedConformance() & (DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | DLMS_CONFORMANCE_SELECTIVE_ACCESS)) != 0)
            {
                CGXDateTime start = CGXDateTime::Now();
                start.ResetTime();
                CGXDateTime end = CGXDateTime::Now();
                if ((ret = comm.ReadRowsByRange((CGXDLMSProfileGeneric*)(*it), start, end, rows)) != 0)
                {
                    str = "Error! Failed to read last day:";
                    str += CGXDLMSConverter::GetErrorMessage(ret);
                    str += "\r\n";
                    WriteValue(str);
                    //Continue reading.
                }
                else
                {
                    //////////////////////////////////////////////////////////////////////////////
                    //Show rows.
                    WriteValue(rows.ToString());
                }
            }
        }
        //Close connection.
        comm.Close();
    }
#if defined(_WIN32) || defined(_WIN64)//Windows
    WSACleanup();
#if _MSC_VER > 1400
    _CrtDumpMemoryLeaks();
#endif
#endif
    return 0;
}

