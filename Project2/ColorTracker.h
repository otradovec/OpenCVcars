#pragma once
#include <opencv2/opencv.hpp>
class ColorTracker
{
private:
	cv::Mat preprocessImage(cv::Mat image);
public:
	ColorTracker();
	~ColorTracker();
	cv::Point getColorPosition(cv::Mat image);
};

