//----------------------------------------------------------------------------
//
//  $Workfile: XBox360Controller.cs$
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
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace XBox360Controller
{
    public struct XBox360ControllerStruct
    {
        public short controlPackage;
        public short buttons;
        public short leftX;
        public short leftY;
        public short rightX;
        public short rightY;
        public short leftTrigger;
        public short rightTrigger;
    };

    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: XBox360Controller
    // 
    // Purpose:
    //      This gets input from the xbos controller and packages into a struct
    //
    //----------------------------------------------------------------------------
    public class XBox360Controller
    {
        const short BUTTON_MASK_X = 0x0001;
        const short BUTTON_MASK_Y = 0x0002;
        const short BUTTON_MASK_A = 0x0004;
        const short BUTTON_MASK_B = 0x0008;
        const short BUTTON_MASK_LS = 0x0010;
        const short BUTTON_MASK_RS = 0x0020;
        const short BUTTON_MASK_START = 0x0040;
        const short BUTTON_MASK_BACK = 0x0080;
        const short BUTTON_MASK_UP = 0x0100;
        const short BUTTON_MASK_DOWN = 0x0200;
        const short BUTTON_MASK_LEFT = 0x0400;
        const short BUTTON_MASK_RIGHT = 0x0800;
        const short BUTTON_MASK_BIG = 0x1000;

        const short BUTTON_OFF_X = 0;
        const short BUTTON_OFF_Y = 1;
        const short BUTTON_OFF_A = 2;
        const short BUTTON_OFF_B = 3;
        const short BUTTON_OFF_LS = 4;
        const short BUTTON_OFF_RS = 5;
        const short BUTTON_OFF_START = 6;
        const short BUTTON_OFF_BACK = 7;
        const short BUTTON_OFF_UP = 8;
        const short BUTTON_OFF_DOWN = 9;
        const short BUTTON_OFF_LEFT = 10;
        const short BUTTON_OFF_RIGHT = 11;
        const short BUTTON_OFF_BIG = 12;

        const int MAX_BYTE_ARRAY = 16;

        XBox360ControllerStruct mCurrentState;
        PlayerIndex mCurrentController;

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public XBox360Controller(int whichController)
        {
            switch(whichController)
            {
                case 0:
                    mCurrentController = PlayerIndex.One;
                    break;
                case 1:
                    mCurrentController = PlayerIndex.Two;
                    break;
                case 2:
                    mCurrentController = PlayerIndex.Three;
                    break;
                case 3:
                    mCurrentController = PlayerIndex.Four;
                    break;
            }
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public XBox360Controller()
        {
            mCurrentController = PlayerIndex.One;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Reads the hardware
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public void readController()
        {
            mCurrentState.buttons = 0;
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.X == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_X) & BUTTON_MASK_X);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.Y == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_Y) & BUTTON_MASK_Y);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.A == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_A) & BUTTON_MASK_A);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.B == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_B) & BUTTON_MASK_B);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.LeftShoulder == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_LS) & BUTTON_MASK_LS);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.RightShoulder == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_RS) & BUTTON_MASK_RS);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.Start == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_START) & BUTTON_MASK_START);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.Back == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_BACK) & BUTTON_MASK_BACK);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).DPad.Up == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_UP) & BUTTON_MASK_UP);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).DPad.Down == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_DOWN) & BUTTON_MASK_DOWN);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).DPad.Left == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_LEFT) & BUTTON_MASK_LEFT);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).DPad.Right == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_RIGHT) & BUTTON_MASK_RIGHT);
            mCurrentState.buttons |= (short)(((short)((GamePad.GetState(mCurrentController).Buttons.BigButton == ButtonState.Pressed) ? (byte)1 : (byte)0) << BUTTON_OFF_BIG) & BUTTON_MASK_BIG);

            mCurrentState.leftX = (short)(GamePad.GetState(mCurrentController).ThumbSticks.Left.X * 100);
            mCurrentState.leftY = (short)(GamePad.GetState(mCurrentController).ThumbSticks.Left.Y * 100);
            mCurrentState.rightX = (short)(GamePad.GetState(mCurrentController).ThumbSticks.Right.X * 100);
            mCurrentState.rightY = (short)(GamePad.GetState(mCurrentController).ThumbSticks.Right.Y * 100);

            mCurrentState.leftTrigger = (short)(GamePad.GetState(mCurrentController).Triggers.Left * 100);
            mCurrentState.rightTrigger = (short)(GamePad.GetState(mCurrentController).Triggers.Right * 100);
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Return the sctruct that was read
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public XBox360ControllerStruct getControllerStruct()
        {
            return mCurrentState;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Pack everything into a byte array
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public byte[] getControllerBytes()
        {
            byte[] newBytes = new byte[getControllerBytesLength()];

            newBytes[0] = (byte)((mCurrentState.controlPackage >> 8) & 0xFF);
            newBytes[1] = (byte)((mCurrentState.controlPackage) & 0xFF);
            newBytes[2] = (byte)((mCurrentState.buttons >> 8) & 0xFF);
            newBytes[3] = (byte)((mCurrentState.buttons) & 0xFF);
            newBytes[4] = (byte)((mCurrentState.leftX >> 8) & 0xFF);
            newBytes[5] = (byte)((mCurrentState.leftX) & 0xFF);
            newBytes[6] = (byte)((mCurrentState.leftY >> 8) & 0xFF);
            newBytes[7] = (byte)((mCurrentState.leftY) & 0xFF);
            newBytes[8] = (byte)((mCurrentState.rightX >> 8) & 0xFF);
            newBytes[9] = (byte)((mCurrentState.rightX) & 0xFF);
            newBytes[10] = (byte)((mCurrentState.rightY >> 8) & 0xFF);
            newBytes[11] = (byte)((mCurrentState.rightY) & 0xFF);
            newBytes[12] = (byte)((mCurrentState.leftTrigger >> 8) & 0xFF);
            newBytes[13] = (byte)((mCurrentState.leftTrigger) & 0xFF);
            newBytes[14] = (byte)((mCurrentState.rightTrigger >> 8) & 0xFF);
            newBytes[15] = (byte)((mCurrentState.rightTrigger) & 0xFF);

            return newBytes;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Return the byte array length
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public int getControllerBytesLength()
        {
            return MAX_BYTE_ARRAY;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Sets the controller struct from a byte array
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public void setControllerBytes(byte[] oldBytes)
        {
            mCurrentState.controlPackage = (short)(((short)oldBytes[0] << 8) + (short)oldBytes[1]);
            mCurrentState.buttons = (short)(((short)oldBytes[2] << 8) + (short)oldBytes[3]);
            mCurrentState.leftX = (short)(((short)oldBytes[4] << 8) + (short)oldBytes[5]);
            mCurrentState.leftY = (short)(((short)oldBytes[6] << 8) + (short)oldBytes[7]);
            mCurrentState.rightX = (short)(((short)oldBytes[8] << 8) + (short)oldBytes[9]);
            mCurrentState.rightY = (short)(((short)oldBytes[10] << 8) + (short)oldBytes[11]);
            mCurrentState.leftTrigger = (short)(((short)oldBytes[12] << 8) + (short)oldBytes[13]);
            mCurrentState.rightTrigger = (short)(((short)oldBytes[14] << 8) + (short)oldBytes[15]);
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns state of the button
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public bool getButtonUp()
        {
            return ((mCurrentState.buttons & BUTTON_MASK_UP) == BUTTON_MASK_UP) ? true : false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns state of the button
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public bool getButtonDown()
        {
            return ((mCurrentState.buttons & BUTTON_MASK_DOWN) == BUTTON_MASK_DOWN) ? true : false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns state of the button
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public bool getButtonLeft()
        {
            return ((mCurrentState.buttons & BUTTON_MASK_LEFT) == BUTTON_MASK_LEFT) ? true : false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns state of the button
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public bool getButtonRight()
        {
            return ((mCurrentState.buttons & BUTTON_MASK_RIGHT) == BUTTON_MASK_RIGHT) ? true : false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns state of the button
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public bool getButtonStart()
        {
            return ((mCurrentState.buttons & BUTTON_MASK_START) == BUTTON_MASK_START) ? true : false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns state of the button
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public bool getButtonBack()
        {
            return ((mCurrentState.buttons & BUTTON_MASK_BACK) == BUTTON_MASK_BACK) ? true : false;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Returns player index of this controller
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public PlayerIndex getPlayerIndex()
        {
            return mCurrentController;
        }
    }
}
