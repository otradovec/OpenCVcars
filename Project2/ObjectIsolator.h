#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

class ObjectIsolator
{
private:
	cv::Ptr<cv::BackgroundSubtractor> backgroundSubtractor;
public:
	ObjectIsolator();
	std::vector<cv::Rect> getObjects(cv::Mat image);

};

