#pragma once

#include <PID_v1.h>
#include "Motors.h"
#include "MPU.h"

class MotorMgr
{
public:
	MotorMgr(void);
	~MotorMgr(void);

	bool Init(MPU* mpu);
	void Update();

private: 
	PID* _nsController;
	PID* _ewController;
	PID* _yawController;

	Motors* _motors;

	double _nsOut;
	double _ewOut;
	double _yawOut;
};

