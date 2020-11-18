#pragma once
#include <stdio.h>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/highgui.hpp"
#include "FrameHistory.h"
#include "ObjectTracker.h"
#include "ObjectIsolator.h"

class Engine {
public:
	Engine();
	~Engine();
	void run();
	
private:
	FrameHistory* history;
	cv::VideoCapture videoCapture;
	cv::Mat cameraFrame;
	cv::Size windowSize;
	void captureCameraFrame();
	void drawBB(cv::Rect rect, cv::Mat image, cv::Scalar color);
	void drawBB(std::vector<cv::Rect> obj, cv::Mat image, cv::Scalar color);
	ColorTracker colorTracker;
	ObjectTracker objectTracker;
	ObjectIsolator objectIsolator;
	void createOrUpdateHistory(cv::Mat currentImage);
	void skip(int howManyFrames);
};

