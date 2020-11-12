#include "ObjectTracker.h"

ObjectTracker::ObjectTracker()
{
}

ObjectTracker::~ObjectTracker()
{
}

std::vector<cv::Point2f> ObjectTracker::getExceptionalPoints(cv::Mat image)
{
	std::vector<cv::Point2f> points;
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	cv::goodFeaturesToTrack(image, points, 1000, 0.01, 4);
	return points;
}

std::vector<cv::Point2f> ObjectTracker::trackObject(cv::Mat oldImage, cv::Mat currentImage, std::vector<cv::Point2f> oldPoints)
{
	std::vector<cv::Point2f> newPoints;
	if (oldPoints.size() > 0)
	{
		cv::cvtColor(oldImage, oldImage, cv::COLOR_BGR2GRAY);
		cv::cvtColor(currentImage, currentImage, cv::COLOR_BGR2GRAY);
		std::vector<uchar> status;
		std::vector<float> error;
		cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);
		cv::calcOpticalFlowPyrLK(oldImage, currentImage, oldPoints, newPoints, status, error);
	}
	return newPoints;
}

void ObjectTracker::drawPoints(cv::Mat image, std::vector<cv::Point2f> points, cv::Scalar color)
{
	for (int i = 0; i < points.size();	 i++)
	{
		cv::circle(image, points.at(i), 4, color,-1);

	}
}

void ObjectTracker::track(cv::Mat cameraFrame)
{
	cameraFrame.copyTo(currentFrame);
	if (pointsToTrack.size() == 0)
	{
		pointsToTrack = getExceptionalPoints(currentFrame);
		cameraFrame.copyTo(previousFrame);
	}
	else {
		pointsToTrack = trackObject(previousFrame, currentFrame, pointsToTrack);
		cameraFrame.copyTo(previousFrame);
		drawPoints(cameraFrame, pointsToTrack, cv::Scalar(0, 255, 0));
	}
}

int ObjectTracker::getNumOfDownCars()
{
	int result = 0;
	for (Car car : cars)
		if (car.isDirectionSet() && !car.goesUp())
			result++;
	return result;
}

void ObjectTracker::trackBB(std::vector<cv::Rect> boxes)
{
	updateBBs(boxes);
	std::vector < cv::Rect> trueCurrentBoxes = getOverlapingBBsWithPastBBs();
	updateCars(trueCurrentBoxes);

}

void ObjectTracker::updateBBs(std::vector<cv::Rect> newBoxes)
{
	//delete subprev?
	subpreviousBBs = previousBBs;
	previousBBs = currentBBs;
	currentBBs = newBoxes;
}

std::vector<cv::Rect> ObjectTracker::getOverlapingBBsWithPastBBs()
{
	return std::vector<cv::Rect>();
}

void ObjectTracker::updateCars(std::vector<cv::Rect> trueCurrentBoxes)
{
	for (Car car : cars) {
		updateCar(car);
	}
	addNewCars(trueCurrentBoxes);
}

void ObjectTracker::updateCar(Car car)
{
}

void ObjectTracker::addNewCars(std::vector<cv::Rect> trueCurrentBoxes)
{
}
