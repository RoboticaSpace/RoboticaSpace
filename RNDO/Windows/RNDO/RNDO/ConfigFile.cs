//----------------------------------------------------------------------------
//
//  $Workfile: ConfigFile.cs$
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
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace RNDO
{
    //----------------------------------------------------------------------------
    //  Class Declarations
    //----------------------------------------------------------------------------
    //
    // Class Name: ConfigFile
    // 
    // Purpose:
    //      Loads the RNDO config file into memory and save it if changed.
    //
    //----------------------------------------------------------------------------
    public class ConfigFile
    {
        //----------------------------------------------------------------------------
        //  Class Constants 
        //----------------------------------------------------------------------------
        const string RNDO_CONFIG_TAG = "RNDOConfig";
        const string RNDO_IP_TAG = "RNDOIP";
        const string LOG_FILE_ROOT_TAG = "LogFileRoot";
        const string PORT_TAG = "port";
        const string NAME_TAG = "name";
        const string DIRECTION_TAG = "direction";

        public static int MAX_MESSAGES = 100;
        public static int MAX_BUFFER_SIZE = MAX_MESSAGES*RNDOMessage.MAX_FULL_MESSAGE_SIZE;
        public static string mRNDOIP = "127.0.0.1";
        public static string mLogFileRoot = "C:\\LogFiles";

        static List<Port> mPortList = new List<Port>();

        class Port
        {
            public string mDesc;
            public string mDir;
            public UInt32 mPort;

            public Port()
            {
                mDesc = "";
                mDir = "";
                mPort = 0;
            }

            public void addPort(string port)
            {
                UInt32.TryParse(port, out mPort);
            }
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     Load the ECBU Definition XML file
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public static void loadDefFile()
        {
            try
            {
                XmlTextReader reader = new XmlTextReader("RNDOConfig.xml");

                while (reader.Read())
                {
                    switch (reader.NodeType)
                    {
                        case XmlNodeType.Element: // The node is an element.
                            if (RNDO_IP_TAG == reader.Name)
                            {
                                reader.Read();
                                mRNDOIP = reader.Value;

                            }
                            if (LOG_FILE_ROOT_TAG == reader.Name)
                            {
                                reader.Read();
                                mLogFileRoot = reader.Value;

                            }
                            if (PORT_TAG == reader.Name)
                            {
                                Port port = new Port();
                                while (reader.MoveToNextAttribute())
                                {
                                    if (NAME_TAG == reader.Name)
                                    {
                                        port.mDesc = reader.Value;
                                    }
                                    if (DIRECTION_TAG == reader.Name)
                                    {
                                        port.mDir = reader.Value;
                                    }
                                }
                                reader.Read();
                                port.addPort(reader.Value);
                                mPortList.Add(port);
                            }
                            break;
                    }
                }
                reader.Close();
            }
            catch (Exception)
            {
            }
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     Return the log file root.
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public static string returnLogFileRoot()
        {
            string returnString = mLogFileRoot + "\\";

            returnString = returnString.Replace("\\\\", "\\");

            Directory.CreateDirectory(returnString);

            return returnString;
        }

        //--------------------------------------------------------------------
        // Purpose:
        //     find the AFDX Port
        //
        // Notes:
        //     None.
        //--------------------------------------------------------------------
        public static UInt32 returnTCPIPPort(string name, string direction)
        {
            for (int index = 0; index < mPortList.Count; index++)
            {
                if ((name == mPortList[index].mDesc) &&
                    (direction == mPortList[index].mDir))
                {
                    return mPortList[index].mPort;
                }
            }
            return 0;
        }
    }
}
