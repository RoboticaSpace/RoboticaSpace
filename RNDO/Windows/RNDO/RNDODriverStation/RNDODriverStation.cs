//----------------------------------------------------------------------------
//
//  $Workfile: RNDODriverStation.cs$
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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using RNDO;
using RNDOClientInterface;
using XBox360Controller;

namespace RNDODriverStation
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: RNDODriverStation
    // 
    // Purpose:
    //      This class is the GUI for the drivers Station
    //
    //----------------------------------------------------------------------------
    public partial class RNDODriverStation : Form
    {
        private RNDOMessage mControlMessage = new RNDOMessage();
        private RNDOClient mClient = new RNDOClient();
        UInt32 mCommonTransmitID=0;
        private XBox360Controller.XBox360Controller mXBoxController = new XBox360Controller.XBox360Controller(0);

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public RNDODriverStation()
        {
            InitializeComponent();
            tSend.Enabled = true;

            mCommonTransmitID = mClient.openSenderPort("RNDOCommon");

            mControlMessage = mClient.getTransNDOMessage(mCommonTransmitID, 0xB0);
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Send the message
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void tSend_Tick(object sender, EventArgs e)
        {
            UInt16 number = (UInt16)nNumber.Value;
            mXBoxController.readController();
            XBox360Controller.XBox360ControllerStruct xboxStruct = mXBoxController.getControllerStruct();

            mControlMessage.Set16BitBlock((UInt16)xboxStruct.buttons, 0);
            mControlMessage.Set16BitBlock((UInt16)xboxStruct.leftX, 1);
            mControlMessage.Set16BitBlock((UInt16)xboxStruct.leftY, 2);
            mControlMessage.Set16BitBlock((UInt16)xboxStruct.rightX, 3);
            mControlMessage.Set16BitBlock((UInt16)xboxStruct.rightY, 4);
            mControlMessage.Set16BitBlock((UInt16)xboxStruct.leftTrigger, 5);
            mControlMessage.Set16BitBlock((UInt16)xboxStruct.rightTrigger, 6);

            lController.Text = "Button:" + xboxStruct.buttons.ToString();
            lController.Text += "\nLeft X:" + xboxStruct.leftX.ToString();
            lController.Text += "\nLeft Y:" + xboxStruct.leftY.ToString();
            lController.Text += "\nRight X:" + xboxStruct.rightX.ToString();
            lController.Text += "\nRight Y:" + xboxStruct.rightY.ToString();
            lController.Text += "\nLeft Trigger:" + xboxStruct.leftTrigger.ToString();
            lController.Text += "\nRight Trigger:" + xboxStruct.rightTrigger.ToString();

            lStatus.Text =   mClient.getTransmitTimeSpan(mCommonTransmitID).ToString();
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Close the form
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RNDODriverStation_FormClosing(object sender, FormClosingEventArgs e)
        {
            mClient.closeSenderPort(mCommonTransmitID);
        }
    }
}
