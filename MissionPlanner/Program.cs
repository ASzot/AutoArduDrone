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
using System.Threading;

namespace MissionPlanner
{
    public static class Program
    {
        [STAThread]
        public static void Main(string[] args)
        {
            ArduinoInterface arduino = new ArduinoInterface();

            arduino.InitSystem();

            PrintHelp();

            for (; ; )
            {
                Print(">");

                string input = Console.ReadLine();

                if (input == "quit")
                    break;
                else if (input == "help")
                    PrintHelp();
                else
                    ProcessCommand(input, arduino);

                Return();
            }

            arduino.CloseSystem();
        }

        private static void ProcessCommand(string command, ArduinoInterface ardu)
        {
            if (command == "printcom")
            {
                string[] allPorts = ardu.GetAllPortNames();
                Print(allPorts);
            }
            else if (command == "connect")
            {
                ardu.Connect(ArduinoInterface.ComPort, "COM5", "115200");
                Print("Connected");
            }
            else if (command == "disconnect")
            {
                ardu.Disconnect(ArduinoInterface.ComPort);
                Print("Disconnected");
            }
            else if (command == "motors")
            {
                Print("Throttle %: ");
                string input = Console.ReadLine();
                int throttle = int.Parse(input);

                Print("Timeout: ");
                input = Console.ReadLine();
                int time = int.Parse(input);

                for (int i = 1; i < 5; ++i)
                {
                    PrintRet("Running motor " + i.ToString());
                    if (!ardu.RunMotor(i, throttle, time))
                        Console.WriteLine("Command denied");
                }
            }
            else if (command == "motor")
            {
                Print("Motor #: ");
                string input = Console.ReadLine();
                int motor = int.Parse(input);

                Print("Throttle %: ");
                input = Console.ReadLine();
                int throttle = int.Parse(input);

                Print("Timeout: ");
                input = Console.ReadLine();
                int time = int.Parse(input);

                if (!ardu.RunMotor(motor, throttle, time))
                    Console.WriteLine("Command denied");
            }
        }

        private static void PrintHelp()
        {
            PrintRet("*****ArduCopter Controller*****");
            PrintRet("**General Controls");
            PrintRet("help - print this message");
            PrintRet("quit - quit the program");
            PrintRet("**Ardu Controls");
            PrintRet("printcom - print com options");
            PrintRet("connect");
            PrintRet("disconnect");
            PrintRet("motor");
            PrintRet("motors");
            Return();
            Return();
        }

        private static void PrintRet(string text)
        {
            Print(text);
            Return();
        }

        private static void Print(string text)
        {
            Console.Write(text);
        }

        private static void Print(string[] texts)
        {
            foreach (string text in texts)
                Print("     -" + text);
        }

        private static void Return()
        {
            Console.WriteLine();
        }
    }
}