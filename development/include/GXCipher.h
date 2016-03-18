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
public:
    /**
     * Encrypt PDU.
     *
     * command : Command.
     * data : Data to encrypt.
     * Returns : Encrypted data.
     */
    int Encrypt(DLMS_COMMAND command, CGXByteBuffer& data, CGXByteBuffer& reply)
    {
        //Encryption is not supported in C++ at the moment.
        return 0;
    }

    /**
     * Decrypt data.
     *
     * data : Decrypted data.
     */
    int Decrypt(CGXByteBuffer& data)
    {
        //Decryption is not supported in C++ at the moment.
        return 0;
    }
    bool IsCiphered()
    {
        //Decryption is not supported in C++ at the moment.
        return false;
    }
};
#endif //GXCIPHER_H