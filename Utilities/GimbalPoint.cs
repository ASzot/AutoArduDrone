using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace MissionPlanner.Utilities
{
    class GimbalPoint
    {
        public static int yawchannel = 7;
        public static int pitchchannel = 5;
        public static int rollchannel = -1;

        public enum axis
        {
            roll,
            pitch,
            yaw
        }

        /// returns the angle (degrees*100) that the RC_Channel input is receiving
        static float angle_input(bool rev, float radio_in, float radio_min, float radio_max, float angle_min,
            float angle_max)
        {
            return (rev ? -1.0f : 1.0f)*(radio_in - radio_min)*(angle_max - angle_min)/(radio_max - radio_min) +
                   (rev ? angle_max : angle_min);
        }

        static int channelpwm(int channel)
        {
            if (channel == 1)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch1out;
            if (channel == 2)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch2out;
            if (channel == 3)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch3out;
            if (channel == 4)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch4out;
            if (channel == 5)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch5out;
            if (channel == 6)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch6out;
            if (channel == 7)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch7out;
            if (channel == 8)
                return (int) (float) ArduinoInterface.ComPort.MAV.cs.ch8out;

            return 0;
        }

        public static double ConvertPwmtoAngle(axis axis)
        {
            int pwmvalue = -1;

            if (!ArduinoInterface.ComPort.MAV.param.ContainsKey("RC" + yawchannel + "_MIN"))
                return 0;

            switch (axis)
            {
                case GimbalPoint.axis.roll:
                    pwmvalue = channelpwm(rollchannel);
                    float minr = (float) ArduinoInterface.ComPort.MAV.param["RC" + rollchannel + "_MIN"];
                    float maxr = (float) ArduinoInterface.ComPort.MAV.param["RC" + rollchannel + "_MAX"];
                    float minroll = (float) ArduinoInterface.ComPort.MAV.param["MNT_ANGMIN_ROL"];
                    float maxroll = (float) ArduinoInterface.ComPort.MAV.param["MNT_ANGMAX_ROL"];
                    float revr = (float) ArduinoInterface.ComPort.MAV.param["RC" + rollchannel + "_REV"];

                    return angle_input(revr != 1, pwmvalue, minr, maxr, minroll, maxroll)/100.0;

                case GimbalPoint.axis.pitch:
                    pwmvalue = channelpwm(pitchchannel);
                    float minp = (float) ArduinoInterface.ComPort.MAV.param["RC" + pitchchannel + "_MIN"];
                    float maxp = (float) ArduinoInterface.ComPort.MAV.param["RC" + pitchchannel + "_MAX"];
                    float minpitch = (float) ArduinoInterface.ComPort.MAV.param["MNT_ANGMIN_TIL"];
                    float maxpitch = (float) ArduinoInterface.ComPort.MAV.param["MNT_ANGMAX_TIL"];
                    float revp = (float) ArduinoInterface.ComPort.MAV.param["RC" + pitchchannel + "_REV"];


                    return angle_input(revp != 1, pwmvalue, minp, maxp, minpitch, maxpitch)/100.0;

                case GimbalPoint.axis.yaw:
                    pwmvalue = channelpwm(yawchannel);
                    float miny = (float) ArduinoInterface.ComPort.MAV.param["RC" + yawchannel + "_MIN"];
                    float maxy = (float) ArduinoInterface.ComPort.MAV.param["RC" + yawchannel + "_MAX"];
                    float minyaw = (float) ArduinoInterface.ComPort.MAV.param["MNT_ANGMIN_PAN"];
                    float maxyaw = (float) ArduinoInterface.ComPort.MAV.param["MNT_ANGMAX_PAN"];
                    float revy = (float) ArduinoInterface.ComPort.MAV.param["RC" + yawchannel + "_REV"];

                    return angle_input(revy != 1, pwmvalue, miny, maxy, minyaw, maxyaw)/100.0;
            }

            return 0;
        }

        public static PointF FindLineIntersection(PointF start1, PointF end1, PointF start2, PointF end2)
        {
            double denom = ((end1.X - start1.X)*(end2.Y - start2.Y)) - ((end1.Y - start1.Y)*(end2.X - start2.X));
            //  AB & CD are parallel         
            if (denom == 0)
                return new PointF();
            double numer = ((start1.Y - start2.Y)*(end2.X - start2.X)) - ((start1.X - start2.X)*(end2.Y - start2.Y));
            double r = numer/denom;
            double numer2 = ((start1.Y - start2.Y)*(end1.X - start1.X)) - ((start1.X - start2.X)*(end1.Y - start1.Y));
            double s = numer2/denom;
            if ((r < 0 || r > 1) || (s < 0 || s > 1))
                return new PointF();
            // Find intersection point      
            PointF result = new PointF();
            result.X = (float) (start1.X + (r*(end1.X - start1.X)));
            result.Y = (float) (start1.Y + (r*(end1.Y - start1.Y)));
            return result;
        }

        public const double rad2deg = (180/System.Math.PI);
        public const double deg2rad = (1.0/rad2deg);
    }
}