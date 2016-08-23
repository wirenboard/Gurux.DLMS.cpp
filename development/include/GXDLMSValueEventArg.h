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
// and/or modify it under the terms of the GNU General License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXDLMSVALUEEVENTARGS_H
#define GXDLMSVALUEEVENTARGS_H

#include "enums.h"
#include "errorcodes.h"
#include "GXDLMSVariant.h"

class CGXDLMSObject;

class CGXDLMSValueEventArg
{
private:
    /**
    * CGXDLMSVariant value.
    */
    CGXDLMSVariant m_Value;
    /**
    * Is request handled.
    */
    bool m_Handled;
    /**
    * Target DLMS object
    */
    CGXDLMSObject* m_Target;
    /**
    * Attribute index.
    */
    int m_Index;
    /**
    * Optional selector.
    */
    int m_Selector;
    /**
    * Optional parameters.
    */
    CGXDLMSVariant m_Parameters;

    /**
     * Occurred error.
     */
    DLMS_ERROR_CODE m_Error;
    /**
    * Is action. This is reserved for internal use.
    */
    bool m_Action;

    /**
    * Is data handled as byte array.
    */
    bool m_ByteArray;

public:
    /**
    * @return Target DLMS object.
    */
    CGXDLMSObject* GetTarget();

    void SetTarget(CGXDLMSObject* value);

    /**
    * @return Attribute index of queried object.
    */
    int GetIndex();

    void SetIndex(int value);

    /**
    * @return CGXDLMSVariant value.
    */
    CGXDLMSVariant& GetValue();

    /**
    * @param value
    *            CGXDLMSVariant value.
    */
    void SetValue(CGXDLMSVariant value);

    /**
    * @return Data type of the value.
    */
    DLMS_DATA_TYPE GetDataType();

    /**
    * @param value
    *            Data type of the value.
    */
    void SetDataType(DLMS_DATA_TYPE value);

    /**
    * @return Is request handled.
    */
    bool GetHandled();

    /**
    * @param value
    *            Is request handled.
    */
    void SetHandled(bool value);

    /**
    * @return Optional selector.
    */
    int GetSelector();

    /**
    * @param value
    *           Selector.
    */
    void SetSelector(int value);

    /**
    * @return Optional parameters.
    */
    CGXDLMSVariant& GetParameters();

    /**
    * @param value
    *           Parameters.
    */
    void SetParameters(CGXDLMSVariant& value);


    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    */
    CGXDLMSValueEventArg(
        CGXDLMSObject* target,
        int index);

    /**
    * Constructor.
    *
    * @param target
    *            Event target.
    * @param index
    *            Event index.
    * @param selector
    *            Optional read event selector.
    * @param parameters
    *            Optional parameters.
    */
    CGXDLMSValueEventArg(
        CGXDLMSObject* target,
        int index,
        int selector,
        CGXDLMSVariant& parameters);

    /**
    * @return Occurred error.
    */
    DLMS_ERROR_CODE GetError();

    /**
     * @param error
     *            Occurred error.
     */
    void SetError(DLMS_ERROR_CODE value);

    /**
    * @return Is action.
    */
    bool IsAction();

    /**
    * @param value
    *            Is action.
    */
    void SetAction(bool value);

    /**
       * @return Is byte array.
       */
    bool IsByteArray();

    /**
    * @param value
    *            Is byte array.
    */
    void SetByteArray(bool value);
};
#endif //GXDLMSVALUEEVENTARGS_H
