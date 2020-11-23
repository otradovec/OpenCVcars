#include "ObjectTracker.h"

ObjectTracker::ObjectTracker(int fps)
{
	currentBBs = std::vector<cv::Rect>();
	previousBBs = std::vector<cv::Rect>();
	subpreviousBBs = std::vector<cv::Rect>();
	colorTracker = new ColorTracker();
	speedTracker = new SpeedTracker(fps);
}

ObjectTracker::~ObjectTracker()
{
	for (Car* car : cars) { delete car; }
	delete colorTracker;
	delete speedTracker;
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

void ObjectTracker::trackBB(std::vector<cv::Rect> boxes, FrameHistory* frameHistory)
{
	updateBBs(boxes);
	updateCars(frameHistory);
}

void ObjectTracker::updateBBs(std::vector<cv::Rect> newBoxes)
{
	subpreviousBBs = previousBBs;
	previousBBs = currentBBs;
	currentBBs = newBoxes;
}

void ObjectTracker::updateCars(FrameHistory* frameHistory)
{
	for (Car* car : cars) {
		updateCar(car,frameHistory);
	}
	addNewCars();
}

void ObjectTracker::updateCar(Car* car, FrameHistory* frameHistory)
{
	if (!car->exited())
	{
		if (!present(car,currentBBs) && !present(car, previousBBs))
		{
			car->setExited(true);
			std::cout << " Exited car: " + std::to_string(car->getId()) << std::endl;
			car->print();
		}
		else
		{
			updateBB(car);
			updateColor(car,frameHistory);
			speedTracker->updateSpeed(car);
			updateDirection(car);
		}
	}
}

void ObjectTracker::updateColor(Car * car, FrameHistory* frameHistory)
{
	if (!car->isColorSet()) {
		cv::Mat current = frameHistory->getCurrent();
		if (current.empty()) throw new std::runtime_error("Empty image");
		cv::Mat imageCut = getImageCut(car->getBB(), current);
		car->setWhite(colorTracker->isWhiteCar(imageCut));
	}
}

cv::Mat ObjectTracker::getImageCut(cv::Rect box, cv::Mat originalImage)
{
	if (box.area()<1) throw "Seems bad box";
	if (originalImage.empty()) throw "Empty image input";
	return originalImage(box);
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
	cv::Rect newBB = getBBbestOverlapping(car, currentBBs);
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
	return getClosest(bb,over,DistanceTypes::Normal);
}

std::vector<cv::Rect> ObjectTracker::getOverlapping(cv::Rect bb, std::vector<cv::Rect> bbs)
{
	std::vector<cv::Rect> result;
	for (cv::Rect box : bbs) {
		if (isOverlapping(bb,box)) result.push_back(box);
	}
	return result;
}

cv::Rect ObjectTracker::getClosest(cv::Rect mainBB, std::vector<cv::Rect> bbs, DistanceTypes distanceType)
{
	cv::Rect closest;
	double smallestDistance = DBL_MAX;
	cv::Point mainBBCenter = getCenter(mainBB);
	cv::Point otherBBCenter;
	for (cv::Rect box : bbs) {
		otherBBCenter = getCenter(box);
		double idistance = getDistance(mainBBCenter, otherBBCenter);
		if (distanceType = DistanceTypes::FavouringUp) {
			if (mainBBCenter.y - otherBBCenter.y > 0)
				idistance = 0.5*idistance;
			else
				idistance = 2 * idistance;
		}
		if (distanceType = DistanceTypes::FavouringDown) {
			if (mainBBCenter.y - otherBBCenter.y < 0)
				idistance = 0.5*idistance;
			else
				idistance = 2 * idistance;
		}
		if (idistance < smallestDistance) {
			smallestDistance = idistance;
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
	return cv::Point((2*rect.x + rect.width) / 2, (2*rect.y + rect.height) / 2);
}

double ObjectTracker::getDistance(cv::Point first, cv::Point second)
{
	cv::Point diff = first - second;
	return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}

cv::Rect ObjectTracker::getBBbestOverlapping(Car * car, std::vector<cv::Rect> bbs)
{
	cv::Rect carbb = car->getBB();
	std::vector<cv::Rect> over = getOverlapping(carbb, bbs);
	if (car->isDirectionSet()) {
		return getBestClose(carbb, over, car->goesUp());
	}
	else {
		return getClosest(carbb, over,DistanceTypes::Normal);
	}
}

cv::Rect ObjectTracker::getBestClose(cv::Rect carbb, std::vector<cv::Rect> overlapping, bool goingUp)
{
	if (goingUp)
		return getClosest(carbb, overlapping, DistanceTypes::FavouringUp);
	else
		return getClosest(carbb, overlapping, DistanceTypes::FavouringDown);
}





std::vector<cv::Rect> ObjectTracker::getActiveWhiteCars()
{
	std::vector<cv::Rect> whiteActiveCars;
	for (Car* car : cars) {
		if (car->isColorSet() && car->isWhite() && !car->exited()) 
			whiteActiveCars.push_back(car->getBB());
	}
	return whiteActiveCars;
}

double ObjectTracker::getAverageSpeed()
{
	return speedTracker->getAverageSpeed(cars);
}

int ObjectTracker::getWhiteCarsCount()
{
	int whiteCars = 0;
	for (Car* car : cars) {
		if (car->isColorSet() && car->isWhite()) whiteCars++;
	}
	return whiteCars;
}
