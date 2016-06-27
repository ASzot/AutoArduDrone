#include "Common.h"

#include <Arduino.h>

double Pow(float base, float power)
{
	return pow(base, power);
}

double Pow(double base, double power)
{
	return Pow((float)base, (float)power);
}