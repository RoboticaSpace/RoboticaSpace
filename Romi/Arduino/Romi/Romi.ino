//----------------------------------------------------------------------------
//
//  $Workfile: pedestal$
//
//  $Revision: X$
//
//  Project:    Ibbottinator
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
#include <Wire.h>
#include <Romi32U4.h>

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned long  uint32;
typedef char  sint8;
typedef short sint16;
typedef long  sint32;

//----------------------------------------------------------------------------
//  Class Constants
//----------------------------------------------------------------------------
const uint8 MAX_BUFFER = 50;
const uint8 MAX_PART_NUMBER = 16;
const uint8 MAX_SEND_BUFFER = 8;

const uint8 START_CHAR     = 0xA1;
const uint8 LOC_START      = 0;
const uint8 LOC_COMMAND    = 1;
const uint8 LOC_DATA       = 2;
const uint8 LOC_CHECK_BYTE = 1;

const uint8 INDEX_SET_BOTH_MOTORS   =  0;
const uint8 INDEX_SET_LEFT_MOTOR    =  1;
const uint8 INDEX_SET_RIGHT_MOTOR   =  2;
const uint8 INDEX_SET_BOTH_ENCODER  =  3;
const uint8 INDEX_SET_LEFT_Encoder  =  4;
const uint8 INDEX_SET_RIGHT_ENCODER =  5;
const uint8 INDEX_GET_LEFT_ENCODER  =  6;
const uint8 INDEX_GET_RIGHT_ENCODER =  7;
const uint8 INDEX_GET_SWITCHES      =  8;
const uint8 INDEX_GET_ANALOG        =  9;
const uint8 INDEX_PLAY_BUZZER       = 10;
const uint8 INDEX_MAX               = 11;

uint8 MESSAGE_SIZE[] = {   5,    5,    5,    3,    3,    3,    3,    3,    3,    4,    8};
uint8 MESSAGE_CMD[]  = {0x20, 0x21, 0x22, 0x30, 0x31, 0x32, 0x41, 0x42, 0x48, 0x49, 0x90};

const uint8 CMD_SET_BOTH_MOTORS   = 0x20;
const uint8 CMD_SET_LEFT_MOTOR    = 0x21;
const uint8 CMD_SET_RIGHT_MOTOR   = 0x22;
const uint8 CMD_SET_BOTH_ENCODER  = 0x30;
const uint8 CMD_SET_LEFT_ENCODER  = 0x31;
const uint8 CMD_SET_RIGHT_ENCODER = 0x32;
const uint8 CMD_LEFT_ENCODER      = 0x41;
const uint8 CMD_RIGHT_ENCODER     = 0x42;
const uint8 CMD_SWITCHES          = 0x48;
const uint8 CMD_GET_ANALOG        = 0x49;
const uint8 CMD_PLAY_BUZZER       = 0x90;

const uint8 RSP_LEFT_ENCODER      = 0x51;
const uint8 RSP_RIGHT_ENCODER     = 0x52;
const uint8 RSP_SWITCHES          = 0x58; 
const uint8 RSP_ANALOG            = 0x59;
const uint8 RSP_ACK               = 0xF0;

//----------------------------------------------------------------------------
//  Class Atributes
//----------------------------------------------------------------------------
Romi32U4Encoders mEncoders;
Romi32U4Buzzer mBuzzer;
Romi32U4Motors mMotors;
Romi32U4ButtonA mButtonA;
Romi32U4ButtonB mButtonB;
Romi32U4ButtonC mButtonC;

uint8 mPartNumber[MAX_PART_NUMBER];
uint8 mCommReceiveBuffer[MAX_BUFFER];
uint8 mCommReceiveLocation = 0;
uint8 mCommSendBuffer[MAX_SEND_BUFFER];
uint8 mCurrentCommand;
uint8 mCount = 0;

sint16 mLeftMotor = 0;
sint16 mRightMotor = 0;
sint32 mLeftEncoder = 0;
sint32 mRightEncoder = 0;

//----------------------------------------------------------------------------
//  Purpose:
//      Set up the Ardiuno
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void setup() 
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.begin(11500);           // start serial for output
  mEncoders.init();
  mBuzzer.playFrequency(6000, 250, 12);
}

//----------------------------------------------------------------------------
//  Purpose:
//      Main Loop
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void loop() 
{
  uint8 count = 0;
  // Trim the garbage from the start
  while ((START_CHAR != mCommReceiveBuffer[count]) && (count < mCommReceiveLocation))
  {
    count++;
  }

  if (count > 0)
  {
    RemoveDataForNextMessage(count, true);
  }

  // find if we are good or have garbage
  int nextMessage = FindNextMessage();

  if (true == DoWeHaveAGoodMessage())
  {
    mCurrentCommand = mCommReceiveBuffer[LOC_COMMAND];
    ProcessPackets();
    RemoveDataForNextMessage(ReturnSize(mCurrentCommand), true);
  }
  else
  {
    // Trim garbage if there is any
    if (nextMessage > 0)
    {
      RemoveDataForNextMessage(nextMessage, true);
    }
  }
  delay(1);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//      
//  Process Packets
//
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************

//----------------------------------------------------------------------------
//  Purpose:
//      Process the Packets to send comands places
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void ProcessPackets()
{
  bool sendAck = true;
  sint16 motorSpeed = 0;
  sint16 encoderValue = 0;
  bool errorValue = false;
  uint16 note = 0;
  uint16 theLength = 0;
  uint8  volumne = 0;
  
  switch(mCurrentCommand)
  {
    case(CMD_SET_BOTH_MOTORS):
      memcpy(&motorSpeed,&mCommReceiveBuffer[LOC_DATA],sizeof(sint16));

      Serial.print("Both M:");
      Serial.println(motorSpeed);
      
      mMotors.setSpeeds(motorSpeed, motorSpeed);
      break;
    case(CMD_SET_LEFT_MOTOR):
      memcpy(&motorSpeed,&mCommReceiveBuffer[LOC_DATA],sizeof(sint16));

      Serial.print("Left M:");
      Serial.println(motorSpeed);
      
      mMotors.setLeftSpeed(motorSpeed);
      break;
    case(CMD_SET_RIGHT_MOTOR):
      memcpy(&motorSpeed,&mCommReceiveBuffer[LOC_DATA],sizeof(sint16));

      Serial.print("Right M:");
      Serial.println(motorSpeed);
      
      mMotors.setRightSpeed(motorSpeed);
      break;
    case(CMD_SET_BOTH_ENCODER):
      encoderValue = mEncoders.getCountsAndResetLeft();
      encoderValue = mEncoders.getCountsAndResetRight();
      break;
    case(CMD_SET_LEFT_ENCODER):
      encoderValue = mEncoders.getCountsAndResetLeft();
      break;
    case(CMD_SET_RIGHT_ENCODER):
      encoderValue = mEncoders.getCountsAndResetRight();
      break;
    case(CMD_LEFT_ENCODER):
      encoderValue = mEncoders.getCountsLeft();
      errorValue = mEncoders.checkErrorLeft();

      Serial.print("Left E:");
      Serial.print(encoderValue);
      Serial.print(" Err:");
      Serial.println(errorValue);
      
      mCommSendBuffer[LOC_COMMAND] = RSP_LEFT_ENCODER;
      memcpy(&mCommSendBuffer[LOC_DATA],&encoderValue,sizeof(sint16));
      if(true == errorValue)
      {
        mCommSendBuffer[LOC_DATA+2] = 0x01;
      }
      else
      {
        mCommSendBuffer[LOC_DATA+2] = 0x00;
      }
      sendAck = false;
      break;
    case(CMD_RIGHT_ENCODER):
      encoderValue = mEncoders.getCountsRight();
      errorValue = mEncoders.checkErrorRight();

      Serial.print("Right E:");
      Serial.print(encoderValue);
      Serial.print(" Err:");
      Serial.println(errorValue);
            
      mCommSendBuffer[LOC_COMMAND] = RSP_LEFT_ENCODER;
      memcpy(&mCommSendBuffer[LOC_DATA],&encoderValue,sizeof(sint16));
      if(true == errorValue)
      {
        mCommSendBuffer[LOC_DATA+2] = 0x01;
      }
      else
      {
        mCommSendBuffer[LOC_DATA+2] = 0x00;
      }
      sendAck = false;
      break;
    case(CMD_SWITCHES):
      break;
    case(CMD_GET_ANALOG):
      break;
    case(CMD_PLAY_BUZZER):
      memcpy(&note,&mCommReceiveBuffer[LOC_DATA],sizeof(sint16));
      memcpy(&theLength,&mCommReceiveBuffer[LOC_DATA+2],sizeof(sint16));
      volumne = mCommReceiveBuffer[LOC_DATA+4];

      Serial.print("Note:");
      Serial.print(note);
      Serial.print(" Len:");
      Serial.print(theLength);
      Serial.print(" Vol:");
      Serial.println(volumne);
      
      mBuzzer.playFrequency(note,theLength,volumne);
      break;
  }
  if(true == sendAck)
  {
    mCommSendBuffer[LOC_COMMAND] = RSP_ACK;
    mCommSendBuffer[LOC_DATA] = mCurrentCommand;
  }
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//      
//  I2C Comunication
//
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************

//----------------------------------------------------------------------------
//  Purpose:
//      Receive event from i2c
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void receiveEvent(int howMany) 
{
  while ((0 < Wire.available())&&(mCommReceiveLocation<MAX_BUFFER)) 
  {
    mCommReceiveBuffer[mCommReceiveLocation] = Wire.read(); 
    mCommReceiveLocation++;
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//      Send event to i2c
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void requestEvent() 
{
  uint8 theLength = 4;
  if(RSP_ACK != mCommSendBuffer[LOC_COMMAND])
  {
    theLength = 6;
  }
  mCommSendBuffer[LOC_START] = START_CHAR;
 
  mCommSendBuffer[theLength-LOC_CHECK_BYTE] = CalcCheckByte(mCommSendBuffer, 0, theLength-LOC_CHECK_BYTE);

  Wire.write(mCommSendBuffer,theLength); 
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//      
//  Comunication Utilities
//
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************

//----------------------------------------------------------------------------
//  Purpose:
//      Return if the packet is well formed
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
bool DoWeHaveAGoodMessage()
{
  bool returnValue = false;
  uint8 theLength = ReturnSize(mCommReceiveBuffer[LOC_COMMAND]);

  if ((mCommReceiveLocation >= theLength) && (mCommReceiveLocation != 0) && (theLength != 0))
  {
    //Is the preamble where it should be
    if (mCommReceiveBuffer[LOC_START] == START_CHAR)
    {
      uint8 checkByte = CalcCheckByte(mCommReceiveBuffer, LOC_START, theLength - LOC_CHECK_BYTE);
#ifdef DEBUG
      Serial.print("Len:");
      Serial.print(theLength);
      Serial.print(" CMD:");
      Serial.print(mCommReceiveBuffer[LOC_COMMAND]);
      Serial.print(" Calc:");
      Serial.print(checkByte);
      Serial.print(" Frm:");
      Serial.println(mCommReceiveBuffer[theLength - LOC_CHECK_BYTE]);
#endif      
      if (checkByte == mCommReceiveBuffer[theLength - LOC_CHECK_BYTE])
      {
        returnValue = true;
      }
      else
      {
        RemoveDataForNextMessage(theLength, true);
      }
    }
    else
    {
      RemoveDataForNextMessage(theLength, true);
    }
  }
  return returnValue;
}

//----------------------------------------------------------------------------
//  Purpose:
//      Calc a check byte from the data
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
uint8 CalcCheckByte(uint8* data, uint8 start, uint8 number)
{
  uint8 checkByte = 0xFF;
  for (uint8 index = 0; index < number; index++)
  {
    checkByte ^= data[start + index];
  }
  return checkByte;
}

//----------------------------------------------------------------------------
//  Purpose:
//      Find the next message after the first one
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
uint8 FindNextMessage()
{
  uint8 nextMessIndex = 0;
  bool found = false;
  uint8 theLength = ReturnSize(mCommReceiveBuffer[LOC_COMMAND]);

  //Is the preamble where it should be
  if ((mCommReceiveBuffer[LOC_START] == START_CHAR)&&(mCommReceiveLocation>2))
  {
    theLength = ReturnSize(mCommReceiveBuffer[LOC_COMMAND]);
    //From the end of the message search the rest of what we have gotten
    //for another preamble.
    for (nextMessIndex = 1; nextMessIndex < mCommReceiveLocation; nextMessIndex++)
    {
      if (mCommReceiveBuffer[nextMessIndex] == START_CHAR)
      {
        //If we found one stop
        found = true;
        break;
      }
    }
  }

  //If we found one the nextMessIndex should be good.
  //If not then set it to 0 and return 0.
  if (found == false)
  {
    nextMessIndex = 0;
  }
  return nextMessIndex;
}

//----------------------------------------------------------------------------
//  Purpose:
//      Trim the front of the buffer
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
void RemoveDataForNextMessage(uint8 offset, bool isBad)
{
  int index;

  //Move the first 'offset' number of bytes forward.
  for (index = 0; index < mCommReceiveLocation - offset; index++)  // JSF162 JSF213 Exception
  {
    mCommReceiveBuffer[index] = mCommReceiveBuffer[offset + index];
  }

  //if we have been asked to remove more bytes than we have set the number
  //of bytes to 0.
  if (offset > mCommReceiveLocation)
  {
    mCommReceiveLocation = 0;
  }
  else
  {
    //If not then reduce the number of bytes we have by the offset.
    mCommReceiveLocation -= offset;
  }

  //Move the rest of the message down to right after the 'offset' bytes.
  // Process the rest of the buffer
  for (; index < MAX_BUFFER; index++)   // JSF200 JSF162 Exception
  {
    //If we are under the MAX packet size then move the data.
    if ((offset + index) < (mCommReceiveLocation))
    {
      mCommReceiveBuffer[index] = mCommReceiveBuffer[offset + index];
    }
    else
    {
      //If we are over the MAX packet size then clear out the bytes.
      mCommReceiveBuffer[index] = 0;
    }
  }
}

//----------------------------------------------------------------------------
//  Purpose:
//      Return Size
//
//  Notes:
//      None
//
//----------------------------------------------------------------------------
uint8 ReturnSize(uint8 theCommand)
{
  if(0 == theCommand)
  {
    return 1;
  }
  
  for(int i=0;i< INDEX_MAX;i++)
  {
    if(MESSAGE_CMD[i] == theCommand)
    {
      return MESSAGE_SIZE[i];
    }
  }
  return 1;
}

