#pragma once
#include <opencv2/opencv.hpp>
#include "Car.h"
#include "FrameHistory.h"

class ColorTracker
{
public:
	ColorTracker();
	~ColorTracker();
	cv::Point getColorPosition(cv::Mat image);
	bool isWhiteCar(cv::Mat cut);
private:
	cv::Mat preprocessImage(cv::Mat image);
	double getGreatestArea(cv::Mat blackAndWhiteImage);
};

