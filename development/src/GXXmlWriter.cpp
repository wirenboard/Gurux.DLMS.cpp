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

#include <string.h>
#include "../include/GXXmlWriter.h"

CGXXmlWriter::CGXXmlWriter(FILE* f)
{
    m_f = f;
}

int CGXXmlWriter::AppendSpaces()
{
    for (size_t pos = 0; pos != m_Elements.size(); ++pos)
    {
        fprintf(m_f, "%s", "  ");
    }
    return 0;
}

int CGXXmlWriter::Write(const char* data)
{
    fprintf(m_f, "%s", data);
    return 0;
}

int CGXXmlWriter::WriteStartDocument()
{
    m_Elements.clear();
    const char* str = "<?xml version=\"1.0\">";
    fprintf(m_f, "%s%s", str, NEW_LINE);
    return 0;
}

int CGXXmlWriter::WriteStartElement(const char* name)
{
    AppendSpaces();
    m_Elements.push_back(name);
    fprintf(m_f, "<%s>%s", name, NEW_LINE);
    return 0;
}

int CGXXmlWriter::WriteElementString(const char* name, int value)
{
    if (value != 0)
    {
        char tmp[20];
        printf("%d", value);
        WriteElementString(name, tmp);
    }
    return 0;
}

int CGXXmlWriter::WriteElementString(const char* name, const char* value)
{
    if (value != NULL && strlen(value) != 0)
    {
        AppendSpaces();
        fprintf(m_f, "<%s>%s</%s>%s", name, value, name, NEW_LINE);
    }
    return 0;
}

int CGXXmlWriter::WriteElementString(const char* name, const std::string& value)
{
    return WriteElementString(name, value.c_str());
}

int CGXXmlWriter::WriteEndElement(bool addSpaces)
{
    std::string item = m_Elements.back();
    m_Elements.pop_back();
    if (addSpaces)
    {
        AppendSpaces();
    }
    Write("</");
    Write(item.c_str());
    Write(">");
    Write(NEW_LINE);
    return 0;
}

int CGXXmlWriter::WriteEndElement()
{
    return WriteEndElement(true);
}

int CGXXmlWriter::WriteEndDocument()
{
    return 0;
}
