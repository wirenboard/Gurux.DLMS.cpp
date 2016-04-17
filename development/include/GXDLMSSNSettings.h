//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL:  $
//
// Version:         $Revision:  $,
//                  $Date:  $
//                  $Author: $
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------

#ifndef GXDLMSSNSETTINGS_H
#define GXDLMSSNSETTINGS_H

#include <string.h>

class CGXDLMSSNSettings
{
public:
    CGXDLMSSNSettings(void);
    ~CGXDLMSSNSettings(void);

    unsigned char m_ConformanceBlock[3];

    void SetConformanceBlock(unsigned char* pBuff)
    {
        memcpy(pBuff, m_ConformanceBlock, 3);
//		DumpValues();
    }

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

    bool GetRead();
    void SetRead(bool newVal);
    bool GetWrite();
    void SetWrite(bool newVal);
    bool GetUnconfirmedWrite();
    void SetUnconfirmedWrite(bool newVal);

    /**
    * @return Can data read in blocks.
    */
    bool GetReadBlockTransfer();

    /**
     * @param value
     *            Can data read in blocks.
     */
    void SetReadBlockTransfer(bool value);

    /**
     * @return Can data write in blocks.
     */
    bool GetWriteBlockTransfer();

    /**
     * @param value
     *            Can data write in blocks.
     */
    void SetWriteBlockTransfer(bool value);

    bool GetMultibleReferences();
    void SetMultibleReferences(bool newVal);

    bool GetInformationReport();
    void SetInformationReport(bool newVal);

    /**
    * @return Is data notification supported.
    */
    bool GetDataNotification();

    /**
     * @param value
     *            Is data notification supported.
     */
    void SetDataNotification(bool value);

    bool GetParametrizedAccess();
    void SetParametrizedAccess(bool newVal);
};
#endif //GXDLMSSNSETTINGS_H