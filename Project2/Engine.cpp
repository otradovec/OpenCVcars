#include "Engine.h"

//Celkovy pocet auta, ktere projeli smerem k?ulici Pionyrska.
//Celkovy pocet auta, ktere projeli smerem k?ulici Lesnicka
//Celkovy pocet bilych aut.
//Prumerna rychlost automobilu.
//Ve chvili, kdy je dopravni prostredek rozeznam zobrazi se kolem nej ramecek

Engine::Engine()
{
	videoCapture = cv::VideoCapture("C:\\src\\video.mp4");
	if (!videoCapture.isOpened()) {
		std::cerr << "Video not openned\n";
	}
	std::cout << "Checkpoint1";
	windowSize = cv::Size(640, 480);
}

void Engine::run()
{
	char c = cv::waitKey(1);
	while(c != 27) {
		captureCameraFrame();
		captureCameraFrame();
		captureCameraFrame();

		float cropH = 0.6;
		cv::Rect myROI(0, windowSize.height*cropH, windowSize.width, windowSize.height*(1-cropH));
		cv::Mat cropped = cameraFrame(myROI);
		std::vector<cv::Rect> obj = objectIsolator.getObjects(cropped);
		drawBB(obj, cropped);
		/*
		cv::Point colorPosition = colorTracker.getColorPosition(cropped);
		cv::circle(cropped, colorPosition, 15, cv::Scalar(122, 122, 122), -1);
		*/
		objectTracker.trackBB(obj);

		std::string textString = std::string("Pionyrska: ") + std::to_string(objectTracker.getNumOfDownCars()) + " Lesnicka: " + "Bila: ";
		cv::putText(cameraFrame,textString, cv::Point(10, 50), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(118, 185, 0), 2);
		cv::imshow("Camera stream", cameraFrame);
		c = cv::waitKey(1);
	}
	videoCapture.release();
	cv::destroyAllWindows();

}

void Engine::captureCameraFrame()
{
	videoCapture >> cameraFrame;
	cv::flip(cameraFrame, cameraFrame, 1);
	cv::resize(cameraFrame, cameraFrame, windowSize);
}

void Engine::drawBB(cv::Rect rect,cv::Mat image)
{
	cv::rectangle(image, rect.tl(), rect.br(), cv::Scalar(255, 0, 0), 5);
}

void Engine::drawBB(std::vector<cv::Rect> obj,cv::Mat image)
{
	for (cv::Rect x : obj) {
		drawBB(x,image);
	}
}


