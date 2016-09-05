//----------------------------------------------------------------------------
//
//  $Workfile: RNDOTransmitClent.h$
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
#ifndef RNDOTRANSMITCLIENT_H_
#define RNDOTRANSMITCLIENT_H_

#include <string>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "Common.h"
#include "RNDOMessage.h"

using namespace std;

class RNDOTransmitClient
{
	string mName;
	UInt32 mTCPIPPortID;
	bool mRunThread;
	pthread_t mTCPIPThread;
	vector<RNDOMessage*> mRNDOMessages;
/*
	List<RNDOMessage> mRNDOMessages = new List<RNDOMessage>();
	public
	public TimeSpan mTransmitTimeSpan = new TimeSpan();
*/
	void* RunTCPIPThread(void);
	static void*  callMemberFunction(void *arg) { return ((RNDOTransmitClient*)arg)->RunTCPIPThread(); }

public:
	UInt32 mTransmitCount;

	RNDOMessage* GetRNDOMessage(UInt16 id);
	RNDOMessage* GetRNDOMessage(UInt16 id, UInt16 robot);

	RNDOTransmitClient(string name);
	void Start(void);
	void Stop(void);
};

#endif /* RNDOTRANSMITCLIENT_H_ */
