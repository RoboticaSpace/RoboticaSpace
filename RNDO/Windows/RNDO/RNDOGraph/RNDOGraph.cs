//----------------------------------------------------------------------------
//
//  $Workfile: RNDOGraph.cs$
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

namespace RNDOGraph
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: RNDOGraph
    // 
    // Purpose:
    //      This class handles drawing the graph
    //
    //----------------------------------------------------------------------------
    public partial class RNDOGraph : Form
    {
        //----------------------------------------------------------------------------
        //  Class constants
        //----------------------------------------------------------------------------
        const int MAX_DATA_POINTS = 800;
        const int HEIGHT = 400;
        const int TOP_OFFSET = 20;
        const int LEFT_OFFSET = 20;

        //----------------------------------------------------------------------------
        //  Class Attributes
        //----------------------------------------------------------------------------
        private RNDOMessage mControlMessage = new RNDOMessage();
        private RNDOMessage mGyroDebug = new RNDOMessage();
        private RNDOMessage mPID = new RNDOMessage();
        private RNDOClient mClient = new RNDOClient();
        UInt32 mCommonReceiveID = 0;
        PointChart mPointChart = new PointChart();
        Channel mNumber1 = new Channel(Color.Purple, MAX_DATA_POINTS);
        Channel mNumber2 = new Channel(Color.Orange, MAX_DATA_POINTS);
        Channel mNumber3 = new Channel(Color.Red, MAX_DATA_POINTS);
        Channel mNumber4 = new Channel(Color.DarkCyan, MAX_DATA_POINTS);
        Channel mNumber5 = new Channel(Color.Green, MAX_DATA_POINTS);
        Channel mNumber6 = new Channel(Color.Blue, MAX_DATA_POINTS);
        Channel mNumber7 = new Channel(Color.DarkGreen, MAX_DATA_POINTS);
        Channel mNumber8 = new Channel(Color.Purple, MAX_DATA_POINTS);
        Channel mNumber9 = new Channel(Color.Olive, MAX_DATA_POINTS);
        Channel mNumber10 = new Channel(Color.Red, MAX_DATA_POINTS);
        int mCount = 0;

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public RNDOGraph()
        {
            InitializeComponent();
            mCommonReceiveID = mClient.openListenerPort("RNDOCommon");

            mControlMessage = mClient.getReceiveRNDOMessage(mCommonReceiveID, 0xB0);
            mGyroDebug = mClient.getReceiveRNDOMessage(mCommonReceiveID, 0x30, 0x01);
            mPID = mClient.getReceiveRNDOMessage(mCommonReceiveID, 0x21, 0x01);

            mPointChart = new PointChart(LEFT_OFFSET, TOP_OFFSET, MAX_DATA_POINTS, HEIGHT);
            mPointChart.AddChannel(mNumber1);
            mPointChart.AddChannel(mNumber2);
            mPointChart.AddChannel(mNumber3);
            mPointChart.AddChannel(mNumber4);
            mPointChart.AddChannel(mNumber5);
            mPointChart.AddChannel(mNumber6);
            mPointChart.AddChannel(mNumber7);
            mPointChart.AddChannel(mNumber8);
            mPointChart.AddChannel(mNumber9);
            mPointChart.AddChannel(mNumber10);
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Load the form
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RNDOGraph_Load(object sender, EventArgs e)
        {
            tDisplay.Enabled = true;
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Close the form
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RNDOGraph_FormClosing(object sender, FormClosingEventArgs e)
        {
            mClient.closeListenerPort(mCommonReceiveID);
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Display the chart
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void tDisplay_Tick(object sender, EventArgs e)
        {
            lData.Text = mGyroDebug.Get16BitBlock(0).ToString();
            int number = (Int16)mGyroDebug.GetDoublebitBlock(0, 10);
            mNumber1.AddPoint(number);
            number = (Int16)mGyroDebug.GetDoublebitBlock(1, 10);
            mNumber2.AddPoint(number);
            number = (Int16)mGyroDebug.GetDoublebitBlock(2, 10);
            mNumber3.AddPoint(number);
            number = (Int16)mGyroDebug.GetDoublebitBlock(3, 10);
            mNumber4.AddPoint(number);
            number = (Int16)mPID.Get16BitBlock(0);
            mNumber5.AddPoint(number/5);
            number = (Int16)mPID.Get16BitBlock(1);
            mNumber6.AddPoint(number/5);
            number = (Int16)mPID.Get16BitBlock(2);
            mNumber7.AddPoint(number*2);
            number = (Int16)mPID.Get16BitBlock(3);
            mNumber8.AddPoint(number/100);
            number = (Int16)mPID.Get16BitBlock(4);
            mNumber9.AddPoint(number);
            number = (Int16)mPID.Get16BitBlock(5);
            mNumber10.AddPoint(number / 15);

            mCount++;

            this.Invalidate();
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Paint the screen
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RNDOGraph_Paint(object sender, PaintEventArgs e)
        {
            mPointChart.Draw(e.Graphics);
        }
    }
}
