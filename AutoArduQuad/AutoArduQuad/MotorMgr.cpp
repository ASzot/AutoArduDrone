#include "MotorMgr.h"
#include "SerialHelper.h"

MotorMgr::MotorMgr(void)
{
	_motors = new Motors();
	_baseSpeed = 1000;
	_testMode = false;
	_setNS = 0.0;
	_setEW = 0.0;
	_setYaw = 0.0;

	_nsOut = 0.0;
	_ewOut = 0.0;
	_yawOut = 0.0;

	_verboseNS = false;
	_verboseEW = false;
	_verboseYaw = false;
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

void MotorMgr::SetBaseSpeed(double d)
{
	_baseSpeed = d;
}

double MotorMgr::GetBaseSpeed()
{
	return _baseSpeed;
}

bool MotorMgr::Init(MPU* mpu)
{
	_ypr0 = &mpu->YPR[0];
	_ypr1 = &mpu->YPR[1];
	_ypr2 = &mpu->YPR[2];

	_nsController = new PID(&mpu->YPR[2], &_nsOut, &_setNS, 7, 0.0, 1.1, REVERSE);
	_ewController = new PID(&mpu->YPR[1], &_ewOut, &_setEW, 10.0, 0.06, 1.5, REVERSE);
	_yawController = new PID(&mpu->YPR[0], &_yawOut, &_setYaw, 2.0, 0.0, 0.0, DIRECT);
	
	_ewController->SetMode(AUTOMATIC);
	_yawController->SetMode(AUTOMATIC);
	_nsController->SetMode(AUTOMATIC);

	_nsController->SetOutputLimits(-1000, 1000);
	_ewController->SetOutputLimits(-1000, 1000);
	_yawController->SetOutputLimits(-300, 300);

	_motors->Init();

	// Set here which controller should be affected by tuning parameters. 
	_editController = _yawController;

	return true;
}

void MotorMgr::ToggleTestMode()
{
	_testMode = !_testMode;
}

void MotorMgr::Update()
{
	if (_testMode)
	{
		_motors->SetN(_baseSpeed);
		_motors->SetE(_baseSpeed);
		_motors->SetS(_baseSpeed);
		_motors->SetW(_baseSpeed);
		return;
	}

	_nsController->Compute(_verboseNS);
	_ewController->Compute(_verboseEW);
	_yawController->Compute(_verboseYaw);

	SerialHelper::Print("Yaw: ");
	SerialHelper::Println(_yawOut);
	
	SerialHelper::Print("EW: ");
	SerialHelper::Println(_ewOut);
	
	SerialHelper::Print("NS: ");
	SerialHelper::Println(_nsOut);

	_motors->SetNS(_baseSpeed + _yawOut, _nsOut);
	_motors->SetEW(_baseSpeed - _yawOut, _ewOut);

}

bool MotorMgr::GetTestMode()
{
	return _testMode;
}

void MotorMgr::StopAll()
{
	_motors->StopAll();
}

void MotorMgr::IncreaseP()
{
	_editController->SetTunings(_editController->GetKp() + 0.1, _editController->GetKi(), _editController->GetKd());
}

void MotorMgr::DecreaseP()
{
	_editController->SetTunings(_editController->GetKp() - 0.1, _editController->GetKi(), _editController->GetKd());
}

void MotorMgr::IncreaseI()
{
	_editController->SetTunings(_editController->GetKp(), _editController->GetKi() + 0.1, _editController->GetKd());
}

void MotorMgr::DecreaseI()
{
	_editController->SetTunings(_editController->GetKp(), _editController->GetKi() - 0.1, _editController->GetKd());
}

void MotorMgr::IncreaseD()
{
	_editController->SetTunings(_editController->GetKp(), _editController->GetKi(), _editController->GetKd() + 0.1);
}

void MotorMgr::DecreaseD()
{
	_editController->SetTunings(_editController->GetKp(), _editController->GetKi(), _editController->GetKd() - 0.1);
}

void MotorMgr::GetP()
{
	_editController->GetKp();
}

void MotorMgr::GetI()
{
	_editController->GetKi();
}

void MotorMgr::GetD()
{
	_editController->GetKd();
}

void MotorMgr::SetVerboseEW(bool verboseEW)
{
	_verboseEW = verboseEW;
}

void MotorMgr::SetVerboseNS(bool verboseNS)
{
	_verboseNS = verboseNS;
}

void MotorMgr::SetVerboseYaw(bool verboseYaw)
{
	_verboseYaw = verboseYaw;
}

bool MotorMgr::GetVerboseEW()
{
	return _verboseEW;
}

bool MotorMgr::GetVerboseNS()
{
	return _verboseNS;
}

bool MotorMgr::GetVerboseYaw()
{
	return _verboseYaw;
}