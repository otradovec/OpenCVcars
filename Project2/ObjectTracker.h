#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>

class ObjectTracker
{
public:
	ObjectTracker();
	~ObjectTracker();
	std::vector<cv::Point2f> getFeatures(cv::Mat image);
	std::vector<cv::Point2f> trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints);
	void drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color);
};

