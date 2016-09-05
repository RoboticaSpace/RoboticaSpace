//----------------------------------------------------------------------------
//
//  $Workfile: Utils.h$
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
#ifndef UTILS_H
#define UTILS_H

#include "Common.h"

UInt32 GetU32FrombyteArray(byte* data, int location);
UInt16 GetU16FrombyteArray(byte* data, int location);
void PutU32TobyteArray(UInt32 number, byte* data, int location);
void PutU16TobyteArray(UInt16 number, byte* data, int location);
void PutbyteArrayTobyteArrayLocationTo(byte* fromData, byte* data, int location, int size);
void PutbyteArrayTobyteArrayLocationFrom(byte* fromData, byte* data, int location, int size);
byte CalcCheckByte(byte* data, int start, int number);
byte GetHexValue(byte firstByte, byte secondByte);
byte GetHexByte(byte theByte);
void SetAChunkOfU32(UInt32* dataToSave, UInt32 dataToSet, int location, int size);
UInt32 GetAChunkOfU32(UInt32 data, int location, int size);
void SetAChunkOfU16(UInt16* dataToSave, UInt16 dataToSet, int location, int size);
UInt32 GetAChunkOfU16(UInt16 data, int location, int size);
void SetAChunkOfByte(byte* dataToSave, byte dataToSet, int location, int size);
byte GetAChunkOfByte(byte data, int location, int size);
UInt16 AddCRC(UInt16 crc, byte b);
UInt16 CalcCRC16(byte* data, int start, int size);

#endif
