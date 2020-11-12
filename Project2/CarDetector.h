#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>

class CarDetector {

private:
	cv::CascadeClassifier classifier;

public:
	CarDetector();
	std::vector<cv::Rect> getCars(cv::Mat image);

};

