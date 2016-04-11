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

#ifndef GXBYTEBUFFER_H
#define GXBYTEBUFFER_H

#include "errorcodes.h"
#include "enums.h"

const unsigned char VECTOR_CAPACITY = 50;
class CGXByteBuffer
{
    unsigned char* m_Data;
    int m_Capacity;
    int m_Size;
    int m_Position;
public:
    //Constructor.
    CGXByteBuffer();

    //Constructor.
    CGXByteBuffer(int capacity);

    //Copy constructor.
    CGXByteBuffer(const CGXByteBuffer& value);

    //Destructor.
    ~CGXByteBuffer();

    //Returns buffer size.
    int GetSize();

    //Set buffer size.
    void SetSize(int value);

    //Increase buffer size.
    int IncreaseSize(int size);

    //Returns position of the buffer.
    int GetPosition();

    //Returns position of the buffer.
    void SetPosition(int value);

    //Allocate new size for the array in bytes.
    void Capacity(int capacity);

    //Get buffer capacity.
    int Capacity();

    //Fill buffer it with zeros.
    void Zero(int index, int count);

    //Push new data to the byteArray.
    void SetUInt8(unsigned char item);

    void SetUInt8(int index, int item);
    void SetUInt16(unsigned short item);

    void SetUInt32(unsigned long item);

    void SetUInt32ByIndex(int index, unsigned long item);

    void SetUInt64(unsigned long long item);

    void SetInt8(char item);

    void SetInt16(short item);

    void SetInt32(long item);

    void SetInt64(long long item);

    void SetFloat(float value);

    void SetDouble(double value);

    void AddRange(const void* pSource, int count);

    void AddRange2(CGXByteBuffer* data, int index, int count);

    void AddString(const char* value);

    void AttachString(char* value);

    void Clear();

    int GetUInt8(unsigned char* value);

    int GetUInt8(int index, unsigned char* value);

    int GetUInt16(unsigned short* value);

    int GetUInt32(unsigned long* value);

    int GetInt16(short* value);

    int GetInt32(long* value);

    int GetInt64(long long* value);

    int GetUInt64(unsigned long long* value);

    int GetFloat(
        float* value);

    int GetDouble(
        double* value);

    int Get(unsigned char* value, int count);

    unsigned char* GetData();

    void ToArray(unsigned char*& value, int& count);

    /**
     * Compares, whether two given arrays are similar starting from current
     * position.
     *
     * @param arr
     *            Array to compare.
     * @return True, if arrays are similar. False, if the arrays differ.
     */
    bool Compare(unsigned char* buff, int length);

    int GetUInt16(int index, unsigned short* value);
    int GetUInt32(int index, unsigned long* value);
    int GetUInt64(int index, unsigned long long* value);
    int GetUInt128(int index, unsigned char* value);

    //Get byte array as a std::string.
    std::string ToString();

    //Get byte array as hex std::string.
    std::string ToHexString();

    //Add integer value to byte array as a std::string.
    void AddIntAsString(int value);

    //Add double value to byte array as a std::string.
    void AddDoubleAsString(double value);

    //Move data insize byte array.
    int Move(int srcPos, int destPos, int count);

    //Little Endian methods.
    int GetUInt32LE(unsigned long* value);
    int GetUInt32LE(int index, unsigned long* value);
    void SetUInt32ByIndexLE(int index, unsigned long item);

    CGXByteBuffer& operator=(CGXByteBuffer& value);
};

#endif //GXBYTEBUFFER_H
