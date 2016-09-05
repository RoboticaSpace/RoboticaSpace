//----------------------------------------------------------------------------
//
//  $Workfile: RNDOTransmitClent.cpp$
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
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include "RNDOTransmitClient.h"

using namespace std;

RNDOTransmitClient::RNDOTransmitClient(string name)
{
	mName = name;
//	mTCPIPPortID = ConfigFile.returnTCPIPPort(name, "Receive");
	mTCPIPPortID = 4543;
	mTransmitCount = 0;
}

//--------------------------------------------------------------------
// Purpose:
//     Start the threads
//
// Notes:
//     None.
//--------------------------------------------------------------------
void RNDOTransmitClient::Start(void)
{
	mRunThread = true;
	pthread_create(&mTCPIPThread, 0, &RNDOTransmitClient::callMemberFunction, this);
}

//--------------------------------------------------------------------
// Purpose:
//     Stop the threads
//
// Notes:
//     None.
//--------------------------------------------------------------------
void RNDOTransmitClient::Stop(void)
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
RNDOMessage* RNDOTransmitClient::GetRNDOMessage(UInt16 id, UInt16 robot)
{
	RNDOMessage* message = new RNDOMessage(id,robot);

	for (int index = 0; index < (int) mRNDOMessages.size(); index++)
	{
		if (mRNDOMessages[index]->GetHeader() == message->GetHeader())
		{
			return mRNDOMessages[index];
		}
	}

	mRNDOMessages.push_back(message);
	return message;
}

//--------------------------------------------------------------------
// Purpose:
//
//
// Notes:
//     None.
//--------------------------------------------------------------------
RNDOMessage* RNDOTransmitClient::GetRNDOMessage(UInt16 id)
{
	return GetRNDOMessage(id,0);
}

//----------------------------------------------------------------------------
//  Purpose:
//      the thread that does the work
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void* RNDOTransmitClient::RunTCPIPThread(void)
{
    int sock;
    struct sockaddr_in server;

    server.sin_addr.s_addr = inet_addr("192.168.143.193");
    server.sin_family = AF_INET;
    server.sin_port = htons( 4543 );

	while(true == mRunThread)
	{
		try
		{
		    sock = socket(AF_INET , SOCK_STREAM , 0);
		    if (sock == -1)
		    {
		        cout<<"Could not create socket"<<endl;
		    }
		    cout<<"Socket created"<<endl;

		    //Connect to remote server
		    int connected = connect(sock , (struct sockaddr *)&server , sizeof(server));

		    cout<<"Connected"<<connected<<endl;

			while ((true == mRunThread)&&(connected>=0))
			{
				try
				{
					UInt32 dataSize = (UInt32)((RNDOMessage::MAX_FULL_MESSAGE_SIZE * mRNDOMessages.size()));
					byte data[dataSize];

					for(int index=0;index<(int)mRNDOMessages.size();index++)
					{
						mRNDOMessages[index]->GetByteArray(data, (index * RNDOMessage::MAX_FULL_MESSAGE_SIZE));
					}

					send(sock , data , dataSize , 0);
					/*
					cout<<"send:"<<dataSize<<":";

					for(int index=0;index<dataSize;index++)
					{
						cout<<(int)data[index]<<" ";
					}
				    cout<<endl;
					*/
					//mTransmitTimeSpan = DateTime.Now.Subtract(transmitTime);
					//transmitTime = DateTime.Now;
					mTransmitCount++;
				}
				catch(...)
				{
				}
				usleep(8000);
			}
		}
		catch(...)
		{
		}
	}
	return NULL;
}

