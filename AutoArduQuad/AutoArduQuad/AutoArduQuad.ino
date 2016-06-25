#include "QuadController.h"
#include "SerialHelper.h"

QuadController g_quadController;
bool failed = false;

void setup()
{
	SerialHelper::Println("Starting");
	failed = !g_quadController.Init();
}

void loop()
{
	if (failed)
		return;

	g_quadController.Update();
}
