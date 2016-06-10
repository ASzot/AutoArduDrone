#pragma once

#include "MPU.h"

class Sensors
{
public:
	Sensors(void);
	~Sensors(void);

	bool Init();
	void Update();
	void ZeroAll();

	MPU* GetMPU();

private:
	MPU* _mpu;
};

