#pragma once
#include <opencv2/opencv.hpp>

using std::cout;
using std::to_string;

class Car
{
private:
	cv::Rect m_lastBB;
	cv::Rect m_prevBB;
	bool m_colorSetted;
	bool m_isWhite;
	bool m_directionSet;
	bool m_goesUp;
	bool m_exited;
	long m_id;
	double m_speed;
	long static s_lastId;
public:
	Car(cv::Rect bb);
	~Car();
	cv::Rect getBB();
	cv::Rect getprevBB();
	bool isColorSet();
	bool isWhite();
	bool isDirectionSet();
	bool goesUp();
	bool exited();
	long getId();
	double getSpeed();
	void setBB(cv::Rect lastBB);
	void setWhite(bool isWhite);
	void setGoesUp(bool goesUp);
	void setExited(bool exited);
	void setSpeed(double speed);
	void print();
};

