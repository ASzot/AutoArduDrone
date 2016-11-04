#include "QuadController.h"
#include "SerialHelper.h"


QuadController::QuadController()
{
	_sensors = new Sensors();
	_motorMgr = new MotorMgr();
	_run = false;
	_systemsCalibrated = false;
}


QuadController::~QuadController()
{
	delete _sensors;
	_sensors = 0;

	delete _motorMgr;
	_motorMgr = 0;
}

void QuadController::Update(unsigned long time)
{
	if (!_systemsCalibrated)
	{
		if (_sensors->UpdateCal(time))
		{
			_systemsCalibrated = true;
			SerialHelper::Println("Systems Calibrated.");
		}
	}
	else
	{
		_sensors->Update();
		//if (_run)
		//{
		//	_motorMgr->Update();
		//}

		UpdateInput();
	}
}

bool QuadController::Init()
{
	SerialHelper::Start();
	SerialHelper::Println("Press any key to start");
	while (!SerialHelper::Avaliable())
	{
		// Do nothing. 
	}

	if (!_sensors->Init())
	{
		SerialHelper::Println("Couldn't init sensors");
		return false;
	}

	//if (!_motorMgr->Init(_sensors->GetMPU()))
	//{
	//	SerialHelper::Println("Couldn't init motors");
	//	return false;
	//}

	SerialHelper::Println("Init successful");
	SerialHelper::Println("Calibrating Systems...");
	return true;
}

void QuadController::UpdateInput()
{
	if (!SerialHelper::Avaliable())
		return;

	char input = SerialHelper::GetInput();

	if (input == 'Z')
	{
		_sensors->ZeroAll();
		SerialHelper::Println("MPU zeroed");
	}
	else if (input == '1')
	{
		_motorMgr->SetBaseSpeed(_motorMgr->GetBaseSpeed() + 100);
		SerialHelper::Println(_motorMgr->GetBaseSpeed());
	}
	else if (input == '2')
	{
		_motorMgr->SetBaseSpeed(_motorMgr->GetBaseSpeed() - 100);
		SerialHelper::Println(_motorMgr->GetBaseSpeed());
	}
	else if (input == 'a')
	{
		_motorMgr->SetBaseSpeed(_motorMgr->GetBaseSpeed() + 20);
		SerialHelper::Println(_motorMgr->GetBaseSpeed());
	}
	else if (input == 'b')
	{
		_motorMgr->SetBaseSpeed(_motorMgr->GetBaseSpeed() - 20);
		SerialHelper::Println(_motorMgr->GetBaseSpeed());
	}
	else if (input == 'V')
	{
		_sensors->GetMPU()->PrintDebugStr();
	}
	else if (input == 'T')
	{
		_motorMgr->ToggleTestMode();
		SerialHelper::Println(_motorMgr->GetTestMode() ? "In test mode" : "Not in test mode");
	}
	else if (input == 'R')
	{
		_run = !_run;
		if (!_run)
			_motorMgr->StopAll();
	}
	else if (input == '3')
	{
		_motorMgr->IncreaseP();
	}
	else if (input == '4')
	{
		_motorMgr->DecreaseP();
	}
	else if (input == '5')
	{
		_motorMgr->IncreaseI();
	}
	else if (input == '6')
	{
		_motorMgr->DecreaseI();
	}
	else if (input == '7')
	{
		_motorMgr->IncreaseD();
	}
	else if (input == '8')
	{
		_motorMgr->DecreaseD();
	}
	else if (input == 'n')
	{
		SerialHelper::Println("NS verbose output toggled.");
		_motorMgr->SetVerboseNS(!_motorMgr->GetVerboseNS());
	}
	else if (input == 'e')
	{
		SerialHelper::Println("EW verbose output toggled.");
		_motorMgr->SetVerboseEW(!_motorMgr->GetVerboseEW());
	}
	else if (input == 'y')
	{
		SerialHelper::Println("Yaw verbose output toggled.");
		_motorMgr->SetVerboseYaw(!_motorMgr->GetVerboseYaw());
	}
	else if (input == 'p')
	{
		_motorMgr->PrintNS();
	}
	else if (input == 'o')
	{
		_motorMgr->PrintEW();
	}
}