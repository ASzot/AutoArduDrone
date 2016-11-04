#include "SerialHelper.h"
#include "Arduino.h"

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include "Arduino.h"
//#else
//	#include "WProgram.h"
//#endif

namespace SerialHelper
{
	void Start()
	{
		Serial.begin(115200);
	}

	void Print(int i)
	{
		Serial.print(i);
	}

	void Print(char *str)
	{
		Serial.print(str);
	}

	void Print(const char* str)
	{
		Serial.print(str);
	}

	void Print(double d)
	{
		Serial.print(d);
	}

	void Print(long l)
	{
		Serial.print(l);
	}

	void Print(unsigned long l)
	{
		Serial.print(l);
	}

	void Println(int i)
	{
		Print(i);
		Println();
	}

	void Println(char *str)
	{
		Print(str);
		Println();
	}

	void Println(const char* str)
	{
		Print(str);
		Println();
	}

	void Println(long l)
	{
		Print(l);
		Println();
	}

	void Println(unsigned long l)
	{
		Print(l);
		Println();
	}

	void Println(double d)
	{
		Print(d);
		Println();
	}

	void Println()
	{
		Serial.println();
	}

	bool Avaliable()
	{
		return Serial.available();
	}

	char GetInput()
	{
		return (char)Serial.read();
	}
}

