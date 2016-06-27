#pragma once

#include "MPU.h"

class Sensors
{
public:
	Sensors(void);
	~Sensors(void);

	bool Init();
	void Update();
	bool UpdateCal(unsigned long time);
	void ZeroAll();

	MPU* GetMPU();

private:
	MPU* _mpu;
	unsigned long _lastTime;
};

