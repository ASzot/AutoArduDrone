#include "QuadController.h"
#include "SerialHelper.h"


QuadController::QuadController()
{
	_sensors = new Sensors();
	_motorMgr = new MotorMgr();
	_run = false;
}


QuadController::~QuadController()
{
	delete _sensors;
	_sensors = 0;

	delete _motorMgr;
	_motorMgr = 0;
}

void QuadController::Update()
{
	_sensors->Update();
	if (_run)
	{
		_motorMgr->Update();
	}

	UpdateInput();
}

bool QuadController::Init()
{
	SerialHelper::Start();

	if (!_sensors->Init())
	{
		SerialHelper::Println("Couldn't init sensors");
		return false;
	}

	if (!_motorMgr->Init(_sensors->GetMPU()))
	{
		SerialHelper::Println("Couldn't init motors");
		return false;
	}

	return true;
}

void QuadController::UpdateInput()
{
	if (!SerialHelper::Avaliable())
		return;
	char input = SerialHelper::GetInput();

	if (input == 'Z')
		_sensors->ZeroAll();
	else if (input == 'R')
		_run = true;
}