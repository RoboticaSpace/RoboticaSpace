//----------------------------------------------------------------------------
//
//  $Workfile: RNDOTransmitClient.cs$
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
using RNDO;

namespace RNDOClientInterface
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: RNDOTransmitClient
    // 
    // Purpose:
    //      This class handles sending messages to the server
    //
    //----------------------------------------------------------------------------
    public class RNDOTransmitClient
    {
        public string mName;
        bool mRunThread = true;
        Thread mTCPIPThread;
        UInt32 mTCPIPPortID;
        List<RNDOMessage> mRNDOMessages = new List<RNDOMessage>();
        public UInt32 mTransmitCount = 0;
        public TimeSpan mTransmitTimeSpan = new TimeSpan();

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public RNDOTransmitClient(string name)
        {
            mName = name;
            mTCPIPPortID = ConfigFile.returnTCPIPPort(name, "Receive");
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

            for (int index = 0; index < mRNDOMessages.Count; index++)
            {
                if (mRNDOMessages[index].GetHeader() == message.GetHeader())
                {
                    return mRNDOMessages[index];
                }
            }

            mRNDOMessages.Add(message);

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

            for (int index = 0; index < mRNDOMessages.Count; index++)
            {
                if (mRNDOMessages[index].GetHeader() == message.GetHeader())
                {
                    return mRNDOMessages[index];
                }
            }

            mRNDOMessages.Add(message);

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
            DateTime transmitTime = DateTime.Now;
            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Parse(ConfigFile.mRNDOIP), (int)mTCPIPPortID);

            while (true == mRunThread)
            {
                try
                {
                    Socket sender = new Socket(AddressFamily.InterNetwork,
                                                    SocketType.Stream,
                                                    ProtocolType.Tcp);

                    sender.Connect(localEndPoint);
                    sender.ReceiveTimeout = 6;
                    sender.SendTimeout = 6;

                    while ((true == mRunThread)&&(true == sender.Connected))
                    {
                        try
                        {
                            UInt32 dataSize = (UInt32)((RNDOMessage.MAX_FULL_MESSAGE_SIZE * mRNDOMessages.Count));
                            byte[] data = new byte[dataSize];

                            for(int index=0;index<mRNDOMessages.Count;index++)
                            {
                                mRNDOMessages[index].GetByteArray(data, (index * RNDOMessage.MAX_FULL_MESSAGE_SIZE));
                            }

                            sender.Send(data);
                            mTransmitTimeSpan = DateTime.Now.Subtract(transmitTime);
                            transmitTime = DateTime.Now;
                            mTransmitCount++;
                        }
                        catch(Exception)
                        {
                        }
                        Thread.Sleep(9);
                    }
                }
                catch(Exception)
                {
                }
                Thread.Sleep(9);
            }
        }
    }
}
