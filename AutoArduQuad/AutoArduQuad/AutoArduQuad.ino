#include "QuadController.h"

QuadController g_quadController;
bool failed = false;

void setup()
{
	//failed = !g_quadController->Init();
}

void loop()
{
	if (failed)
		return;

	//g_quadController->Update();
}
