/*
 * Motors.h
 *
 *  Created on: May 12, 2012
 *      Author: Thomas Teisberg
 */

#ifndef MOTORS_H_
#define MOTORS_H_

#include <Servo.h>
#include <Arduino.h>

class Motors {
public:
	Motors();
	virtual ~Motors();

	void initMotors();

	void setN(int val);
	void setE(int val);
	void setS(int val);
	void setW(int val);

	void setNS(int base, int offset);
	void setEW(int base, int offset);

	void stopAll();
protected:
  void armMotors();

	Servo northESC, eastESC, southESC, westESC;

};

#endif /* MOTORS_H_ */
