#include "Engine.h"

//Prumerna rychlost automobilu.

Engine::Engine()
{
	videoCapture = cv::VideoCapture("C:\\src\\video.mp4");
	if (!videoCapture.isOpened()) {
		std::cerr << "Video not openned\n";
	}
	std::cout << "Checkpoint1";
	windowSize = cv::Size(640, 480);
	history = nullptr;
	double fpsD = 25 / (1 + skipped);
	int fps = std::round(fpsD);
	objectTracker = new ObjectTracker(fps);
}

Engine::~Engine()
{
	if (history != nullptr) delete history;
	delete objectTracker;
}

void Engine::run()
{
	captureCameraFrame();
	char c = cv::waitKey(1);
	while(c != 27 && !cameraFrame.empty()) {
		float cropH = 0.6F;
		cv::Rect myROI(0, windowSize.height*cropH, windowSize.width, windowSize.height*(1-cropH));
		cv::Mat cropped = cameraFrame(myROI);
		createOrUpdateHistory(cropped);
		std::vector<cv::Rect> obj = objectIsolator.getObjects(cropped);
		cv::Mat activeCars;
		cropped.copyTo(activeCars);
		cv::Scalar blueColor = cv::Scalar(255, 0, 0);
		objectTracker->trackBB(obj, history);
		cv::Scalar whiteColor = cv::Scalar(255, 255, 255);
		std::vector<cv::Rect> whiteActiveCars = objectTracker->getActiveWhiteCars();
		drawBB(obj, cropped, blueColor);
		drawBB(whiteActiveCars, cropped, whiteColor);
		std::cout << " Active cars: " + std::to_string(objectTracker->getBBsOfActiveCars().size());
		drawBB(objectTracker->getBBsOfActiveCars(), activeCars,blueColor);
		//cv::imshow("Active cars", activeCars);
		
		showResult();
		skip(skipped);
		captureCameraFrame();
		if (c == 115) skip(20);
		c = cv::waitKey(1);
	}
	videoCapture.release();
	cv::destroyAllWindows();
}

void Engine::captureCameraFrame()
{
	videoCapture >> cameraFrame;
	if (!cameraFrame.empty())
	{
		cv::flip(cameraFrame, cameraFrame, 1);
		cv::resize(cameraFrame, cameraFrame, windowSize);
	}
	else {
		std::cout << std::endl << "End of video." << std::endl ;
	}
}

void Engine::drawBB(cv::Rect rect,cv::Mat image,cv::Scalar color)
{
	cv::rectangle(image, rect.tl(), rect.br(),color, 5);
}

void Engine::drawBB(std::vector<cv::Rect> obj,cv::Mat image, cv::Scalar color)
{
	for (cv::Rect x : obj) {
		drawBB(x,image,color);
	}
}

void Engine::createOrUpdateHistory(cv::Mat currentImage)
{
	if (history != nullptr)
	{
		history->update(currentImage);
	}
	else {
		history = new FrameHistory(currentImage);
	}
}

void Engine::skip(int howManyFrames)
{
	for (int i = 0; i < howManyFrames; i++)
		captureCameraFrame();
}

void Engine::showResult()
{
	std::vector<std::string> lines = std::vector<std::string>();
	cv::Scalar textColor = cv::Scalar(0, 204, 255);
	lines.push_back(std::string("Rychlost: ") + std::to_string(objectTracker->getAverageSpeed()).substr(0, 5) + " km/h");
	lines.push_back(std::string("Lesnicka: ") + std::to_string(objectTracker->getCarsGoingUp()));
	lines.push_back(std::string("Pionyrska: ") + std::to_string(objectTracker->getCarsGoingDown()));
	lines.push_back("Bila: " + std::to_string(objectTracker->getWhiteCarsCount()));
	putLines(cameraFrame, lines, cv::Point(10, 30), cv::FONT_HERSHEY_DUPLEX, 1.0, textColor, 2);
	cv::imshow("Camera frame with isolation of objects", cameraFrame);
}

void Engine::putLines(cv::Mat frame, std::vector<std::string> lines, cv::Point start, int fontFace, double fontScale, cv::Scalar color, int thickness)
{
	for (int i = 0; i < lines.size(); i++) {
		std::string line = lines.at(i);
		cv::putText(frame, line, cv::Point(start.x,start.y + i*30), fontFace, fontScale, color, thickness);
	}
}


