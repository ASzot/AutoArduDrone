#include "Sensors.h"
#include "SerialHelper.h"


Sensors::Sensors(void)
{
	_mpu = new MPU();
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

MPU* Sensors::GetMPU()
{
	return _mpu;
}