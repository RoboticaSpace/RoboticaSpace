//----------------------------------------------------------------------------
//
//  $Workfile: RNDOMessage.h$
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
#ifndef RNDOMESSAGE_H
#define RNDOMESSAGE_H

#include "common.h"
#include <time.h>

class RNDOMessage
{
public:
    static const UInt16 MAX_DATA_BYTE_SIZE = 21;
    static const UInt16 MAX_MESSAGE_TO_CHECK_SIZE = 25;
    static const UInt16 MAX_FULL_MESSAGE_SIZE = 26;
    static const UInt16 LOC_ID = 0;
    static const UInt16 LOC_R = 2;
    static const UInt16 LOC_DATA = 4;
    static const UInt16 SIZE_CHECK = 1;
    static const int MAX_STALE_TIME = 2 * CLOCKS_PER_SEC;

private:
    UInt16 mID;
    UInt16 mRobot;
    byte  mData[MAX_DATA_BYTE_SIZE];
    byte  mCheck;
    clock_t mLastUpdate;
    bool mValid;

public:
    RNDOMessage();
    RNDOMessage(UInt16 id);
    RNDOMessage(UInt16 id, UInt16 robot);
    RNDOMessage(byte* data);

    UInt32 GetHeader();

    UInt32 Get32BitBlock(int whichBlock);
    UInt16 Get16BitBlock(int whichBlock);
    double GetDoublebitBlock(int whichBlock, int decimalDigits);
    byte  Get8BitBlock(int whichBlock);
    bool IsStale();
    bool IsValid();

    void Set32BitBlock(UInt32 data, int whichBlock);
    void Set16BitBlock(UInt16 data, int whichBlock);
    void SetDoublebitBlock(double data, int whichBlock, int decimalDigits);
    void Set8BitBlock(byte data, int whichBlock);

    void SetData(byte* data);
    void GetByteArray(byte* data);
    void GetByteArray(byte* data, int loc);

private:
    void SetCheck(void);
};

#endif
