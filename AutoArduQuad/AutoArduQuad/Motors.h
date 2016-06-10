#pragma once

#include <Servo.h>
#include <Arduino.h>

class Motors {
public:
	Motors();
	virtual ~Motors();

	void Init();

	void SetN(int val);
	void SetE(int val);
	void SetS(int val);
	void SetW(int val);

	void SetNS(int base, int offset);
	void SetEW(int base, int offset);

	void StopAll();
protected:
	void ArmMotors();

	Servo _northESC;
	Servo _eastESC;
	Servo _southESC;
	Servo _westESC;
};
