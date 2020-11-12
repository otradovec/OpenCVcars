#include "ObjectIsolator.h"



ObjectIsolator::ObjectIsolator()
{
	backgroundSubtractor = cv::createBackgroundSubtractorMOG2();
}


std::vector<cv::Rect> ObjectIsolator::getObjects(cv::Mat image)
{
	backgroundSubtractor->apply(image, image);
	int threshold_value = 200;
	int	max_binary_value = 255;
	int tresholdType = 0;
	cv::threshold(image, image, threshold_value, max_binary_value, tresholdType);
	//cv::imshow("before erosion", image);
	int erosion_size = 2;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1));
	cv::erode(image, image, element);
	int dilateSize = 3;
	//cv::imshow("before dilate", image);
	cv::Mat dilElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * dilateSize + 1, 2 * dilateSize + 1));
	cv::dilate(image, image, dilElement);
	//cv::imshow("before erosion 2", image);
	erosion_size = 9;
	element = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1));
	cv::erode(image, image, element);
	dilateSize = 15;
	//cv::imshow("before dilate 2", image);
	dilElement = cv::getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * dilateSize + 1, 2 * dilateSize + 1));
	cv::dilate(image, image, dilElement);
	cv::imshow("after dilate 2", image);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0.0));
	double minArea = 3000;
	std::vector<std::vector<cv::Point>> processedContours;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours.at(i));
		if (area >= minArea)
		{
			processedContours.push_back(contours.at(i));
		}
	}
	std::vector<cv::Rect> obj;
	if (!processedContours.empty())
	{
		for (int i = 0; i < processedContours.size(); i++)
		{
			std::vector<cv::Point> polygon;
			cv::approxPolyDP(processedContours.at(i), polygon, 5, true);
			cv::Rect boundingBox = cv::boundingRect(polygon);
			obj.push_back(boundingBox);
		}
	}
	return obj;
}
