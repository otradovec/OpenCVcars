#pragma once
#include "Car.h"

class SpeedTracker
{
	int m_fps;
	int m_pxInCrop;
	double  m_realCropLengthInMetres;
public:
	SpeedTracker(int fps);
	~SpeedTracker();
	void updateSpeed(Car* car);
	double getAverageSpeed(std::vector<Car*> cars);
private:
	void setSpeed(Car* car, double speed);
	double calculateSpeed(Car* car);
	double getSpeed(int speedInPx);
	double getSpeedMps(int speedInPx);
};

