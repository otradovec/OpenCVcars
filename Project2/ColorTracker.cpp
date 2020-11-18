#include "ColorTracker.h"

ColorTracker::ColorTracker()
{
}

ColorTracker::~ColorTracker()
{
}

cv::Mat ColorTracker::preprocessImage(cv::Mat image)
{
	cv::Mat preprocessedImage;
	//image.copyTo(preprocessedImage);
	cv::Mat hsvImage;
	cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
	cv::imshow("HSV", hsvImage);
	//hsvImage.copyTo(preprocessedImage);
	int hMax = 360 / 2;
	int vMax = 255;
	int sensitivity = 15;
	cv::inRange(hsvImage, cv::Scalar(0, 0,vMax-sensitivity), cv::Scalar(hMax, vMax,vMax), preprocessedImage);
	cv::imshow("In range", preprocessedImage);
	///Operation open
	cv::erode(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2, 2)));
	cv::dilate(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
	cv::erode(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
	cv::dilate(preprocessedImage, preprocessedImage, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(6, 6)));
	return preprocessedImage;
}

cv::Point ColorTracker::getColorPosition(cv::Mat image)
{
	cv::Point position = cv::Point(0,0);
	cv::Mat preprocessedImage = preprocessImage(image);
	cv::imshow("Preprocessed image", preprocessedImage);
	std::vector<std::vector<cv::Point>> contours;
	///param hierarchy is for hierarchy what is inside of other
	std::vector<cv::Vec4i> hierarchy;
	///input image must be black and white
	cv::findContours(preprocessedImage, contours, hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point(0.0));
	double largestArea = 0;
	int largestAreaIndex = -1;
	for ( int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours.at(i));
		if (area > largestArea)
		{
			largestArea = area;
			largestAreaIndex = i;
		}
	}
	if (largestAreaIndex != -1 && largestArea > 50)
	{
		std::vector<cv::Point> polygon;
		cv::approxPolyDP(contours.at(largestAreaIndex), polygon, 5, true);
		cv::Rect boundingBox = cv::boundingRect(polygon);
		position = cv::Point((2*boundingBox.x + boundingBox.width) / 2, (2*boundingBox.y + boundingBox.height) / 2);
	}
	cv::drawContours(image, contours, -1, (0, 255, 0), 3);

	return position;
}

bool ColorTracker::isWhiteCar(cv::Mat cut)
{
	cv::Mat preprocessedImage = preprocessImage(cut);
	double greatestArea = getGreatestArea(preprocessedImage);
	std::cout << "Area size: " + std::to_string(greatestArea) << std::endl;
	return greatestArea > 800;
}

double ColorTracker::getGreatestArea(cv::Mat blackAndWhiteImage)
{
	std::vector<std::vector<cv::Point>> contours;
	///param hierarchy is for hierarchy what is inside of other
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(blackAndWhiteImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0.0));
	double largestArea = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours.at(i));
		if (area > largestArea)
		{
			largestArea = area;
		}
	}
	cv::drawContours(blackAndWhiteImage, contours, -1, (0, 255, 0), 3);
	return largestArea;
}