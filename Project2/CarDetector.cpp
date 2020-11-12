#include "CarDetector.h"



CarDetector::CarDetector()
{
	classifier.load("cars.xml");
}

std::vector<cv::Rect> CarDetector::getCars(cv::Mat image)
{
	std::vector<cv::Rect> cars;
	cv::Mat grayscaleImage;
	cv::cvtColor(image, grayscaleImage, cv::COLOR_BGRA2GRAY);
	classifier.detectMultiScale(grayscaleImage, cars, 1.1, 2,0 , cv::Size(20, 20), cv::Size(150, 150));
	//classifier.detectMultiScale(grayscaleImage, cars);
	cv::putText(image, "OpenCV", cv::Point(100, 50), cv::FONT_HERSHEY_SIMPLEX, 4, (255, 255, 255), 2, cv::LINE_AA);
	return cars;
}


