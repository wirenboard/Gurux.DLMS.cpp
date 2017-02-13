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

#include "../include/communication.h"
#include "../../development/include/GXDLMSConverter.h"
#include "../../development/include/GXDLMSProfileGeneric.h"
#include "../../development/include/GXDLMSDemandRegister.h"

static void WriteValue(std::string line)
{
    printf(line.c_str());
    GXHelpers::Write("LogFile.txt", line);
}

#if defined(_WIN32) || defined(_WIN64)//Windows
int _tmain(int argc, _TCHAR* argv[])
#else
int main(int argc, char* argv[])
#endif
{
    {
        //Use user locale settings.
        std::locale::global(std::locale(""));
#if defined(_WIN32) || defined(_WIN64)//Windows
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            // Tell the user that we could not find a usable WinSock DLL.
            return 1;
        }
#endif
        int ret;
        ////////////////////////////////////////////////////////////////////////////////////////////////////////
        //TODO: Client and Server addresses are manufacturer dependence. They should be standard values but they are not.
        //Below are some example values. Ask correct values from your meter manufacturer or http://www.gurux.org.
        /*
            //Iskra Serial port settings.
            CGXDLMSClient cl(true, 100, CGXDLMSClient::GetServerAddress(1, 17), DLMS_AUTHENTICATION_LOW, "12345678");
            //Landis+Gyr settings.
            CGXDLMSClient cl(false);
            //Kamstrup settings.
            CGXDLMSClient cl(true);
            //Actaris settings.
            CGXDLMSClient cl(true, 1, CGXDLMSClient::GetServerAddress(1, 17), DLMS_AUTHENTICATION_LOW, "ABCDEFGH");
            //Iskra TCP/IP settings.
            CGXDLMSClient cl(true, 100, 1, DLMS_AUTHENTICATION_LOW, "12345678", DLMS_INTERFACE_TYPE_WRAPPER);
            CGXDLMSClient cl(true, 1, 1, DLMS_AUTHENTICATION_LOW, "12345678", DLMS_INTERFACE_TYPE_WRAPPER);
            //Note! New Iskra meters need also this.
            cl.SetServiceClass(DLMS_SERVICE_CLASS_CONFIRMED);
            //ZIV settings.
            CGXDLMSClient cl(true, 1, 1, DLMS_AUTHENTICATION_NONE, NULL, DLMS_INTERFACE_TYPE_WRAPPER);
        */
        //Remove trace file if exists.
        remove("trace.txt");
        remove("LogFile.txt");
        bool trace = true;
        //Use Logical name.
        CGXDLMSClient cl(true);
        CGXCommunication comm(&cl, 5000, trace);
        //Serial port settings.
        /*
        if ((ret = comm.Open("COM3", false)) != 0)
        {
            TRACE("Connect failed %S.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
            return 1;
        }

        */
        //TCP/IP settings. Default is Gurux example meter.
        if ((ret = comm.Connect("localhost", 4061)) != 0)
        {
            TRACE("Connect failed %s.\r\n", CGXDLMSConverter::GetErrorMessage(ret));
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
        std::string value;
        std::string str;
        std::string ln;
        std::vector<std::pair<CGXDLMSObject*, unsigned char> > list;
        if ((cl.GetConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) != 0)
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
                cl.SetConformance((DLMS_CONFORMANCE)(cl.GetConformance() & ~DLMS_CONFORMANCE_MULTIPLE_REFERENCES));
            }
        }
        if ((cl.GetConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) == 0)
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
            if ((cl.GetConformance() & (DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | DLMS_CONFORMANCE_SELECTIVE_ACCESS)) != 0)
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
            if ((cl.GetConformance() & (DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | DLMS_CONFORMANCE_SELECTIVE_ACCESS)) != 0)
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

