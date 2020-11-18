#include "FrameHistory.h"

FrameHistory::FrameHistory(cv::Mat currentFrame)
{
	m_currentFrame = currentFrame;
}

cv::Mat FrameHistory::getCurrent()
{
	return m_currentFrame;
}

cv::Mat FrameHistory::getPrevious()
{
	return m_previousFrame;
}

void FrameHistory::update(cv::Mat currentFrame)
{
	if (currentFrame.empty()) throw new std::runtime_error("Empty frame updating");
	m_previousFrame = m_currentFrame;
	m_currentFrame = currentFrame;
}
