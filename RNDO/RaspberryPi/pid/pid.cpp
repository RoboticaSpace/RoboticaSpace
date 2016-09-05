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
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <iostream>
#include <unistd.h>
#include <wiringPi.h>
#include <termios.h>
#include <fcntl.h>
#include "RNDOTransmitClient.h"
#include "LinuxUtils.h"

using namespace std;

const int CHAN_A = 8;
const int CHAN_B = 9;
const int PWM_PIN = 1;
const int DIR_1 = 28;
const int DIR_2 = 29;

const int SPEED = 10;

long mCount = 0;
int mLastA = 0;
int mLastB = 0;

//CHAN_A   0011001100110011001100
//CHAN_B   0110011001100110011001

void intChannelA(void)
{
  int temp = digitalRead(CHAN_A);
 
  if(1 == temp)
  {
    if(1 == mLastB)
    { 
      mCount++;
    }
  }
  else
  {
    if(0 == mLastB)
    {
      mCount++;
    }
  }

  mLastA = temp;
}

void intChannelB(void)
{
  int temp = digitalRead(CHAN_B);

  if(0 == temp)
  {
    if(0 == mLastA)
    {
      mCount--;
    }
  }
  else
  {
    if(1 == mLastA)
    {
      mCount--;
    }
  }

  mLastB = temp;
}


int main()
{
	int count=0;
        int speed = 0;
	int direction = -1;
	RNDOTransmitClient sender("PID Test");
	RNDOMessage* gyroZDebug = sender.GetRNDOMessage(0x21,0x01);

	sender.Start();

	if(-1 == wiringPiSetup())
	{
		return -1;
	}

	pinMode(PWM_PIN, PWM_OUTPUT);
	pinMode(CHAN_A, INPUT);
	pinMode(CHAN_B, INPUT);
	pinMode(DIR_1, OUTPUT);
	pinMode(DIR_2, OUTPUT);

        wiringPiISR(CHAN_A, INT_EDGE_BOTH, &intChannelA);
        wiringPiISR(CHAN_B, INT_EDGE_BOTH, &intChannelB);

	digitalWrite(DIR_1,1);
	digitalWrite(DIR_2,0);
	pwmWrite(PWM_PIN,100);

	// loop forever toggling the on board LED every second
	for (;;)
	{
           pwmWrite(PWM_PIN,speed);

           if(-1 == direction)
           {
             digitalWrite(DIR_1,1);
             digitalWrite(DIR_2,0);
           }
           else
           {
             digitalWrite(DIR_1,0);
             digitalWrite(DIR_2,1);
           }

    

            int ch = kbhit();
	    if(1 == ch)
            {
              ch = getchar();
	      cout<<"z:"<<mCount<<" "<<ch<<endl;
	      if(119 == ch)
              {
                break;
              }
 
              if(113 == ch)
              {
                 speed += SPEED*direction;
              }

              if(101 == ch)
              {
                 speed -= SPEED*direction;
              }

              if(speed > 1024)
              {
               speed = 1024;
              }
              if(speed < 0)
              {
                speed = 0;
                if(-1 == direction)
                {
                  direction = 1;
                }
                else
                {
                  direction = -1;
                }
              }
            }
	    count++;
            gyroZDebug->Set16BitBlock((unsigned short)mCount,0);
	    gyroZDebug->Set16BitBlock((unsigned short)speed*direction,1);
	    gyroZDebug->Set32BitBlock(count,4);

            cout<<"Count:"<<mCount<<endl;
	    mCount = 0;

	    usleep(100000);
	}

        pwmWrite(PWM_PIN,0);
	sender.Stop();

	return 0;
}
