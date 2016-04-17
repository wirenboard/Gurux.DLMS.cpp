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

#ifndef GXDLMSLNSETTINGS_H
#define GXDLMSLNSETTINGS_H

#include <string.h>

struct CGXDLMSLNSettings
{
public:
    CGXDLMSLNSettings(void);
    ~CGXDLMSLNSettings(void);

    unsigned char m_ConformanceBlock[3];
    void SetConformanceBlock(unsigned char* pBuff)
    {
        memcpy(m_ConformanceBlock, pBuff, 3);
        //DumpValues();
    }

    /**
    * Clear all bits.
    */
    void Clear();

    /**
     * @return Is general protection supported.
     */
    bool GetGeneralProtection();

    /**
     * @param value
     *            Is general protection supported.
     */
    void SetGeneralProtection(bool value);

    /**
     * @return Is general block transfer supported.
     */
    bool GetGeneralBlockTransfer();

    /**
     * @param value
     *            Is general block transfer supported.
     */
    void SetGeneralBlockTransfer(bool value);

    bool GetAttribute0SetReferencing();
    void SetAttribute0SetReferencing(bool newVal);
    bool GetPriorityManagement();
    void SetPriorityManagement(bool newVal);
    bool GetAttribute0GetReferencing();
    void SetAttribute0GetReferencing(bool newVal);
    bool GetGetBlockTransfer();
    void SetGetBlockTransfer(bool newVal);
    bool GetSetBlockTransfer();
    void SetSetBlockTransfer(bool newVal);
    bool GetActionBlockTransfer();
    void SetActionBlockTransfer(bool newVal);
    bool GetMultibleReferences();
    void SetMultibleReferences(bool newVal);

    /**
    * @return Is data notification supported.
    */
    bool GetDataNotification();

    /**
     * @param value
     *            Is data notification supported.
     */
    void SetDataNotification(bool value);

    /**
     * @return Is access used.
     */
    bool GetAccess();

    /**
     * @param value
     *            Is access used.
     */
    void SetAccess(bool value);

    bool GetGet();
    void SetGet(bool newVal);
    bool GetSet();
    void SetSet(bool newVal);
    bool GetSelectiveAccess();
    void SetSelectiveAccess(bool newVal);

    /**
     * @return Is server supporting event notifications.
     */
    bool GetEventNotification();
    /**
     * @param value
     *            Is server supporting event notifications.
     */
    void SetEventNotification(bool newVal);

    /**
     * @return Can client call actions (methods).
     */
    bool GetAction();
    /**
     * @param value
     *            Can client call actions (methods).
     */
    void SetAction(bool newVal);
};
#endif //GXDLMSLNSETTINGS_H