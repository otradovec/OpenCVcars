#include "Car.h"

Car::Car(cv::Rect bb)
{
	lastBB = bb;
	s_lastId = s_lastId + 1;
	m_id = s_lastId;
	m_colorSetted = false;
	m_isWhite = false;
	m_directionSet = false;
	m_goesUp = false;
	m_exited = false;
}

Car::~Car()
{
}

bool Car::isColorSet()
{
	return m_colorSetted;
}

bool Car::isWhite()
{
	if (!m_colorSetted)
	{
		throw "Getting not processed color";
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
		throw "Getting not processed direction";
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

void Car::setProcessedColor(bool isColorSet)
{
	if (m_colorSetted)
	{
		throw "Color already processed";
	}
	else
	{
		m_colorSetted = isColorSet;
	}
	
}

void Car::setWhite(bool isWhite)
{
	if (m_colorSetted)
	{
		throw "Setting color that was already processed";
	}
	m_isWhite = isWhite;
}

void Car::setProcessedDirection(bool isDirectionSet)
{
	if (m_directionSet) {
		throw "Already processed setted for a second time";
	}
	m_directionSet = isDirectionSet;
}

void Car::setGoesUp(bool goesUp)
{
	if (m_directionSet) {
		throw "Setting direction, but was already setted.";
	}
	m_goesUp = goesUp;
}

void Car::setExited(bool exited)
{
	m_exited = exited;
}
