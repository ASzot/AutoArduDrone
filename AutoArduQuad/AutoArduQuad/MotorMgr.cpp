#include "MotorMgr.h"
#include "SerialHelper.h"

#define BASE_SPEED 1000

MotorMgr::MotorMgr(void)
{
	_motors = new Motors();
}


MotorMgr::~MotorMgr(void)
{
	delete _nsController;
	_nsController = 0;

	delete _ewController;
	_ewController = 0;

	delete _yawController;
	_yawController = 0;

	delete _motors;
	_motors = 0;
}

bool MotorMgr::Init(MPU* mpu)
{
	double nsSet = 0;
	_nsController = new PID(&mpu->YPR[2], &_nsOut, &nsSet, 7, 0.0, 1.1, REVERSE);

	double ewSet = 0;
	_ewController = new PID(&mpu->YPR[1], &_ewOut, &ewSet, 10.0, 0.06, 1.5, REVERSE);

	double yawSet = 0;
	_yawController = new PID(&mpu->YPR[0], &_yawOut, &yawSet, 2.0, 0.0, 0.0, DIRECT);

	_nsController->SetOutputLimits(-1000, 1000);
	_nsController->SetMode(AUTOMATIC);
	_ewController->SetOutputLimits(-1000, 1000);
	_ewController->SetMode(AUTOMATIC);
	_yawController->SetOutputLimits(-300, 300);
	_yawController->SetMode(AUTOMATIC);

	_motors->Init();

	return true;
}

void MotorMgr::Update()
{
	_nsController->Compute();
	_ewController->Compute();
	_yawController->Compute();

	_motors->SetNS(BASE_SPEED + _yawOut, _nsOut);
	_motors->SetEW(BASE_SPEED - _yawOut, _ewOut);
}