/*
 * Motors.cpp
 *
 * Handles sending commands to the four ESCs
 *
 *  Created on: May 12, 2012
 *      Author: Thomas Teisberg
 */

#include "Motors.h"


Motors::Motors() {

}

Motors::~Motors() {

}

// Initialize ESCs
void Motors::initMotors()
{
	// Specify the PWM port each ESC is connected to
	northESC.attach(11);
	eastESC.attach(10);
	southESC.attach(9);
	westESC.attach(6);

	// Send a zero command to each ESC
	stopAll();
  armMotors();
}

// Set just the north motor speed
void Motors::setN(int val)
{
	northESC.writeMicroseconds(val);
}

// Set just the east motor speed
void Motors::setE(int val)
{
	eastESC.writeMicroseconds(val);
}

// Set just the south motor speed
void Motors::setS(int val)
{
	southESC.writeMicroseconds(val);
}

// Set just the west motor speed
void Motors::setW(int val)
{
	westESC.writeMicroseconds(val);
}

// Set the north and south motor speeds in terms of a base speed and an offset
// A positive offset means a greater value to the north motor.
void Motors::setNS(int base, int offset){
	int north = base + (offset / 2);
	int south = base - (offset / 2);
	if(offset % 2 != 0){ // if offset is odd
		if(offset > 0){
			north++;
		}else{ // (0 is even, so no else if)
			south++;
		}
	}
	northESC.writeMicroseconds(north);
	southESC.writeMicroseconds(south);
}

// Set the east and west motor speeds in terms of a base speed and an offset
// A positive offset means a greater value to the east motor.
void Motors::setEW(int base, int offset){
	int east = base + (offset / 2);
	int west = base - (offset / 2);
	if(offset % 2 != 0){ // if offset is odd
		if(offset > 0){
			east++;
		}else{ // (0 is even, so no else if)
			west++;
		}
	}
	eastESC.writeMicroseconds(east);
	westESC.writeMicroseconds(west);
}

// Send a safe zero command to each motor
// Note that it is important to consult to documentation for each specific
// ESC to find a value that is safely within the zero range.
void Motors::stopAll()
{
	northESC.writeMicroseconds(700);
	eastESC.writeMicroseconds(700);
	southESC.writeMicroseconds(700);
	westESC.writeMicroseconds(700);
}

void Motors::armMotors() 
{
  northESC.writeMicroseconds(750);
  eastESC.writeMicroseconds(750);
  southESC.writeMicroseconds(750);
  westESC.writeMicroseconds(750);
}

