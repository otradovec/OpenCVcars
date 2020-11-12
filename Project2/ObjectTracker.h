#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Car.h"
class ObjectTracker
{
private:
	int upCarsOutOfSight;
	int downCarsOutOfSight;
	cv::Mat currentFrame;
	cv::Mat previousFrame;
	std::vector<cv::Point2f> pointsToTrack;
	std::vector<cv::Rect> subpreviousBBs;
	std::vector<cv::Rect> previousBBs;
	std::vector<cv::Rect> currentBBs;
	std::vector<Car> cars;
public:
	ObjectTracker();
	~ObjectTracker();
	std::vector<cv::Point2f> getExceptionalPoints(cv::Mat image);
	std::vector<cv::Point2f> trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints);
	void drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color);
	void track(cv::Mat frame);
	int getNumOfDownCars();
	void trackBB(std::vector < cv::Rect> boxes);
};

