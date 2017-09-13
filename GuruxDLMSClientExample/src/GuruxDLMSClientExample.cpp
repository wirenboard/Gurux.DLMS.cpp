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
#if defined(_WIN32) || defined(_WIN64)//Windows
#include "../include/getopt.h"
#endif
#include "../include/communication.h"

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
        GX_TRACE_LEVEL trace = GX_TRACE_LEVEL_INFO;
        bool useLogicalNameReferencing = true;
        int clientAddress = 16, serverAddress = 1;
        DLMS_AUTHENTICATION authentication = DLMS_AUTHENTICATION_NONE;
        DLMS_INTERFACE_TYPE interfaceType = DLMS_INTERFACE_TYPE_HDLC;
        char *password = NULL;
        char *p, *p2, *readObjects = NULL;
        int index, a, b, c, d, e, f;
        int opt = 0;
        int port = 0;
        char* address = NULL;
        char* serialPort = NULL;
        bool iec = false;
        while ((opt = getopt(argc, argv, "h:p:c:s:r:it:a:wP:g:")) != -1)
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
                if (strcmp("Error", optarg) == 0)
                    trace = GX_TRACE_LEVEL_ERROR;
                else  if (strcmp("Warning", optarg) == 0)
                    trace = GX_TRACE_LEVEL_WARNING;
                else  if (strcmp("Info", optarg) == 0)
                    trace = GX_TRACE_LEVEL_INFO;
                else  if (strcmp("Verbose", optarg) == 0)
                    trace = GX_TRACE_LEVEL_VERBOSE;
                else  if (strcmp("Off", optarg) == 0)
                    trace = GX_TRACE_LEVEL_OFF;
                else
                {
                    printf("Invalid trace option '%s'. (Error, Warning, Info, Verbose, Off)", optarg);
                    return 1;
                }
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
            case 'g':
                //Get (read) selected objects.
                p = optarg;
                do
                {
                    if (p != optarg)
                    {
                        ++p;
                    }
#if defined(_WIN32) || defined(_WIN64)//Windows
                    if ((ret = sscanf_s(p, "%d.%d.%d.%d.%d.%d:%d", &a, &b, &c, &d, &e, &f, &index)) != 7)
#else
                    if ((ret = sscanf(p, "%d.%d.%d.%d.%d.%d:%d", &a, &b, &c, &d, &e, &f, &index)) != 7)
#endif
                    {
                        ShowHelp();
                        return 1;
                    }
                } while ((p = strchr(p, ',')) != NULL);
                readObjects = optarg;
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
                else if (optarg[0] == 'g') {
                    printf("Missing mandatory OBIS code option.\n");
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
                printf("Connect failed %s.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
                return 1;
            }
        }
        else if (serialPort != NULL)
        {
            if ((ret = comm.Open(serialPort, iec)) != 0)
            {
                printf("Connect failed %s.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
                return 1;
            }
        }
        else
        {
            printf("Missing mandatory connection information for TCP/IP or serial port connection.\n");
            return 1;
        }

        if (readObjects != NULL)
        {
            if ((ret = comm.InitializeConnection()) == 0 &&
                (ret = comm.GetAssociationView()) == 0)
            {
                std::string str;
                std::string value;
                char buff[200];
                p = readObjects;
                do
                {
                    if (p != readObjects)
                    {
                        ++p;
                    }
                    str.clear();
                    p2 = strchr(p, ':');
                    ++p2;
#if defined(_WIN32) || defined(_WIN64)//Windows
                    sscanf_s(p2, "%d", &index);
#else
                    sscanf(p2, "%d", &index);
#endif
                    str.append(p, p2 - p);
                    CGXDLMSObject* obj = cl.GetObjects().FindByLN(DLMS_OBJECT_TYPE_ALL, str);
                    value.clear();
                    if ((ret = comm.Read(obj, index, value)) != DLMS_ERROR_CODE_OK)
                    {
#if _MSC_VER > 1000
                        sprintf_s(buff, 100, "Error! Index: %d %s\r\n", index, CGXDLMSConverter::GetErrorMessage(ret));
#else
                        sprintf(buff, "Error! Index: %d read failed: %s\r\n", index, CGXDLMSConverter::GetErrorMessage(ret));
#endif
                        comm.WriteValue(GX_TRACE_LEVEL_ERROR, buff);
                        //Continue reading.
                    }
                    else
                    {
#if _MSC_VER > 1000
                        sprintf_s(buff, 100, "Index: %d Value: ", index);
#else
                        sprintf(buff, "Index: %d Value: ", index);
#endif
                        comm.WriteValue(trace, buff);
                        comm.WriteValue(trace, value.c_str());
                        comm.WriteValue(trace, "\r\n");
                    }

                } while ((p = strchr(p, ',')) != NULL);
            }
        }
        else {
            ret = comm.ReadAll();
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

