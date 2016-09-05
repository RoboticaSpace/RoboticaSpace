//----------------------------------------------------------------------------
//
//  $Workfile: HandleOut.cs$
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
using System.Threading;
using System.Net.Sockets;
using System.Net;
using System.Diagnostics;
using RNDO;

namespace RNDOServer
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: HandleOut
    // 
    // Purpose:
    //      This class handles sending messages to the tools
    //
    //----------------------------------------------------------------------------
    class HandleOut
    {
        const int TIMEOUT_MS = 250;

        string mName;
        bool mRunThread = true;
        UInt32 mTCPIPPortID;
        List<RNDOMessage> mMessages = new List<RNDOMessage>();
        byte[] mData;
        UInt32 mDataSize;
        public List<Socket> mClientSockets = new List<Socket>();
        Thread mAFDXThread;
        Thread mTCPIPThread;
        public TimeSpan mReceiveTimeSpan = new TimeSpan();
        public UInt32 mMaxMils = 0;
        public UInt32 mTotalMils = 0;
        public UInt32 mNumMils = 0;

        //--------------------------------------------------------------------
        // Purpose:
        //     Constructor
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public HandleOut(string name, List<RNDOMessage> messages)
        {
            mName = name;
            mMessages = messages;

            mData = new byte[ConfigFile.MAX_BUFFER_SIZE];
            mDataSize = 0;

            mTCPIPPortID = ConfigFile.returnTCPIPPort(name, "Transmit");
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
            mTCPIPThread = new Thread(new ThreadStart(RunTCPIPThread));
            mTCPIPThread.Name = string.Format("Handle TCP/IP " + mName);
            mTCPIPThread.IsBackground = true;
            mTCPIPThread.Start();
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
            DateTime oldTime = DateTime.Now;
            int index;

            while (true == mRunThread)
            {
                mDataSize = (UInt32)(mMessages.Count * RNDOMessage.MAX_FULL_MESSAGE_SIZE);
                mData = new byte[mDataSize];

                for (index = 0; index < mMessages.Count; index++)
                {
                    mMessages[index].GetByteArray(mData, (index * RNDOMessage.MAX_FULL_MESSAGE_SIZE));
                }

                if (mDataSize > 0)
                {
                    mReceiveTimeSpan = DateTime.Now.Subtract(oldTime);
                    UInt32 mils = (UInt32)mReceiveTimeSpan.TotalMilliseconds;

                    if (mils > mMaxMils)
                    {
                        mMaxMils = mils;
                    }
                    mTotalMils += mils;
                    mNumMils++;

                    oldTime = DateTime.Now;

                    for (index = 0; index < mClientSockets.Count; index++)
                    {
                        try
                        {
                            mClientSockets[index].Send(mData, (int)mDataSize, SocketFlags.None);
                            if (false == mClientSockets[index].Connected)
                            {
                                mClientSockets.RemoveAt(index);
                                break;
                            }
                        }
                        catch (Exception)
                        {
                            mClientSockets.RemoveAt(index);
                            break;
                        }
                    }
                }
                else
                {
                    if (mReceiveTimeSpan.TotalSeconds > 100)
                    {
                        mClientSockets.Clear();
                    }
                }

                Thread.Sleep(3);
            }

            for (index = 0; index < mClientSockets.Count; index++)
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
            IPEndPoint localEndPoint = new IPEndPoint(IPAddress.Any, (int)mTCPIPPortID);

            Socket listener = new Socket(AddressFamily.InterNetwork,
                                            SocketType.Stream,
                                            ProtocolType.Tcp);

            listener.Bind(localEndPoint);
            listener.Listen(20);
            listener.ReceiveTimeout = 50;

            while (true == mRunThread)
            {
                try
                {
                    Socket client = listener.Accept();
                    mClientSockets.Add(client);
                }
                catch (Exception)
                {
                }
                Thread.Sleep(2);
            }

            listener.Close();
        }
    }
}
