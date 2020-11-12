#pragma once
#include <stdio.h>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui.hpp"
#include "CarDetector.h"
#include "ColorTracker.h"
#include "ObjectTracker.h"
#include "ObjectIsolator.h"
class Engine {
public:
	Engine();
	void run();

private:
	cv::VideoCapture videoCapture;
	cv::Mat cameraFrame;
	cv::Size windowSize;
	void captureCameraFrame();
	CarDetector carDetector;
	void drawBB(cv::Rect rect, cv::Mat image);
	void drawBB(std::vector<cv::Rect> obj, cv::Mat image);
	ColorTracker colorTracker;
	ObjectTracker objectTracker;
	cv::Mat currentFrame;
	cv::Mat previousFrame;
	std::vector<cv::Point2f> pointsToTrack;
	ObjectIsolator objectIsolator;

};

