//----------------------------------------------------------------------------
//
//  $Workfile: RNDOMessage.cpp$
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
#include <iostream>
#include "Utils.h"
#include "RNDOMessage.h"

using namespace std;

RNDOMessage::RNDOMessage() :
mID(0x00),
mRobot(0x00),
mCheck(0x00)
{
    for (byte i = 0; i < MAX_DATA_BYTE_SIZE; i++)
    {
        mData[i] = 0x00;
    }
    SetCheck();
    mValid = false;
}

RNDOMessage::RNDOMessage(UInt16 id) :
mID(id),
mRobot(0x00),
mCheck(0x00)
{
    cout << "Adding " << id << endl;

    for (byte i = 0; i < MAX_DATA_BYTE_SIZE; i++)
    {
        mData[i] = 0x00;
    }
    SetCheck();
    mValid = false;
}

RNDOMessage::RNDOMessage(UInt16 id, UInt16 robot) :
mID(id),
mRobot(robot),
mCheck(0x00)
{
    cout << "Adding " << id << endl;

    for (byte i = 0; i < MAX_DATA_BYTE_SIZE; i++)
    {
        mData[i] = 0x00;
    }
    SetCheck();
    mValid = false;
}

RNDOMessage::RNDOMessage(byte* data)
{
    mID = GetU16FrombyteArray(data, LOC_ID);
    mRobot = GetU16FrombyteArray(data, LOC_R);
    PutbyteArrayTobyteArrayLocationFrom(data, mData, LOC_DATA, MAX_DATA_BYTE_SIZE);
    byte check = data[MAX_MESSAGE_TO_CHECK_SIZE];
    SetCheck();

    if (check != mCheck)
    {
        mValid = false;
    }

    cout << "Adding " << mID << endl;
}

UInt32 RNDOMessage::GetHeader()
{
    return ((mID<<16)+mRobot);
}


void RNDOMessage::SetData(byte* data)
{
    int i = 0;
    //    cout << "Setting for " << mID << ":" << endl;
    for (i = 0; i < MAX_DATA_BYTE_SIZE; i++)
    {
        mData[i] = data[i];
        //        printf("%2x ", mData[i]);
    }
    //    cout << endl;
    SetCheck();
}

UInt32 RNDOMessage::Get32BitBlock(int whichBlock)
{
    int startByte = whichBlock * 4;
    UInt32 data = 0;

    data = mData[startByte] << 24;
    data += mData[startByte+1] << 16;
    data += mData[startByte+2] << 8;
    data += mData[startByte+3];

    return data;
}

UInt16 RNDOMessage::Get16BitBlock(int whichBlock)
{
    int startByte = whichBlock * 2;
    UInt16 data = 0;

    data = mData[startByte] << 8;
    data += mData[startByte + 1];
    return data;
}

double RNDOMessage::GetDoublebitBlock(int whichBlock, int decimalDigits)
{
    UInt32 data = Get32BitBlock(whichBlock);
    
    return (double)((double)(data) / (double)(decimalDigits));
}

byte  RNDOMessage::Get8BitBlock(int whichBlock)
{
    return mData[whichBlock];
}

void RNDOMessage::Set32BitBlock(UInt32 data, int whichBlock)
{
    int startByte = whichBlock * 4;

    mData[startByte] = (byte)(data >> 24);
    mData[startByte+1] = (byte)(data >> 16);
    mData[startByte+2] = (byte)(data >> 8);
    mData[startByte+3] = (byte)(data);
    SetCheck();
}

void RNDOMessage::Set16BitBlock(UInt16 data, int whichBlock)
{
    int startByte = whichBlock * 2;

    mData[startByte] = (byte)(data >> 8);
    mData[startByte + 1] = (byte)(data);
    SetCheck();
}

void RNDOMessage::SetDoublebitBlock(double data, int whichBlock, int decimalDigits)
{
    UInt32 dataToSave = (UInt32)(data*(decimalDigits));
    Set32BitBlock(dataToSave,whichBlock);
    SetCheck();
}

void RNDOMessage::Set8BitBlock(byte data, int whichBlock)
{
    mData[whichBlock] = data;
    SetCheck();
}

void RNDOMessage::GetByteArray(byte* data)
{
	GetByteArray(data,0);
}

void RNDOMessage::GetByteArray(byte* data, int loc)
{
    PutU16TobyteArray(mID, data, LOC_ID+loc);
    PutU16TobyteArray(mRobot, data, LOC_R+loc);
    PutbyteArrayTobyteArrayLocationTo(mData, data, LOC_DATA+loc, MAX_DATA_BYTE_SIZE);
    data[(MAX_FULL_MESSAGE_SIZE - SIZE_CHECK)+loc] = mCheck;
}

bool RNDOMessage::IsStale()
{
    if ((clock() - mLastUpdate) > MAX_STALE_TIME)
    {
        return true;
    }

    return false;
}

bool RNDOMessage::IsValid()
{
    return mValid;
}

void RNDOMessage::SetCheck(void)
{
    byte data[MAX_MESSAGE_TO_CHECK_SIZE];

    PutU16TobyteArray(mID, data, LOC_ID);
    PutU16TobyteArray(mRobot, data, LOC_R);
    PutbyteArrayTobyteArrayLocationTo(mData, data, LOC_DATA, MAX_DATA_BYTE_SIZE);

    mCheck = 0xFF;

    for (int index = 0; index < MAX_MESSAGE_TO_CHECK_SIZE; index++)
    {
        mCheck ^= data[index];
    }
    mValid = true;
    mLastUpdate = clock();

}
