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
	std::cout << std::endl << "Created car id: " + m_id;
}

Car::~Car()
{
}

cv::Rect Car::getBB()
{
	return m_lastBB;
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

void Car::setBB(cv::Rect lastBB)
{
	if (lastBB.area() < 1) throw std::invalid_argument("Setting bb without area");
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
