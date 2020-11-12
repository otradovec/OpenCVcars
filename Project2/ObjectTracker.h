#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>

class ObjectTracker
{
private:
	int upCarsOutOfSight;
	int downCarsOutOfSight;
	cv::Mat currentFrame;
	cv::Mat previousFrame;
	std::vector<cv::Point2f> pointsToTrack;
public:
	ObjectTracker();
	~ObjectTracker();
	std::vector<cv::Point2f> getExceptionalPoints(cv::Mat image);
	std::vector<cv::Point2f> trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints);
	void drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color);
	void track(cv::Mat frame);
};

