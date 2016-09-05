//----------------------------------------------------------------------------
//
//  $Workfile: RNDOClient.cs$
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
using System.Net.Sockets;
using System.Threading;
using System.Net;
using System.Diagnostics;
using RNDO;

namespace RNDOClientInterface
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: RNDOClient
    // 
    // Purpose:
    //      This is the main class for handling tools to send and receive messages 
    //      from the server
    //
    //----------------------------------------------------------------------------
    public class RNDOClient
    {
        List<RNDOReceiveClient> mListRNDOReceiveClient = new List<RNDOReceiveClient>();
        List<RNDOTransmitClient> mListRNDOTransmitClient = new List<RNDOTransmitClient>();

        public RNDOClient()
        {
            ConfigFile.loadDefFile();
        }

        public UInt32 openListenerPort(string name)
        {
            for (UInt32 index = 0; index < mListRNDOReceiveClient.Count; index++)
            {
                if (name == mListRNDOReceiveClient[(int)index].mName)
                {
                    return index;
                }
            }

            RNDOReceiveClient tcpipClient = new RNDOReceiveClient(name);
            tcpipClient.Start();
            mListRNDOReceiveClient.Add(tcpipClient);
            return (UInt32)mListRNDOReceiveClient.Count - 1;
        }

        public UInt32 openSenderPort(string name)
        {
            for (UInt32 index = 0; index < mListRNDOTransmitClient.Count; index++)
            {
                if (name == mListRNDOTransmitClient[(int)index].mName)
                {
                    return index;
                }
            }

            RNDOTransmitClient tcpipTransmitClient = new RNDOTransmitClient(name);
            tcpipTransmitClient.Start();
            mListRNDOTransmitClient.Add(tcpipTransmitClient);
            return (UInt32)mListRNDOTransmitClient.Count - 1;
        }

        public RNDOMessage getReceiveRNDOMessage(UInt32 listenerIndex, UInt16 id)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOReceiveClient.Count))
            {
                return mListRNDOReceiveClient[(int)listenerIndex].GetRNDOMessage(id);
            }

            return null;
        }

        public RNDOMessage getReceiveRNDOMessage(UInt32 listenerIndex, UInt16 id, UInt16 robot)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOReceiveClient.Count))
            {
                return mListRNDOReceiveClient[(int)listenerIndex].GetRNDOMessage(id, robot);
            }

            return null;
        }

        public RNDOMessage getTransNDOMessage(UInt32 listenerIndex, UInt16 id)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOTransmitClient.Count))
            {
                return mListRNDOTransmitClient[(int)listenerIndex].GetRNDOMessage(id);
            }

            return null;
        }

        public RNDOMessage getTransNDOMessage(UInt32 listenerIndex, UInt16 id, UInt16 robot)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOTransmitClient.Count))
            {
                return mListRNDOTransmitClient[(int)listenerIndex].GetRNDOMessage(id,robot);
            }

            return null;
        }

        public UInt32 getRecievedCount(UInt32 listenerIndex)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOReceiveClient.Count))
            {
                return mListRNDOReceiveClient[(int)listenerIndex].mReceivedCount;
            }
            return 0;
        }

        public UInt32 getTransmitCount(UInt32 listenerIndex)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOTransmitClient.Count))
            {
                return mListRNDOTransmitClient[(int)listenerIndex].mTransmitCount;
            }
            return 0;
        }

        public TimeSpan getRecievedTimeSpan(UInt32 listenerIndex)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOReceiveClient.Count))
            {
                return mListRNDOReceiveClient[(int)listenerIndex].mReceivedTimeSpan;
            }
            return new TimeSpan();
        }

        public TimeSpan getTransmitTimeSpan(UInt32 listenerIndex)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOTransmitClient.Count))
            {
                return mListRNDOTransmitClient[(int)listenerIndex].mTransmitTimeSpan;
            }
            return new TimeSpan();
        }

        public void closeListenerPort(UInt32 listenerIndex)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOReceiveClient.Count))
            {
                mListRNDOReceiveClient[(int)listenerIndex].Stop();
            }
        }

        public void closeSenderPort(UInt32 listenerIndex)
        {
            if ((listenerIndex >= 0) && (listenerIndex < mListRNDOTransmitClient.Count))
            {
                mListRNDOTransmitClient[(int)listenerIndex].Stop();
            }
        }
    }
}
