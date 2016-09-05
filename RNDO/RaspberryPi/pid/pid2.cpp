//----------------------------------------------------------------------------
//
//  $Workfile: GyroRNDOSender$
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
#include <wiringPi.h>
#include <iostream>
#include <unistd.h>
#include "RNDOTransmitClient.h"

using namespace std;

int main()
{
	int count=0;

	if(-1 == wiringPiSetup())
	{
		return -1;
	}


	RNDOTransmitClient sender("PID Test");
	RNDOMessage* gyroZDebug = sender.GetRNDOMessage(0x21,0x1);

	sender.Start();

	// loop forever toggling the on board LED every second
	for (;;)
	{
	    cout<<"z:"<<count<<endl;

	    gyroZDebug->Set32BitBlock(count,4);
	    count++;
	    usleep(250);
	}

	sender.Stop();

	return 0;
}
