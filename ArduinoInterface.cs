using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.Linq;
using MissionPlanner.Utilities;
using System.Drawing;
using System.Text.RegularExpressions;
using MissionPlanner.Comms;

namespace MissionPlanner
{
    class ArduinoInterface
    {
        private Thread _serialReaderThread;
        private bool _serialThreadActive = false;
        private ManualResetEvent _serialThreadrunner = new ManualResetEvent(false);

        /// <summary>
        /// Active Comport interface
        /// </summary>
        public static MAVLinkInterface ComPort = new MAVLinkInterface();

        /// <summary>
        /// passive comports
        /// </summary>
        public static List<MAVLinkInterface> Comports = new List<MAVLinkInterface>();


        /// <summary>
        /// track the last heartbeat sent
        /// </summary>
        private DateTime _heartbeatSend = DateTime.Now;


        public ArduinoInterface()
        {

        }

        public void InitSystem()
        {
            CleanupFiles();

            Application.DoEvents();

            ComPort.BaseStream.BaudRate = 115200;
            ArduinoInterface.ComPort.MAV.cs.ResetInternals();

            Comports.Add(ComPort);


            // setup main serial reader
            _serialReaderThread = new Thread(SerialReader)
            {
                IsBackground = true,
                Name = "Main Serial reader",
                Priority = ThreadPriority.AboveNormal
            };
            _serialReaderThread.Start();
        }

        public void CloseSystem()
        {
            _serialThreadActive = false;
        }

        public void Disconnect(MAVLinkInterface comPort)
        {
            comPort.BaseStream.DtrEnable = false;
            comPort.Close();
        }

        public bool RunMotor(int motorNum, int throttle, int timeout)
        {
            return ComPort.doMotorTest(motorNum, MAVLink.MOTOR_TEST_THROTTLE_TYPE.MOTOR_TEST_THROTTLE_PERCENT, throttle, timeout);
        }

        public void Connect(MAVLinkInterface comPort, string portname, string baud)
        {
            switch (portname)
            {
                case "preset":
                    break;
                case "TCP":
                    comPort.BaseStream = new TcpSerial();
                    break;
                case "UDP":
                    comPort.BaseStream = new UdpSerial();
                    break;
                case "UDPCl":
                    comPort.BaseStream = new UdpSerialConnect();
                    break;
                case "AUTO":
                default:
                    comPort.BaseStream = new SerialPort();
                    break;
            }

            comPort.MAV.cs.ResetInternals();
            comPort.logreadmode = false;
            if (comPort.logplaybackfile != null)
                comPort.logplaybackfile.Close();
            comPort.logplaybackfile = null;
            // set port, then options
            comPort.BaseStream.PortName = portname;

            comPort.BaseStream.BaudRate = int.Parse(baud);

            // prevent serialreader from doing anything
            comPort.giveComport = true;

            //// reset on connect logic.
            //if (config["CHK_resetapmonconnect"] != null &&
            //    bool.Parse(config["CHK_resetapmonconnect"].ToString()) == true)
            //{
            //    log.Info("set dtr rts to false");
            //    comPort.BaseStream.DtrEnable = false;
            //    comPort.BaseStream.RtsEnable = false;

            //    comPort.BaseStream.toggleDTR();
            //}

            comPort.giveComport = false;

            // do the connect
            comPort.Open(false, false);


            // get all mavstates
            var list = comPort.MAVlist.GetMAVStates();

            // get all the params
            foreach (var mavstate in list)
            {
                comPort.sysidcurrent = mavstate.sysid;
                comPort.compidcurrent = mavstate.compid;
                comPort.getParamList();
            }

            // set to first seen
            comPort.sysidcurrent = list[0].sysid;
            comPort.compidcurrent = list[0].compid;
        }

        public string[] GetAllPortNames()
        {
            return SerialPort.GetPortNames();
        }

        static void CleanupFiles()
        {
            //cleanup bad file
            string file = Application.StartupPath + Path.DirectorySeparatorChar +
                          @"LogAnalyzer\tests\TestUnderpowered.py";
            if (File.Exists(file))
            {
                File.Delete(file);
            }
        }

        private void SerialReader()
        {
            if (_serialThreadActive == true)
                return;
            _serialThreadActive = true;

            _serialThreadrunner.Reset();

            int minbytes = 0;

            int altwarningmax = 0;

            bool armedstatus = false;

            string lastmessagehigh = "";

            DateTime speechcustomtime = DateTime.Now;

            DateTime speechlowspeedtime = DateTime.Now;

            DateTime linkqualitytime = DateTime.Now;

            while (_serialThreadActive)
            {
                try
                {
                    Thread.Sleep(1); // was 5 

                    // attenuate the link qualty over time
                    if ((DateTime.Now - ArduinoInterface.ComPort.MAV.lastvalidpacket).TotalSeconds >= 1)
                    {
                        if (linkqualitytime.Second != DateTime.Now.Second)
                        {
                            ArduinoInterface.ComPort.MAV.cs.linkqualitygcs = (ushort)(ArduinoInterface.ComPort.MAV.cs.linkqualitygcs * 0.8f);
                            linkqualitytime = DateTime.Now;
                        }
                    }

                    // send a hb every seconds from gcs to ap
                    if (_heartbeatSend.Second != DateTime.Now.Second)
                    {
                        MAVLink.mavlink_heartbeat_t htb = new MAVLink.mavlink_heartbeat_t()
                        {
                            type = (byte)MAVLink.MAV_TYPE.GCS,
                            autopilot = (byte)MAVLink.MAV_AUTOPILOT.INVALID,
                            mavlink_version = 3 // MAVLink.MAVLINK_VERSION
                        };

                        foreach (var port in Comports)
                        {
                            try
                            {
                                port.sendPacket(htb);
                            }
                            catch (Exception ex)
                            {
                                // close the bad port
                                port.Close();
                            }
                        }

                        _heartbeatSend = DateTime.Now;
                    }

                    // if not connected or busy, sleep and loop
                    if (!ComPort.BaseStream.IsOpen || ComPort.giveComport == true)
                    {
                        if (!ComPort.BaseStream.IsOpen)
                        {
                            // check if other ports are still open
                            foreach (var port in Comports)
                            {
                                if (port.BaseStream.IsOpen)
                                {
                                    Console.WriteLine("Main comport shut, swapping to other mav");
                                    ComPort = port;
                                    break;
                                }
                            }
                        }

                        System.Threading.Thread.Sleep(100);
                    }

                    // read the interfaces
                    foreach (var port in Comports)
                    {
                        if (!port.BaseStream.IsOpen)
                        {
                            // skip primary interface
                            if (port == ComPort)
                                continue;

                            // modify array and drop out
                            Comports.Remove(port);
                            break;
                        }

                        while (port.BaseStream.IsOpen && port.BaseStream.BytesToRead > minbytes &&
                               port.giveComport == false)
                        {
                            try
                            {
                                port.readPacket();
                            }
                            catch (Exception ex)
                            {
                            }
                        }
                        // update currentstate of sysids on the port
                        foreach (var MAV in port.MAVlist.GetMAVStates())
                        {
                            try
                            {
                                MAV.cs.UpdateCurrentSettings(null, false, port, MAV);
                            }
                            catch (Exception ex)
                            {
                            }
                        }
                    }
                }
                catch (Exception e)
                {
                    try
                    {
                        ComPort.Close();
                    }
                    catch (Exception ex)
                    {
                    }
                }
            }

            Console.WriteLine("SerialReader Done");
            _serialThreadrunner.Set();
        }
    }
}
