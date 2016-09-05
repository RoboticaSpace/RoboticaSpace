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
#include <stdio.h>
#include <iostream>
#include <wiringPi.h>
#include "RNDOTransmitClient.h"
#include "LinuxUtils.h"

using namespace std;

const int CHAN_A = 8;
const int CHAN_B = 9;
const int PWM_PIN = 1;
const int DIR_1 = 28;
const int DIR_2 = 29;

const int MAX_TICKS = 500;
const int MIN_TICKS = -500;
const int SPEED = 2;

const double KI = .25;
const double KP = .75;
const double KD = .25;

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
    int speed = 0;
    int requestedTicks = 0;
    int error = 0;
    int acumError = 0;
    int deriv = 0;
    int lastError = 0;
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
    pwmWrite(PWM_PIN,speed);

    // loop forever toggling the on board LED every second
    while (true)
    {
        int tempSpeed = speed;

        if (tempSpeed < 0)
        {
            digitalWrite(DIR_1, 1);
            digitalWrite(DIR_2, 0);
            tempSpeed *= -1;
        }
        else
        {
            digitalWrite(DIR_1, 0);
            digitalWrite(DIR_2, 1);
        }

        pwmWrite(PWM_PIN,tempSpeed);

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
                requestedTicks += SPEED;
            }

            if(101 == ch)
            {
                requestedTicks -= SPEED;
            }

            if(requestedTicks > MAX_TICKS)
            {
                requestedTicks = MAX_TICKS;
            }

            if(requestedTicks < MIN_TICKS)
            {
                requestedTicks = MIN_TICKS;
            }
        }

        error = requestedTicks - mCount;
        acumError += error;
        deriv = error - lastError;
        lastError = error;

        speed = KP * (double)error + KI * (double)acumError + KD * (double)deriv;

        if (speed > 1024)
        {
            speed = 1024;
        }

        if (speed < -1024)
        {
            speed = -1024;
        }

        gyroZDebug->Set16BitBlock((unsigned short)requestedTicks, 0);
        gyroZDebug->Set16BitBlock((unsigned short)mCount, 1);
        gyroZDebug->Set16BitBlock((unsigned short)error, 2);
        gyroZDebug->Set16BitBlock((unsigned short)acumError, 3);
        gyroZDebug->Set16BitBlock((unsigned short)deriv, 4);
        gyroZDebug->Set16BitBlock((unsigned short)speed, 5);

        cout<<"Count:"<<mCount<<" "<< requestedTicks <<" "<<speed<<endl;
        mCount = 0;

        usleep(100000);
    }

    pwmWrite(PWM_PIN,0);
    sender.Stop();

    return 0;
}
