using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using RNDO;

namespace RNDOServer
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: RNDOServerScreen
    // 
    // Purpose:
    //      Show the movement of messages in the system
    //
    //----------------------------------------------------------------------------
    public partial class RNDOServerScreen : Form
    {
        HandleIn mComMessagesIn;
        HandleOut mComMessagesOut;
        List<RNDOMessage> mMessages = new List<RNDOMessage>();

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Constructor
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        public RNDOServerScreen()
        {
            InitializeComponent();
            ConfigFile.loadDefFile();

            ListViewItem newRow = new ListViewItem("RNDOCommon");
            newRow.SubItems.Add("Rec");
            newRow.SubItems.Add("0");
            newRow.SubItems.Add("0");
            lvStatus.Items.Add(newRow);
            newRow = new ListViewItem("RNDOCommon");
            newRow.SubItems.Add("Trns");
            newRow.SubItems.Add("0");
            newRow.SubItems.Add("0");
            lvStatus.Items.Add(newRow);
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Load the form
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void RNDOServer_Load(object sender, EventArgs e)
        {
            mComMessagesIn = new HandleIn("RNDOCommon", mMessages);
            mComMessagesIn.start();
            mComMessagesOut = new HandleOut("RNDOCommon", mMessages);
            mComMessagesOut.start();
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
        private void RNDOServer_FormClosing(object sender, FormClosingEventArgs e)
        {
            mComMessagesIn.stop();
        }

        //----------------------------------------------------------------------------
        //  Purpose:
        //      Updates the form
        //
        //  Notes:
        //      None
        //
        //----------------------------------------------------------------------------
        private void tDisplay_Tick(object sender, EventArgs e)
        {
            lvStatus.Items[0].SubItems[2].Text = mComMessagesIn.CountOfConnections().ToString();
            lvStatus.Items[0].SubItems[3].Text = mComMessagesIn.mReceiveTimeSpan.TotalMilliseconds.ToString();

            lvStatus.Items[1].SubItems[2].Text = mComMessagesOut.mClientSockets.Count.ToString();
            lvStatus.Items[1].SubItems[3].Text = mComMessagesOut.mReceiveTimeSpan.TotalMilliseconds.ToString("F0");

            lvMessages.Items.Clear();
            for(int i=0;i<mMessages.Count;i++)
            {
                ListViewItem newRow = new ListViewItem(mMessages[i].GetID().ToString());
                newRow.SubItems.Add(mMessages[i].GetRobot().ToString());
                newRow.SubItems.Add(mMessages[i].GetDataString());
                newRow.SubItems.Add(mMessages[i].GetCheck().ToString());
                newRow.SubItems.Add((true == mMessages[i].GetStale()?"true":"false"));
                lvMessages.Items.Add(newRow);
            }
        }
    }
}
