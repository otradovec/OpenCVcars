#include "SpeedTracker.h"



SpeedTracker::SpeedTracker(int fps)
{
	m_fps = fps;
	m_realCropLengthInMetres = 15;
	m_pxInCrop = 191;
}


SpeedTracker::~SpeedTracker()
{
}

void SpeedTracker::updateSpeed(Car * car)
{
	if ((car->getSpeed() < 0.001)  && car->isDirectionSet()){
		double newSpeed = calculateSpeed(car);
		if (newSpeed > 0.001) {
			this->setSpeed(car, newSpeed);
		}
	}
}

double SpeedTracker::getAverageSpeed(std::vector<Car*> cars)
{
	double sum = 0;
	int count = 0;
	for (Car* car : cars) {
		double speed = car->getSpeed();
		if (speed > 0.0) {
			sum = sum + speed;
			count++;
		}			
	}
	if (count > 0)
		return sum / (double)count;
	else
		return 0.0;
}

void SpeedTracker::setSpeed(Car * car, double speed)
{
	car->setSpeed(speed);
}

double SpeedTracker::calculateSpeed(Car * car)
{
	if (car->goesUp()) {
		int speedInPx = car->getprevBB().tl().y - car->getBB().tl().y;
		return getSpeed(speedInPx);
	}
	else
	{
		int speedInPx = car->getBB().br().y - car->getprevBB().br().y;
		return getSpeed(speedInPx);
	}
}

double SpeedTracker::getSpeed(int speedInPx)
{
	return 3.6 * getSpeedMps(speedInPx);
}

double SpeedTracker::getSpeedMps(int speedInPx)
{
	return ((double)speedInPx / m_pxInCrop) * m_realCropLengthInMetres * m_fps;
}
