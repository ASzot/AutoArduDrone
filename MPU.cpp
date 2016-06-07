#include "MPU.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

MPU6050 _mpu;

volatile bool g_mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void g_fdmpGetData() 
{
    g_mpuInterrupt = true;
}

MPU::MPU() 
{
	_dmpReady = false;
	_calibrationYPR = new double[3];
	for (int i = 0; i < 3; ++i)
		_calibrationYPR[i] = 0.0;

	ShowAll = false;
}

bool MPU::Init()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    _mpu.initialize();
	
    _mpu.setXGyroOffset(220);
    _mpu.setYGyroOffset(76);
    _mpu.setZGyroOffset(-85);

    _mpu.initialize();
	if (!_mpu.testConnection())
		return false;

	uint8_t devStatus = _mpu.dmpInitialize();

	if (devStatus == 0)
	{
        _mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
		attachInterrupt(0, g_fdmpGetData, RISING);
        _mpuStatus = _mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        _dmpReady = true;

        // get expected DMP packet size for later comparison
        _expectedPacketSize = _mpu.dmpGetFIFOPacketSize();
	}
	else 
	{
		return false;
	}

	_calibrationYPR[0] = 0;
	_calibrationYPR[1] = 0;
	_calibrationYPR[2] = 0;

	_lastYaw = YPR[0];

	return true;
}

// Get new data
void MPU::Update()
{		
	double *ypr = CalculateYPR();
	for (int i = 0; i < 3; ++i)
	{
		YPR[i] = ypr[i] - _calibrationYPR[i];
	}
}

double* MPU::CalculateYPR() 
{
    if (!_dmpReady) 
		return new double[3];
	
    // wait for MPU interrupt or extra packet(s) available
    while (!g_mpuInterrupt && _fifoCount < _expectedPacketSize) 
	{

    }
	
    g_mpuInterrupt = false;
    _mpuStatus = _mpu.getIntStatus();

	// get current FIFO count
    _fifoCount = _mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((_mpuStatus & 0x10) || _fifoCount == 1024) 
	{
        // reset so we can continue cleanly
        _mpu.resetFIFO();
    } 
	else if (_mpuStatus & 0x02) 
	{
        // wait for correct available data length, should be a VERY short wait
        while (_fifoCount < _expectedPacketSize) 
			_fifoCount = _mpu.getFIFOCount();

		uint8_t fifoBuffer[64];
        // read a packet from FIFO
        _mpu.getFIFOBytes(fifoBuffer, _expectedPacketSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        _fifoCount -= _expectedPacketSize;
		
		Quaternion q;
		float ypr[3];	
		VectorFloat gravity;

        _mpu.dmpGetQuaternion(&q, fifoBuffer);
        _mpu.dmpGetGravity(&gravity, &q);
        _mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
		

		// Convert to degrees and to doubles.
		double* finalYpr = new double[3];
		finalYpr[0] = (double)ypr[0] * (180.0 / M_PI);
		finalYpr[1] = (double)ypr[1] * (180.0 / M_PI);
		finalYpr[2] = (double)ypr[2] * (180.0 / M_PI);

		return finalYpr;
	}

	// Just return an empty array.
	return new double[3];
}

// Get tilt in degrees on the NS axis (to north = positive)
double MPU::GetNS() 
{
	return YPR[2];
}

// Get tilt in degress on the EW axis (to east = positive)
double MPU::GetEW()
{
	return YPR[1];
}

// Get yaw in degrees (clockwise = positive)
double MPU::GetYaw()
{
	return YPR[0];
}

// Print current sensor data
void MPU::PrintDebugStr()
{
	Serial.print(YPR[0]);
	Serial.print(", ");
	Serial.print(YPR[1]);
	Serial.print(", ");
	Serial.println(YPR[2]);
}

// Check if MPU data has passed allowed bounds (>30 degrees tilt)
bool MPU::CheckBeyondLimits()
{
	if(abs(GetNS()) > 30) 
	  return true;
	if(abs(GetEW()) > 30) 
	  return true;
	return false;
}

// Zero the MPU at the current orientation
void MPU::Zero()
{
	double *ypr = CalculateYPR();
	// The array needs to be copied over to avoid pointer problems. 
	_calibrationYPR = new double[3];
	for (int i = 0; i < 3; ++i)
	{
		_calibrationYPR[i] = ypr[i];
	}
	_lastYaw = _calibrationYPR[0];
}

// Zero the yaw value at the current orientation
void MPU::ZeroYaw()
{
	double* ypr = CalculateYPR();
	_calibrationYPR[0] = ypr[0];
}


