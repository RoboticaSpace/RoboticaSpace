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
#include "mraa.hpp"
#include <iostream>
#include <unistd.h>
#include "SFE_LSM9DS0.h"
#include "RNDOTransmitClient.h"
#include "Kalman.h"

using namespace std;

int main()
{
	LSM9DS0 *imu;
	imu = new LSM9DS0(0x6B, 0x1D);
	uint16_t imuResult = imu->begin();
	cout<<hex<<"Chip ID: 0x"<<imuResult<<dec<<" (should be 0x49d4)"<<endl;
	UInt32 count = 0;
	double gyroZAve = 0.0;
    KalmanFilter* theFilter = new KalmanFilter(.1, 1.015, .001);
    double kalmanGyroZ = 0.0;
    double deadZone = 0.0;

	RNDOTransmitClient sender("Gyro Test");
	RNDOMessage* gyroZDebug = sender.GetRNDOMessage(0x30,0x1);

	sender.Start();

	// loop forever toggling the on board LED every second
	for (;;)
	{
	    imu->readGyro();
	    double gyroZ = imu->calcGyro(imu->gz);

	    gyroZAve = (31*(gyroZAve/32))+(gyroZ/32);
	    kalmanGyroZ = theFilter->getAngle(kalmanGyroZ, gyroZ,.001);
	    //cout<<"z:"<<kalmanGyroZ<<" "<<gyroZ<<endl;

	    deadZone = kalmanGyroZ;
	    if((deadZone > -4)&&(deadZone<4))
	    {
	    	deadZone = 0.0;
	    }

	    gyroZDebug->SetDoublebitBlock(gyroZ,0,1024);
	    gyroZDebug->SetDoublebitBlock(gyroZAve,1,1024);
	    gyroZDebug->SetDoublebitBlock(kalmanGyroZ,2,1024);
	    gyroZDebug->SetDoublebitBlock(deadZone,3,1024);
	    gyroZDebug->Set32BitBlock(count,4);
	    count++;
	    usleep(250);
	}

	sender.Stop();

	return mraa::SUCCESS;
}
