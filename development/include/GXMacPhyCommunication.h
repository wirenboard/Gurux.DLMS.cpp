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

#ifndef GXMACPHYCOMMUNICATION_H
#define GXMACPHYCOMMUNICATION_H

//MAC PHY communication parameters.
class CGXMacPhyCommunication
{
    /*
    * EUI is the EUI-48 of the other device.
    */
    CGXByteBuffer m_Eui;

    /*
     * The tx power of GPDU packets sent to the device.
     */
    short m_TxPower;

    /*
     * The Tx coding of GPDU packets sent to the device.
     */
    short m_TxCoding;

    /*
     * The Rx coding of GPDU packets received from the device.
     */
    short m_RxCoding;
    /*
     * The Rx power level of GPDU packets received from the device.
     */
    short m_RxLvl;
    /*
     * SNR of GPDU packets received from the device.
     */
    short m_Snr;
    /*
     * The number of times the Tx power was modified.
     */
    short m_TxPowerModified;
    /*
     * The number of times the Tx coding was modified.
     */
    short m_TxCodingModified;
    /*
     * The number of times the Rx coding was modified.
     */
    short m_RxCodingModified;
public:
    /*
     * Returns EUI is the EUI-48 of the other device.
     */
    CGXByteBuffer& GetEui()
    {
        return m_Eui;
    }

    /*
     * value:
     *            EUI is the EUI-48 of the other device.
     */
    void SetEui(CGXByteBuffer& value)
    {
        m_Eui = value;
    }

    /*
     * Returns The tx power of GPDU packets sent to the device.
     */
    short GetTxPower()
    {
        return m_TxPower;
    }

    /*
     * value:
     *            The tx power of GPDU packets sent to the device.
     */
    void SetTxPower(short value)
    {
        m_TxPower = value;
    }

    /*
     * Returns The Tx coding of GPDU packets sent to the device.
     */
    short GetTxCoding()
    {
        return m_TxCoding;
    }

    /*
     * value:
     *            The Tx coding of GPDU packets sent to the device.
     */
    void SetTxCoding(short value)
    {
        m_TxCoding = value;
    }

    /*
     * Returns The Rx coding of GPDU packets received from the device.
     */
    short GetRxCoding()
    {
        return m_RxCoding;
    }

    /*
     * value:
     *            The Rx coding of GPDU packets received from the device.
     */
    void SetRxCoding(short value)
    {
        m_RxCoding = value;
    }

    /*
     * Returns The Rx power level of GPDU packets received from the device.
     */
    short GetRxLvl()
    {
        return m_RxLvl;
    }

    /*
     * value:
     *            The Rx power level of GPDU packets received from the device.
     */
    void SetRxLvl(short value)
    {
        m_RxLvl = value;
    }

    /*
     * Returns SNR of GPDU packets received from the device.
     */
    short GetSnr()
    {
        return m_Snr;
    }

    /*
     * value:
     *            SNR of GPDU packets received from the device.
     */
    void SetSnr(short value)
    {
        m_Snr = value;
    }

    /*
     * Returns The number of times the Tx power was modified.
     */
    short GetTxPowerModified()
    {
        return m_TxPowerModified;
    }

    /*
     * value:
     *            The number of times the Tx power was modified.
     */
    void SetTxPowerModified(short value)
    {
        m_TxPowerModified = value;
    }

    /*
     * Returns The number of times the Tx coding was modified.
     */
    short GetTxCodingModified()
    {
        return m_TxCodingModified;
    }

    /*
     * value:
     *            The number of times the Tx coding was modified.
     */
    void SetTxCodingModified(short value)
    {
        m_TxCodingModified = value;
    }

    /*
     * Returns The number of times the Rx coding was modified.
     */
    short GetRxCodingModified()
    {
        return m_RxCodingModified;
    }

    /*
     * value:
     *            The number of times the Rx coding was modified.
     */
    void SetRxCodingModified(short value)
    {
        m_RxCodingModified = value;
    }

    std::string ToString()
    {
        std::stringstream sb;
        sb << m_Eui.ToHexString();
        sb << ' ';
        sb << m_TxPower;
        sb << ' ';
        sb << m_TxCoding;
        sb << ' ';
        sb << m_RxCoding;
        sb << ' ';
        sb << m_RxLvl;
        sb << ' ';
        sb << m_Snr;
        sb << ' ';
        sb << m_TxPowerModified;
        sb << ' ';
        sb << m_TxCodingModified;
        sb << ' ';
        sb << m_RxCodingModified;
        sb << ' ';
        return sb.str();
    }

};
#endif //GXMACPHYCOMMUNICATION_H