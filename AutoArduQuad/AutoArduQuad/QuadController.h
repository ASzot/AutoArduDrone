#pragma once

#include "Sensors.h"
#include "MotorMgr.h"

class QuadController
{
public:
	QuadController();
	~QuadController();

	void Update();
	bool Init();

private:
	void UpdateInput();

private:
	Sensors* _sensors;
	MotorMgr* _motorMgr;
	bool _run;
};

