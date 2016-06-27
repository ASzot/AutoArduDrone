#pragma once

#include "Sensors.h"
#include "MotorMgr.h"

class QuadController
{
public:
	QuadController();
	~QuadController();

	void Update(unsigned long time);
	bool Init();

private:
	void UpdateInput();

private:
	Sensors* _sensors;
	MotorMgr* _motorMgr;
	bool _run;
	bool _systemsCalibrated;
};

