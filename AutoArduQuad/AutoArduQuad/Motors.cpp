#include "Motors.h"
#include "SerialHelper.h"

Motors::Motors() 
{

}

Motors::~Motors() 
{

}

void Motors::Init()
{
	// Specify the PWM port each ESC is connected to
	_northESC.attach(11);
	_eastESC.attach(10);
	_southESC.attach(9);
	_westESC.attach(6);

	// Send a zero command to each ESC
	WriteAll(700);
	ArmMotors();
}

void Motors::SetN(int val)
{
	_northESC.writeMicroseconds(val);
}

void Motors::SetE(int val)
{
	_eastESC.writeMicroseconds(val);
}

void Motors::SetS(int val)
{
	_southESC.writeMicroseconds(val);
}

void Motors::SetW(int val)
{
	_westESC.writeMicroseconds(val);
}

// Set the north and south motor speeds in terms of a base speed and an offset
// A positive offset means a greater value to the north motor.
void Motors::SetNS(int base, int offset)
{
	int north = base + (offset / 2);
	int south = base - (offset / 2);
	if(offset % 2 != 0)
	{ 
		// if offset is odd
		if(offset > 0)
		{
			north++;
		}
		else
		{ 
			// (0 is even, so no else if)
			south++;
		}
	}

	_northESC.writeMicroseconds(north);
	_southESC.writeMicroseconds(south);
}

// Set the east and west motor speeds in terms of a base speed and an offset
// A positive offset means a greater value to the east motor.
void Motors::SetEW(int base, int offset)
{
	int east = base + (offset / 2);
	int west = base - (offset / 2);
	if(offset % 2 != 0)
	{ 
		// if offset is odd
		if(offset > 0)
		{
			east++;
		}
		else
		{ 
			// (0 is even, so no else if)
			west++;
		}
	}

	_eastESC.writeMicroseconds(east);
	_westESC.writeMicroseconds(west);
}

// Send a safe zero command to each motor
// Note that it is important to consult to documentation for each specific
// ESC to find a value that is safely within the zero range.
void Motors::StopAll()
{
	WriteAll(800);
}

void Motors::ArmMotors() 
{
	WriteAll(750);
}

void Motors::WriteAll(int val)
{
	_northESC.writeMicroseconds(val);
	_eastESC.writeMicroseconds(val);
	_southESC.writeMicroseconds(val);
	_westESC.writeMicroseconds(val);
}
