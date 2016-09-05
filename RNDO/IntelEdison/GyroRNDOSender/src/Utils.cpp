//----------------------------------------------------------------------------
//
//  $Workfile: Utils.cpp$
//
//  $Revision: X$
//
//  Project:    Robot Network Data Objects
//
//                            Copyright (c) 2016
//                               James A Wright
//                            All Rights Reserved
//
//  Modification History:
//  $Log:
//  $
//
//----------------------------------------------------------------------------
#include <memory.h>
#include "Common.h"
#include "Utils.h"

/// <summary>
///     Random helper methods
/// </summary>
UInt16 lCRCtbl[] =
{
    0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
    0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
    0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
    0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
    0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
    0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
    0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
    0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
    0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
    0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
    0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
    0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
    0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
    0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
    0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
    0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
    0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
    0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
    0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
    0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
    0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
    0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
    0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
    0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
    0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
    0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
    0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
    0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
    0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
    0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
    0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
    0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Get a u32 from an array of bytes
/// </summary>
/// 
/// Returns:
/// <returns>
/// UInt32 - The number
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
UInt32 GetU32FrombyteArray(byte* data, int location)
{
    return (UInt32)((data[location] << 24) +
        (data[location + 1] << 16) +
        (data[location + 2] << 8) +
        data[location + 3]);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///      Pull a short out of a data buffer
/// </summary>
/// 
/// Returns:
/// <returns>
///     The UInt16 from the data
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
UInt16 GetU16FrombyteArray(byte* data, int location)
{
    return (UInt16)((data[location] << 8) +
        data[location + 1]);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Put a long into a data buffer
/// </summary>
/// 
/// Returns:
/// <returns>
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void PutU32TobyteArray(UInt32 number, byte* data, int location)
{
    data[location] = (byte)((number >> 24) & 0xFF);
    data[location + 1] = (byte)((number >> 16) & 0xFF);
    data[location + 2] = (byte)((number >> 8) & 0xFF);
    data[location + 3] = (byte)(number & 0xFF);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Put a short into a data buffer
/// </summary>
/// 
/// Returns:
/// <returns>
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void PutU16TobyteArray(UInt16 number, byte* data, int location)
{
    data[location] = (byte)((number >> 8) & 0xFF);
    data[location + 1] = (byte)(number & 0xFF);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Put a data buffer into a data buffer
/// </summary>
/// 
/// Returns:
/// <returns>
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void PutbyteArrayTobyteArrayLocationTo(byte* fromData, byte* data, int location, int size)
{
    for (int index = 0; index < size; index++)
    {
        data[location + index] = fromData[index];
    }

    //    memcmp(&data[location], fromData, size);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Put a data buffer into a data buffer
/// </summary>
/// 
/// Returns:
/// <returns>
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void PutbyteArrayTobyteArrayLocationFrom(byte* fromData, byte* data, int location, int size)
{
    for (int index = 0; index < size; index++)
    {
        data[index] = fromData[location + index];
    }

    //    memcmp(&data[location], fromData, size);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///      Calc a check byte from the data
/// </summary>
/// 
/// Returns:
/// <returns>
///     Return the check byte for a packet
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
byte CalcCheckByte(byte* data, int start, int number)
{
    byte checkByte = 0xFF;

    for (int index = 0; index < number; index++)
    {
        checkByte ^= data[start + index];
    }
    return checkByte;
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Return a number from two hex digits
/// </summary>
/// 
/// Returns:
/// <returns>
///     Return byte from two digits
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
byte GetHexValue(byte firstByte, byte secondByte)
{
    return (byte)((GetHexByte(firstByte) << 4) + GetHexByte(secondByte));
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Return a number from a hex digit
/// </summary>
/// 
/// Returns:
/// <returns>
///     Return byte from the digit
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
byte GetHexByte(byte theByte)
{
    if ((theByte >= 'A') & (theByte <= 'F'))
    {
        theByte -= (byte)'A';
        theByte += 10;
    }
    if ((theByte >= 'a') & (theByte <= 'f'))
    {
        theByte -= (byte)'a';
        theByte += 10;
    }
    if ((theByte >= '0') & (theByte <= '9'))
    {
        theByte -= (byte)'0';
    }
    return theByte;
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Sets the chunk of the data field of the data packet
/// </summary>
/// 
/// Returns:
/// <returns>
///     Return the int32
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void SetAChunkOfU32(UInt32* dataToSave, UInt32 dataToSet, int location, int size)
{
    UInt32 newSetMask = (UInt32)((1 << size) - 1);
    newSetMask <<= location;

    UInt32 newClearMask = ~newSetMask;

    UInt32 newData = dataToSet << location;
    newData &= newSetMask;

    *dataToSave &= newClearMask;
    *dataToSave |= newData;
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Gets the chunk of the data field of the data packet
/// </summary>
/// 
/// Returns:
/// <returns>
///     The block from the UInt32
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
UInt32 GetAChunkOfU32(UInt32 data, int location, int size)
{
    int newRollRightValue = (location);

    UInt32 newClearMask = (UInt32)((1 << size) - 1);

    return ((data >> newRollRightValue) & newClearMask);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Sets the chunk of the data field of the data packet
/// </summary>
/// 
/// Returns:
/// <returns>
///     The block from the UInt16
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void SetAChunkOfU16(UInt16* dataToSave, UInt16 dataToSet, int location, int size)
{
    UInt32 tempData = (UInt32)dataToSave;
    SetAChunkOfU32(&tempData, dataToSet, location, size);
    *dataToSave = (UInt16)tempData;
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Gets the chunk of the data field of the data packet
/// </summary>
/// 
/// Returns:
/// <returns>
///     The block from the UInt16
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
UInt32 GetAChunkOfU16(UInt16 data, int location, int size)
{
    return (UInt16)GetAChunkOfU32(data, location, size);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Sets the chunk of the data field of the data packet
/// </summary>
/// 
/// Returns:
/// <returns>
///     The block from the byte
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
void SetAChunkOfByte(byte* dataToSave, byte dataToSet, int location, int size)
{
    UInt32 tempData = (UInt32)dataToSave;
    SetAChunkOfU32(&tempData, dataToSet, location, size);
    *dataToSave = (byte)tempData;
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///      Gets the chunk of the data field of the data packet
/// </summary>
/// 
/// Returns:
/// <returns>
///     The block from the byte
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
byte GetAChunkOfByte(byte data, int location, int size)
{
    return (byte)GetAChunkOfU32(data, location, size);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///      Build the CRC 16
/// </summary>
/// 
/// Returns:
/// <returns>
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
UInt16 AddCRC(UInt16 crc, byte b)
{
    return (UInt16)((crc << 8) ^ lCRCtbl[((crc >> 8) ^ b) & 0x00FF]);
}

///--------------------------------------------------------------------
/// Purpose:
/// <summary>
///     Build the CRC 16
/// </summary>
/// 
/// Returns:
/// <returns>
///     The CRC for the block of data.
/// </returns>
/// 
/// Notes:
/// <remarks>
///     None.
/// </remarks>
///--------------------------------------------------------------------
UInt16 CalcCRC16(byte* data, int start, int size)
{
    UInt16 crcCalced = 0xFFFF;
    for (int j = 0; j < size; j++)
    {
        crcCalced = AddCRC(crcCalced, data[j + start]);
    }
    return crcCalced;
}


