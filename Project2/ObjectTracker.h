#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "Car.h"
#include "ColorTracker.h"
#include "SpeedTracker.h"

enum DistanceTypes{Normal,FavouringUp,FavouringDown};

class ObjectTracker
{
private:
	std::vector<cv::Point2f> pointsToTrack;
	std::vector<cv::Rect> subpreviousBBs;
	std::vector<cv::Rect> previousBBs;
	std::vector<cv::Rect> currentBBs;
	std::vector<Car*> cars;
	ColorTracker* colorTracker;
	SpeedTracker* speedTracker;
public:
	ObjectTracker(int fps);
	~ObjectTracker();
	std::vector<cv::Point2f> getExceptionalPoints(cv::Mat image);
	std::vector<cv::Point2f> trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints);
	void drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color);
	int getCarsGoingUp();
	int getCarsGoingDown();
	void trackBB(std::vector < cv::Rect> boxes, FrameHistory* frameHistory);
	std::vector<cv::Rect> getBBsOfActiveCars();
	int getWhiteCarsCount();
	std::vector<cv::Rect> getActiveWhiteCars();
private:
	void updateBBs(std::vector < cv::Rect> newBoxes);
	void updateCars(FrameHistory* frameHistory);
	void updateCar(Car* car, FrameHistory* frameHistory);
	void updateColor(Car* car, FrameHistory* frameHistory);
	cv::Mat getImageCut(cv::Rect box, cv::Mat originalImage);
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
	cv::Rect getClosest(cv::Rect bb, std::vector<cv::Rect> bbs, DistanceTypes distanceType);
	bool equalsBBs(cv::Rect first, cv::Rect second);
	cv::Point getCenter(cv::Rect rect);
	double getDistance(cv::Point first,cv::Point second);
	cv::Rect getBBbestOverlapping(Car* car, std::vector<cv::Rect> bbs);
	cv::Rect getBestClose(cv::Rect carbb, std::vector<cv::Rect> overlapping, bool goingUp);
};

