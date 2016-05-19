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

#ifndef GXCIPHER_H
#define GXCIPHER_H

class CGXCipher
{
    DLMS_SECURITY m_Security;
    /**
    * System title.
    */
    CGXByteBuffer m_SystemTitle;

    /**
    *  Block cipher key.
    */
    CGXByteBuffer m_BlockCipherKey;
    /**
    *  Block Authentication key.
    */
    CGXByteBuffer m_AuthenticationKey;

    /**
     * Frame counter. AKA Invocation counter.
     */
    unsigned long m_FrameCounter;

public:
    /**
    * Constructor.
    */
    CGXCipher()
    {
    	m_FrameCounter = 0;
        m_Security = DLMS_SECURITY_NONE;
    }
    /**
      * Encrypt PDU.
      *
      * @param tag
      *            Tag.
      * @param systemTitle
      *            System Title.
      * @param data
      *            Data to encrypt.
      * @param reply
      *            Encrypted data.
      */
    int Encrypt(unsigned char tag, CGXByteBuffer& systemTitle, CGXByteBuffer& data, CGXByteBuffer& reply)
    {
        //Encryption is not supported in C++ at the moment.
        return 0;
    }

    /**
      * Decrypt data.
      *
      * @param systemTitle
      *            System Title.
      * @param data
      *            Decrypted data.
      * @param security
      *            Used security level.
      */
    int Decrypt(CGXByteBuffer& title, CGXByteBuffer& data, DLMS_SECURITY& security)
    {
    	security = DLMS_SECURITY_NONE;
        //Decryption is not supported in C++ at the moment.
        return 0;
    }

    /**
     * @return Is ciphering used.
     */
    bool IsCiphered()
    {
        //Decryption is not supported in C++ at the moment.
        return false;
    }
    /**
     * @return Used security.
     */
    DLMS_SECURITY GetSecurity()
    {
        return m_Security;
    }

    /**
    * @param value
    *            Used security.
    */
    void SetSecurity(DLMS_SECURITY value)
    {
        m_Security = value;
    }

    /**
     * @return System title.
     */
    CGXByteBuffer& GetSystemTitle()
    {
        return m_SystemTitle;
    }

    /**
    *  @param value System title.
    */
    void SetSystemTitle(CGXByteBuffer& value)
    {
        m_SystemTitle.Clear();
        m_SystemTitle.Set(&value);
    }

    /**
     * @return Block cipher key.
     */
    CGXByteBuffer& GetBlockCipherKey()
    {
        return m_BlockCipherKey;
    }

    /**
    *  @param value Block cipher key.
    */
    void SetBlockCipherKey(CGXByteBuffer& value)
    {
        m_BlockCipherKey.Clear();
        m_BlockCipherKey.Set(&value);
    }

    /**
     * @return Authentication key.
     */
    CGXByteBuffer& GetAuthenticationKey()
    {
        return m_AuthenticationKey;
    }

    /**
     * @param value
     *            Authentication key.
     */
    void SetAuthenticationKey(CGXByteBuffer& value)
    {
        m_AuthenticationKey.Clear();
        m_AuthenticationKey.Set(&value);
    }

    /**
     * @return Frame counter. Invocation counter.
     */
    unsigned long GetFrameCounter()
    {
        return m_FrameCounter;
    }

    void Reset()
    {

    }
};
#endif //GXCIPHER_H
