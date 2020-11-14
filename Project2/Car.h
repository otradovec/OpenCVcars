#pragma once
#include <opencv2/opencv.hpp>

class Car
{
private:
	cv::Rect m_lastBB;
	bool m_colorSetted;
	bool m_isWhite;
	bool m_directionSet;
	bool m_goesUp;
	bool m_exited;
	long m_id;
	long static s_lastId;
public:
	Car(cv::Rect bb);
	~Car();
	cv::Rect getBB();
	bool isColorSet();
	bool isWhite();
	bool isDirectionSet();
	bool goesUp();
	bool exited();
	long getId();
	void setBB(cv::Rect lastBB);
	void setWhite(bool isWhite);
	void setGoesUp(bool goesUp);
	void setExited(bool exited);

};

