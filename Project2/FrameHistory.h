#pragma once
#include <opencv2/core/mat.hpp>

class FrameHistory
{
	cv::Mat m_currentFrame;
	cv::Mat m_previousFrame;
public:
	FrameHistory(cv::Mat currentFrame);
	cv::Mat getCurrent();
	cv::Mat getPrevious();
	void update(cv::Mat currentFrame);
};

