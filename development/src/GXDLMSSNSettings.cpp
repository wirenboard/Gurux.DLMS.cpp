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

#include "../include/GXDLMSSNSettings.h"
#include "../include/GXHelpers.h"

CGXDLMSSNSettings::CGXDLMSSNSettings(void)
{
    //Set default values.
    m_ConformanceBlock[0] = 0x1C;
    m_ConformanceBlock[1] = 0x03;
    m_ConformanceBlock[2] = 0x20;
}

CGXDLMSSNSettings::~CGXDLMSSNSettings(void)
{
}

bool CGXDLMSSNSettings::GetGeneralProtection()
{
    return GXHelpers::GetBits(m_ConformanceBlock[0], 0x40);
}

void CGXDLMSSNSettings::SetGeneralProtection(bool value)
{
    GXHelpers::SetBits(m_ConformanceBlock[0], 0x40, value);
}

bool CGXDLMSSNSettings::GetGeneralBlockTransfer()
{
    return GXHelpers::GetBits(m_ConformanceBlock[0], 0x20);
}

void CGXDLMSSNSettings::SetGeneralBlockTransfer(bool value)
{
    GXHelpers::SetBits(m_ConformanceBlock[1], 0x20, value);
}

bool CGXDLMSSNSettings::GetRead()
{
    return GXHelpers::GetBits(m_ConformanceBlock[0], 0x10);
}

void CGXDLMSSNSettings::SetRead(bool newVal)
{
    GXHelpers::SetBits(m_ConformanceBlock[0], 0x10, newVal);
}

bool CGXDLMSSNSettings::GetWrite()
{
    return GXHelpers::GetBits(m_ConformanceBlock[0], 0x8);
}

void CGXDLMSSNSettings::SetWrite(bool newVal)
{
    GXHelpers::SetBits(m_ConformanceBlock[0], 0x8, newVal);
}

//Bit 5
bool CGXDLMSSNSettings::GetUnconfirmedWrite()
{
    return GXHelpers::GetBits(m_ConformanceBlock[0], 0x4);
}

void CGXDLMSSNSettings::SetUnconfirmedWrite(bool newVal)
{
    GXHelpers::SetBits(m_ConformanceBlock[0], 0x4, newVal);
}

bool CGXDLMSSNSettings::GetReadBlockTransfer()
{
    return GXHelpers::GetBits(m_ConformanceBlock[1], 0x10);
}
void CGXDLMSSNSettings::SetReadBlockTransfer(bool value)
{
    GXHelpers::SetBits(m_ConformanceBlock[1], 0x10, value);
}

bool CGXDLMSSNSettings::GetWriteBlockTransfer()
{
    return GXHelpers::GetBits(m_ConformanceBlock[1], 0x8);
}

void CGXDLMSSNSettings::SetWriteBlockTransfer(bool value)
{
    GXHelpers::SetBits(m_ConformanceBlock[1], 0x8, value);
}

//Bit 14
bool CGXDLMSSNSettings::GetMultibleReferences()
{
    return GXHelpers::GetBits(m_ConformanceBlock[1], 0x2);
}

void CGXDLMSSNSettings::SetMultibleReferences(bool newVal)
{
    GXHelpers::SetBits(m_ConformanceBlock[1], 0x2, newVal);
}

//Bit 15
bool CGXDLMSSNSettings::GetInformationReport()
{
    return GXHelpers::GetBits(m_ConformanceBlock[1], 0x1);
}

void CGXDLMSSNSettings::SetInformationReport(bool newVal)
{
    GXHelpers::SetBits(m_ConformanceBlock[1], 0x1, newVal);
}

bool CGXDLMSSNSettings::GetDataNotification()
{
    return GXHelpers::GetBits(m_ConformanceBlock[2], 0x80);
}

void CGXDLMSSNSettings::SetDataNotification(bool value)
{
    GXHelpers::SetBits(m_ConformanceBlock[2], 0x80, value);
}

//Bit 18
bool CGXDLMSSNSettings::GetParametrizedAccess()
{
    return GXHelpers::GetBits(m_ConformanceBlock[2], 0x20);
}

void CGXDLMSSNSettings::SetParametrizedAccess(bool newVal)
{
    GXHelpers::SetBits(m_ConformanceBlock[2], 0x20, newVal);
}