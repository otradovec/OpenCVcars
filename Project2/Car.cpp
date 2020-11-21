#include "Car.h"
long Car::s_lastId = 0;
Car::Car(cv::Rect bb)
{
	m_lastBB = bb;
	s_lastId = s_lastId + 1;
	m_id = s_lastId;
	m_colorSetted = false;
	m_isWhite = false;
	m_directionSet = false;
	m_goesUp = false;
	m_exited = false;
	m_speed = 0;
	std::cout << std::endl << " Created car id: " + std::to_string(m_id);
}

Car::~Car()
{
}

cv::Rect Car::getBB()
{
	return m_lastBB;
}

cv::Rect Car::getprevBB()
{
	return m_prevBB;
}

bool Car::isColorSet()
{
	return m_colorSetted;
}

bool Car::isWhite()
{
	if (!m_colorSetted)
	{
		throw std::logic_error("Getting not processed color");
	}
	return m_isWhite;
}

bool Car::isDirectionSet()
{
	return m_directionSet;
}

bool Car::goesUp()
{
	if (!m_directionSet)
	{
		throw std::logic_error("Getting not processed direction");
	}
	return m_goesUp;
}

bool Car::exited()
{
	return m_exited;
}

long Car::getId()
{
	return m_id;
}

double Car::getSpeed()
{
	return m_speed;
}

void Car::setBB(cv::Rect lastBB)
{
	if (lastBB.area() < 1) throw std::invalid_argument("Setting bb without area");
	m_prevBB = m_lastBB;
	m_lastBB = lastBB;
}

void Car::setWhite(bool isWhite)
{
	if (m_colorSetted)
	{
		throw std::logic_error("Setting color that was already processed");
	}
	m_isWhite = isWhite;
	m_colorSetted = true;
}

void Car::setGoesUp(bool goesUp)
{
	if (m_directionSet) {
		throw std::logic_error("Setting direction, but was already setted.");
	}
	m_goesUp = goesUp;
	m_directionSet = true;
}

void Car::setExited(bool exited)
{
	if (m_exited && !exited) throw std::logic_error("Car comeback");
	m_exited = exited;
}

void Car::setSpeed(double speed)
{
	if (m_speed > 0.0) throw new std::logic_error("Speed already set.");
	m_speed = speed;
}

void Car::print()
{
	cout << "Car id: " + to_string(this->m_id)
		+ ", isDirectionSet: " + to_string(this->m_directionSet) + ", goes up: " + to_string(this->m_goesUp)
		+ ", is color set: " + to_string(this->m_colorSetted) + ", is white: " + to_string(this->m_isWhite)
		+ "Speed: " + to_string(this->m_speed) << std::endl;
}
