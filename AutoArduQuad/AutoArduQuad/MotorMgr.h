#pragma once

#include "Motors.h"
#include "MPU.h"
#include "PID.h"

class MotorMgr
{
public:
	MotorMgr(void);
	~MotorMgr(void);

	bool Init(MPU* mpu);
	void Update();

	void SetBaseSpeed(double baseSpeed);
	double GetBaseSpeed();

	void StopAll();

	void ToggleTestMode();
	bool GetTestMode();

	void IncreaseP();
	void DecreaseP();
	void IncreaseI();
	void DecreaseI();
	void IncreaseD();
	void DecreaseD();

	void GetP();
	void GetI();
	void GetD();

	void SetVerboseEW(bool verboseEW);
	void SetVerboseNS(bool verboseNS);
	void SetVerboseYaw(bool verboseYaw);

	bool GetVerboseEW();
	bool GetVerboseNS();
	bool GetVerboseYaw();

private: 
	PID* _nsController;
	PID* _ewController;
	PID* _yawController;

	PID* _editController;

	Motors* _motors;

	double _nsOut;
	double _ewOut;
	double _yawOut;
	double _baseSpeed;
	double _setNS;
	double _setEW;
	double _setYaw;

	double *_ypr0;
	double *_ypr1;
	double *_ypr2;

	bool _testMode;
	bool _verboseNS;
	bool _verboseEW;
	bool _verboseYaw;
};

