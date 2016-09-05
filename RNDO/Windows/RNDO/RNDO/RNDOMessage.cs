//----------------------------------------------------------------------------
//
//  $Workfile: RNDOMessage.cs$
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace RNDO
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: RNDOMessage
    // 
    // Purpose:
    //      The main message that is passed around the RNDO system
    //
    //----------------------------------------------------------------------------
    public class RNDOMessage
    {
        const UInt16 MAX_DATA_BYTE_SIZE = 21;
        const UInt16 MAX_MESSAGE_TO_CHECK_SIZE = 25;
        public const UInt16 MAX_FULL_MESSAGE_SIZE = 26;
        const int LOC_ID = 0;
        const int LOC_ROBOT = 2;
        const int LOC_DATA = 4;
        const int LOC_CHECK = MAX_MESSAGE_TO_CHECK_SIZE - 1;

        UInt16 mID = 0;
        UInt16 mRobot = 0;
        UInt32 mHeader = 0;
        byte[] mData = new byte[MAX_DATA_BYTE_SIZE];
        byte  mCheck = 0;
        DateTime mLastUpdate = new DateTime();

        public RNDOMessage() 
        {
            for (byte i = 0; i < MAX_DATA_BYTE_SIZE; i++)
            {
                mData[i] = 0x00;
            }
            mLastUpdate = DateTime.Now;
        }

        public RNDOMessage(UInt16 id)
        {
            mID = id;
            mRobot = 0;

            mHeader = (UInt32)((UInt32)(mID << 16)) + mRobot;

            for (byte i = 0; i < MAX_DATA_BYTE_SIZE; i++)
            {
                mData[i] = 0x00;
            }
            mLastUpdate = DateTime.Now;
        }

        public RNDOMessage(byte[] data, int location)
        {
            mID = Utils.GetU16FrombyteArray(data, location + LOC_ID);
            mRobot = Utils.GetU16FrombyteArray(data, location + LOC_ROBOT);
            mHeader = (UInt32)((UInt32)(mID << 16)) + mRobot;
            Utils.PutbyteArrayTobyteArrayLocationFromData(data, mData, location + LOC_DATA, MAX_DATA_BYTE_SIZE);
            SetCheck();
            mLastUpdate = DateTime.Now;
        }

        public RNDOMessage(UInt16 id, UInt16 robot)
        {
            mID = id;
            mRobot = robot;
            mHeader = (UInt32)((UInt32)(mID << 16)) + mRobot;
            for (byte i = 0; i < MAX_DATA_BYTE_SIZE; i++)
            {
                mData[i] = 0x00;
            }
        }

        public UInt32 GetHeader()
        {
            return mHeader;
        }

        public UInt16 GetID()
        {
            return mID;
        }

        public UInt16 GetRobot()
        {
            return mRobot;
        }

        public byte GetCheck()
        {
            return mCheck;
        }

        public string GetDataString()
        {
            string returnString = "";

            for (int i = 0; i < MAX_DATA_BYTE_SIZE;i++)
            {
                returnString += mData[i].ToString("X2");
            }
            return returnString;
        }

        public void SetData(byte[] data)
        {
            int i = 0;
            for (i = 0; i < MAX_DATA_BYTE_SIZE; i++)
            {
                mData[i] = data[i];
            }
            SetCheck();
        }

        public byte[] GetData()
        {
            return mData;
        }


        public UInt32 Get32BitBlock(int whichBlock)
        {
            int startByte = whichBlock * 4;
            UInt32 data = 0;

            data =  (UInt32)(mData[startByte] << 24);
            data += (UInt32)(mData[startByte + 1] << 16);
            data += (UInt32)(mData[startByte + 2] << 8);
            data += mData[startByte + 3];

            return data;
        }

        public UInt16 Get16BitBlock(int whichBlock)
        {
            int startByte = whichBlock * 2;
            UInt16 data = 0;

            data = (UInt16)(mData[startByte] << 8);
            data += mData[startByte + 1];
            return data;
        }

        public double GetDoublebitBlock(int whichBlock, int decimalDigits)
        {
            UInt32 data = Get32BitBlock(whichBlock);

            return (double)((double)(data) / (double)(1 << decimalDigits));
        }

        public byte Get8BitBlock(int whichBlock)
        {
            return mData[whichBlock];
        }

        public void Set32BitBlock(UInt32 data, int whichBlock)
        {
            int startByte = whichBlock * 4;

            mData[startByte] = (byte)(data >> 24);
            mData[startByte + 1] = (byte)(data >> 16);
            mData[startByte + 2] = (byte)(data >> 8);
            mData[startByte + 3] = (byte)(data);
            SetCheck();
        }

        public void Set16BitBlock(UInt16 data, int whichBlock)
        {
            int startByte = whichBlock * 2;

            mData[startByte] = (byte)(data >> 8);
            mData[startByte + 1] = (byte)(data);
            SetCheck();
        }

        public void SetDoublebitBlock(double data, int whichBlock, int decimalDigits)
        {
            UInt32 dataToSave = (UInt32)(data*(1 << decimalDigits));
            Set32BitBlock(dataToSave, whichBlock);
            SetCheck();
        }

        public void Set8BitBlock(byte data, int whichBlock)
        {
            mData[whichBlock] = data;
            SetCheck();
        }

        public void GetByteArray(byte[] data)
        {
            GetByteArray(data, 0);
        }

        public void GetByteArray(byte[] data,int loc)
        {
            Utils.PutU16TobyteArray(mID, data, LOC_ID+loc);
            Utils.PutU16TobyteArray(mRobot, data, LOC_ROBOT+loc);
            Utils.PutbyteArrayTobyteArrayLocationToData(mData, data, LOC_DATA+loc, MAX_DATA_BYTE_SIZE);
            data[loc+MAX_FULL_MESSAGE_SIZE - 1] = mCheck;
        }

        public bool GetStale()
        {
            if(DateTime.Now.Subtract(mLastUpdate).TotalMilliseconds > 2000)
            {
                return true;
            }
            return false;
        }


        public void SetCheck()
        {
            byte[] data = new byte[MAX_MESSAGE_TO_CHECK_SIZE];

            Utils.PutU16TobyteArray(mID, data, LOC_ID);
            Utils.PutU16TobyteArray(mRobot, data, LOC_ROBOT);
            Utils.PutbyteArrayTobyteArrayLocationToData(mData, data, LOC_DATA, MAX_DATA_BYTE_SIZE);

            mCheck = 0xFF;

            for (int index = 0; index < MAX_MESSAGE_TO_CHECK_SIZE; index++)
            {
                mCheck ^= data[index];
            }
            mLastUpdate = DateTime.Now;
        }
    }
}
