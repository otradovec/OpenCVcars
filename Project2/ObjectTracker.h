#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Car.h"
class ObjectTracker
{
private:
	cv::Mat currentFrame;
	cv::Mat previousFrame;
	std::vector<cv::Point2f> pointsToTrack;
	std::vector<cv::Rect> subpreviousBBs;
	std::vector<cv::Rect> previousBBs;
	std::vector<cv::Rect> currentBBs;
	std::vector<Car*> cars;
public:
	ObjectTracker();
	~ObjectTracker();
	std::vector<cv::Point2f> getExceptionalPoints(cv::Mat image);
	std::vector<cv::Point2f> trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints);
	void drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color);
	void track(cv::Mat frame);
	int getCarsGoingUp();
	int getCarsGoingDown();
	void trackBB(std::vector < cv::Rect> boxes);
	std::vector<cv::Rect> getBBsOfActiveCars();
private:
	void updateBBs(std::vector < cv::Rect> newBoxes);
	void updateCars();
	void updateCar(Car* car);
	void updateDirection(Car* car);
	bool isGoingUp(cv::Rect currentBB, cv::Rect previousBB);
	void addNewCars();
	std::vector<cv::Rect> getCurrentBBsNotOverlappingWithAnyCar();
	bool isOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs);
	bool isOverlapping(cv::Rect first, cv::Rect second);
	bool present(Car* car, std::vector<cv::Rect> currentBBs);
	void updateBB(Car* car);
	void addCars(std::vector<cv::Rect> bbs);
	cv::Rect getBBClosestOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs);
	std::vector<cv::Rect> getOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs);
	cv::Rect getClosest(cv::Rect bb, std::vector<cv::Rect> bbs);
	bool equalsBBs(cv::Rect first, cv::Rect second);
	cv::Point getCenter(cv::Rect rect);
	long getDistance(cv::Point first,cv::Point second);
};

