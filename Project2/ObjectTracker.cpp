#include "ObjectTracker.h"

ObjectTracker::ObjectTracker()
{
	currentBBs = std::vector<cv::Rect>();
	previousBBs = std::vector<cv::Rect>();
	subpreviousBBs = std::vector<cv::Rect>();
}

ObjectTracker::~ObjectTracker()
{
	for (Car* car : cars) { delete car; }
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

int ObjectTracker::getCarsGoingUp()
{
	int carsGoingUp = 0;
	for (Car* car : cars) {
		if (car->isDirectionSet() && car->goesUp()) carsGoingUp++;
	}
	return carsGoingUp;
}

int ObjectTracker::getCarsGoingDown()
{
	int carsGoingDown = 0;
	for (Car* car : cars) {
		if (car->isDirectionSet() && !car->goesUp()) carsGoingDown++;
	}
	return carsGoingDown;
}

void ObjectTracker::trackBB(std::vector<cv::Rect> boxes)
{
	updateBBs(boxes);
	updateCars();
}

void ObjectTracker::updateBBs(std::vector<cv::Rect> newBoxes)
{
	subpreviousBBs = previousBBs;
	previousBBs = currentBBs;
	currentBBs = newBoxes;
}

void ObjectTracker::updateCars()
{
	for (Car* car : cars) {
		updateCar(car);
	}
	addNewCars();
}

void ObjectTracker::updateCar(Car* car)
{
	if (!car->exited())
	{
		if (!present(car,currentBBs) && !present(car, previousBBs))
		{
			car->setExited(true);
			std::cout << " Exited car: " + std::to_string(car->getId()) << std::endl;
		}
		else
		{
			updateBB(car);
			//updateColor(car);
			updateDirection(car);
		}
	}
}

void ObjectTracker::updateDirection(Car * car)
{
	if (!car->isDirectionSet()) {
		cv::Rect previousBB = getBBClosestOverlapping(car->getBB(), previousBBs);
		if (present(car,currentBBs) && previousBB.area()>0)
			car->setGoesUp(isGoingUp(car->getBB(),previousBB));
	}
}

bool ObjectTracker::isGoingUp(cv::Rect currentBB, cv::Rect previousBB)
{
	return (getCenter(currentBB).y - getCenter(previousBB).y) < 0;
}

void ObjectTracker::addNewCars()
{
	std::vector<cv::Rect> bbs = getCurrentBBsNotOverlappingWithAnyCar();
	addCars(bbs);
}

std::vector<cv::Rect> ObjectTracker::getCurrentBBsNotOverlappingWithAnyCar()
{
	std::vector<cv::Rect> result;
	for (cv::Rect currentBox : currentBBs) {
		if (!isOverlapping(currentBox, getBBsOfActiveCars())) result.push_back(currentBox);
	}
	return result;
}

bool ObjectTracker::isOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs)
{
	for (cv::Rect rect : bbs) {
		if (isOverlapping(rect, bb)) return true;
	}
	return false;
}

bool ObjectTracker::isOverlapping(cv::Rect first, cv::Rect second)
{
	return (first & second).area() > 0;
}

std::vector<cv::Rect> ObjectTracker::getBBsOfActiveCars()
{
	std::vector<cv::Rect> result = std::vector<cv::Rect>();
	for (Car* car : cars) {
		if (!car->exited())
		{
			result.push_back(car->getBB());
		}
	}
	return result;
}

bool ObjectTracker::present(Car* car, std::vector<cv::Rect> bbs)
{
	cv::Rect carsBB = car->getBB();
	for (cv::Rect rect : bbs) {
		if (equalsBBs(carsBB,rect))
		{
			return true;
		}
	}
	return false;
}

void ObjectTracker::updateBB(Car* car)
{
	std::cout << " Updating car: " + std::to_string( car->getId()) << std::endl;
	cv::Rect newBB = getBBClosestOverlapping(car->getBB(), currentBBs);
	if (newBB.area() > 1) {
		car->setBB(newBB);
	}
}

void ObjectTracker::addCars(std::vector<cv::Rect> bbs)
{
	for (cv::Rect box : bbs) {
		cars.push_back(new Car(box));
	}
}

cv::Rect ObjectTracker::getBBClosestOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs)
{
	std::vector<cv::Rect> over = getOverlapping(bb, bbs);
	return getClosest(bb,over);
}

std::vector<cv::Rect> ObjectTracker::getOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs)
{
	std::vector<cv::Rect> result;
	for (cv::Rect box : bbs) {
		if (isOverlapping(bb,box)) result.push_back(box);
	}
	return result;
}

cv::Rect ObjectTracker::getClosest(cv::Rect mainBB, std::vector<cv::Rect> bbs)
{
	cv::Rect closest;
	long smallestDistance = LONG_MAX;
	cv::Point mainBBCenter = getCenter(mainBB);
	cv::Point otherBBCenter;
	for (cv::Rect box : bbs) {
		otherBBCenter = getCenter(box);
		if (getDistance(mainBBCenter, otherBBCenter) < smallestDistance) {
			smallestDistance = getDistance(mainBBCenter, otherBBCenter);
			closest = box;
		}
	}
	return closest;
}

bool ObjectTracker::equalsBBs(cv::Rect first, cv::Rect second)
{
	return (first.area() == second.area()) && (first.x == second.x);
}

cv::Point ObjectTracker::getCenter(cv::Rect rect)
{
	return cv::Point((rect.x + rect.width) / 2, (rect.y + rect.height) / 2);
}

long ObjectTracker::getDistance(cv::Point first, cv::Point second)
{
	cv::Point diff = first - second;
	return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}
