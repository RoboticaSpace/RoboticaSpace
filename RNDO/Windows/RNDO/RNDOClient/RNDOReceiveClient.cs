//----------------------------------------------------------------------------
//
//  $Workfile: RNDOReceiveClient.cs$
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
    // Class Name: RNDOReceiveClient
    // 
    // Purpose:
    //      This class handles receiving messages from the server
    //
    //----------------------------------------------------------------------------
    public class RNDOReceiveClient
    {
        public string mName;
        bool mRunThread = true;
        Thread mTCPIPThread;
        UInt32 mTCPIPPortID;
        byte[] mData;
        UInt32 mDataSize;
        List<RNDOMessage> mNDOMessages = new List<RNDOMessage>();
        public UInt32 mReceivedCount=0;
        public TimeSpan mReceivedTimeSpan = new TimeSpan();

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public RNDOReceiveClient(string name)
        {
            mName = name;
            mTCPIPPortID = ConfigFile.returnTCPIPPort(name, "Transmit");

            mData = new byte[ConfigFile.MAX_BUFFER_SIZE];
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     Start the threads
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public void Start()
        {
            mRunThread = true;
            mTCPIPThread = new Thread(new ThreadStart(RunTCPIPThread));
            mTCPIPThread.Name = string.Format("Handle TCP/IP " + mName);
            mTCPIPThread.Start();
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     Stop the threads
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public void Stop()
        {
            mRunThread = false;
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public RNDOMessage GetRNDOMessage(UInt16 id)
        {
            RNDOMessage message = new RNDOMessage(id);

            for (int index = 0; index < mNDOMessages.Count; index++)
            {
                if (mNDOMessages[index].GetHeader() == message.GetHeader())
                {
                    return mNDOMessages[index];
                }
            }
            mNDOMessages.Add(message);

            return message;
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public RNDOMessage GetRNDOMessage(UInt16 id, UInt16 robot)
        {
            RNDOMessage message = new RNDOMessage(id, robot);

            for (int index = 0; index < mNDOMessages.Count; index++)
            {
                if (mNDOMessages[index].GetHeader() == message.GetHeader())
                {
                    return mNDOMessages[index];
                }
            }

            mNDOMessages.Add(message);

            return message;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      the thread that does the work
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RunTCPIPThread()
        {
            DateTime receiveTime = DateTime.Now;
            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse(ConfigFile.mRNDOIP), (int)mTCPIPPortID);

            while (true == mRunThread)
            {
                try
                {
                    Socket reader = new Socket(AddressFamily.InterNetwork,
                                                    SocketType.Stream,
                                                    ProtocolType.Tcp);

                    Debug.WriteLine("New socket:"+mName);

                    reader.Connect(localEndPoint);
                    reader.ReceiveTimeout = 10;
                    reader.DontFragment = true;

                    while ((true == mRunThread) && (true == reader.Connected))
                    {
                        try
                        {
                            mDataSize = (UInt32)reader.Receive(mData, (int)RNDOMessage.MAX_FULL_MESSAGE_SIZE, SocketFlags.None);
                            mReceivedTimeSpan = DateTime.Now.Subtract(receiveTime);

                            // If we have data update the counts and labels.
                            if((mDataSize < (int)ConfigFile.MAX_BUFFER_SIZE)&&(mDataSize>=RNDOMessage.MAX_FULL_MESSAGE_SIZE))
                            {
                                mReceivedCount++;
                                receiveTime = DateTime.Now;

                                for(int index=0;index<mDataSize;index+=RNDOMessage.MAX_FULL_MESSAGE_SIZE)
                                {
                                    RNDOMessage NDOMess = new RNDOMessage(mData,index);

                                    for (int indexInner = 0; indexInner < mNDOMessages.Count; indexInner++)
                                    {
                                        if(NDOMess.GetHeader() == mNDOMessages[indexInner].GetHeader())
                                        {
                                            mNDOMessages[indexInner].SetData(NDOMess.GetData());
                                        }
                                    }
                                }
                            }
                        }
                        catch(Exception e)
                        {
                            Debug.WriteLine("Inner Except:" + e.ToString());
                            Debug.WriteLine("Connection:" + reader.Connected.ToString());
                        }
                        Thread.Sleep(3);
                    }
                    reader.Close();
                }
                catch(Exception)
                {
                    Debug.WriteLine("Outer Except");
                }
                Thread.Sleep(3);
            }
        }
    }
}
