#include "Sensors.h"
#include "SerialHelper.h"
#include <Arduino.h>


Sensors::Sensors(void)
{
	_mpu = new MPU();
	_lastTime = 0.0;
}


Sensors::~Sensors(void)
{
	delete _mpu;
	_mpu = 0;
}

bool Sensors::Init()
{
	if (!_mpu->Init())
	{
		SerialHelper::Println("Couldn't init MPU");
		return false;
	}

	return true;
}

void Sensors::ZeroAll()
{
	_mpu->Zero();
}

void Sensors::Update()
{
	_mpu->Update();
}

bool Sensors::UpdateCal(unsigned long time)
{
	unsigned long dTime = time - _lastTime;
	_lastTime = time;

	if (_mpu->UpdateCal(dTime))
		return true;

	return false;
}

MPU* Sensors::GetMPU()
{
	return _mpu;
}