#include "MPU.h"

MPU g_mpu;

void setup()
{
	Serial.begin(115200);

	if (!g_mpu.Init())
		Serial.println("Couldn't initialize");
	else
		Serial.println("Initialized.");

	delay(1000);
	g_mpu.Zero();
}

void loop()
{
	g_mpu.Update();
	if(Serial.available())
	{
		byte input = Serial.read();
		if (input == 'z')
		{
			Serial.println("Zeroing");
			g_mpu.Zero();
		}
		else if (input == 's')
		{
			g_mpu.ShowAll = !g_mpu.ShowAll;
		}
	}

	//g_mpu.PrintDebugStr();
}
