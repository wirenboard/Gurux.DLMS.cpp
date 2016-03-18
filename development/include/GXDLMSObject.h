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

#ifndef GXDLMSOBJECT_H
#define GXDLMSOBJECT_H

#include "GXAttributeCollection.h"
#include "GXDLMSVariant.h"
#include "IGXDLMSBase.h"
#include "GXHelpers.h"
#include "GXDateTime.h"

class CGXDLMSObjectCollection;

class CGXDLMSObject : public IGXDLMSBase
{
    friend class CGXDLMSClient;
    friend class CGXDLMSObjectCollection;
    friend class CGXDLMSObjectFactory;
    friend class CGXDLMSAssociationLogicalName;
    friend class CGXDLMSAssociationShortName;

    CGXAttributeCollection m_Attributes;
    CGXAttributeCollection m_MethodAttributes;
    void Initialize(short sn, unsigned short class_id, unsigned char version, CGXByteBuffer* pLogicalName);
    std::string m_Description;
    OBJECT_TYPE m_ObjectType;
    char m_AttributeIndex;
    unsigned short m_DataIndex;
    unsigned short m_Version;
protected:
    std::map<int, time_t> m_ReadTimes;
    CGXDLMSObjectCollection* m_Parent;
    unsigned short m_SN;
    unsigned char m_LN[6];

    /*
     * Is attribute read. This can be used with static attributes to make
     * meter reading faster.
     */
    bool IsRead(int index);
    bool CanRead(int index);
    static int GetLogicalName(CGXDLMSObject * target, CGXDLMSVariant& value);
    static int SetLogicalName(CGXDLMSObject * target, CGXDLMSVariant& value);
public:

    static bool IsLogicalNameEmpty(unsigned char* pLN)
    {
        const unsigned char EmptyLN[] = {0, 0, 0, 0, 0, 0};
        return memcmp(pLN, EmptyLN, 6) == 0;
    }

    CGXDLMSObject(void);
    CGXDLMSObject(OBJECT_TYPE type);

    //SN Constructor.
    CGXDLMSObject(OBJECT_TYPE type, unsigned short sn);

    //LN Constructor.
    CGXDLMSObject(OBJECT_TYPE type, std::string ln);
    CGXDLMSObject(short sn, unsigned short class_id, unsigned char version, CGXByteBuffer& ln);

    virtual ~CGXDLMSObject(void);

    //Get Object's Logical or Short Name as a std::string.
    CGXDLMSVariant GetName();

    int SetName(CGXDLMSVariant value);

    //Get Object's Interface class type.
    OBJECT_TYPE GetObjectType();

    //Get Object's Short Name.
    unsigned short GetShortName();

    //Set Object's Short Name.
    void SetShortName(unsigned short value);

    //Get Object's Logical Name.
    void GetLogicalName(std::string& ln);

    void SetVersion(unsigned short value);
    unsigned short GetVersion();

    CGXAttributeCollection& GetAttributes();
    CGXAttributeCollection& GetMethodAttributes();
    virtual int SetDataType(int index, DLMS_DATA_TYPE type);
    virtual int GetDataType(int index, DLMS_DATA_TYPE& type);

    virtual int GetUIDataType(int index, DLMS_DATA_TYPE& type);
    void SetUIDataType(int index, DLMS_DATA_TYPE type);

    ACCESSMODE GetAccess(int index);
    void SetAccess(int index, ACCESSMODE access);
    METHOD_ACCESSMODE GetMethodAccess(int index);
    void SetMethodAccess(int index, METHOD_ACCESSMODE access);


    //Get description of the object.
    std::string GetDescription();

    //Set description of the object.
    void SetDescription(std::string value);

    //Get values as std::string.
    virtual void GetValues(std::vector<std::string>& values)
    {
        assert(0);
    }

    virtual void GetAttributeIndexToRead(std::vector<int>& attributes)
    {
        assert(0);
    }

    // Returns amount of attributes.
    virtual int GetAttributeCount()
    {
        assert(0);
        return 0;
    }

    // Returns amount of methods.
    virtual int GetMethodCount()
    {
        assert(0);
        return 0;
    }

    // Returns value of given attribute.
    virtual int GetValue(int index, int selector, CGXDLMSVariant& parameters, CGXDLMSVariant& value)
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }

    // Set value of given attribute.
    virtual int SetValue(CGXDLMSSettings* settings, int index, CGXDLMSVariant& value)
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }

    // Set value of given attribute.
    virtual int Invoke(int index, CGXDLMSVariant& parameters)
    {
        return ERROR_CODES_INVALID_PARAMETER;
    }
};

class CGXDLMSCustomObject : public CGXDLMSObject
{
public:
    CGXDLMSCustomObject(OBJECT_TYPE type) : CGXDLMSObject(type)
    {
    }
};

#endif //GXDLMSOBJECT_H