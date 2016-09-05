//----------------------------------------------------------------------------
//
//  $Workfile: HandleIn.cs$
//
//  $Revision: X$
//
//  Project:    Robot Network Data Objects
//
//                            Copyright (c) 2016
//                              James A Wright
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
using System.Threading.Tasks;
using System.Threading;
using System.Net.Sockets;
using System.Net;

using RNDO;

namespace RNDOServer
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: HandleIn
    // 
    // Purpose:
    //      This class receves messages from the tools
    //
    //----------------------------------------------------------------------------
    class HandleIn
    {
        string mName;
        Thread mAFDXThread;
        bool mRunThread = true;
        List<RNDOMessage> mMessages = new List<RNDOMessage>();
        List<Socket> mClientSockets = new List<Socket>();
        UInt32 mTCPIPReceivePortID;
        public TimeSpan mReceiveTimeSpan = new TimeSpan();
        byte[] mData;
        UInt32 mDataSize;

        //--------------------------------------------------------------------
        // Purpose:
        //     Start the threads
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public HandleIn(string name, List<RNDOMessage> messages)
        {
            mMessages = messages;
            mName = name;
            mData = new byte[RNDOMessage.MAX_FULL_MESSAGE_SIZE*4];
            mTCPIPReceivePortID = ConfigFile.returnTCPIPPort(name, "Receive");
        }

        public int CountOfConnections()
        {
            return mClientSockets.Count;
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     Start the threads
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public void start()
        {
            mRunThread = true;
            mAFDXThread = new Thread(new ThreadStart(RunAFDXThread));
            mAFDXThread.Name = string.Format("Handle AFDX " + mName);
            mAFDXThread.IsBackground = true;
            mAFDXThread.Start();
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     Stop the threads
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public void stop()
        {
            mRunThread = false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      the thread that does the work
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RunAFDXThread()
        {
            DateTime oldReceiveTime = DateTime.Now;
            DateTime oldTime = DateTime.Now;
            // Open up the receive TCP/IP port.
            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, (int)mTCPIPReceivePortID);

            Socket listener = new Socket(AddressFamily.InterNetwork,
                                            SocketType.Stream,
                                            ProtocolType.Tcp);
            listener.Bind(localEndPoint);
            listener.Listen(20);
            listener.ReceiveTimeout = 9000;

            while (true == mRunThread)
            {
                try
                {
                    Socket client = listener.Accept();
                    oldTime = DateTime.Now;

                    while ((true == mRunThread) && (true == client.Connected))
                    {
                        try
                        {
                            mDataSize = (UInt32)client.Receive(mData);
                            mReceiveTimeSpan = DateTime.Now.Subtract(oldReceiveTime);

                            if (0 != mDataSize)
                            {
                                oldReceiveTime = DateTime.Now;

                                for (int index = 0; index < mDataSize; index += RNDOMessage.MAX_FULL_MESSAGE_SIZE)
                                {
                                    RNDOMessage NDOMess = new RNDOMessage(mData, index);
                                    bool found = false;

                                    for (int indexInner = 0; indexInner < mMessages.Count; indexInner++)
                                    {
                                        if (NDOMess.GetHeader() == mMessages[indexInner].GetHeader())
                                        {
                                            mMessages[indexInner].SetData(NDOMess.GetData());
                                            found = true;
                                        }
                                    }

                                    if(false == found)
                                    {
                                        mMessages.Add(NDOMess);
                                    }

                                }
                            }
                            else
                            {
                                if (oldTime.AddSeconds(3) < DateTime.Now)
                                {
                                    break;
                                }
                            }
                            Thread.Sleep(3);
                        }
                        catch (Exception)
                        {
                        }
                        Thread.Sleep(3);
                    }
                    client.Close();
                }
                catch (Exception)
                {
                }
                Thread.Sleep(3);
            }

            for (int index = 0; index < mClientSockets.Count; index++)
            {
                try
                {
                    mClientSockets[index].Close();
                }
                catch (Exception)
                {
                }
            }
        }
    }
}
