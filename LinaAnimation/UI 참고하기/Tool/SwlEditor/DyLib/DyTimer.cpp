#include "DyTimer.h"

DyTimer::DyTimer()
	:m_secPerCount(0.0), m_deltaTime(0.0), m_baseTime(0), m_pausedTime(0), m_stopTime(0)
	, m_prevTime(0), m_curTime(0), m_stopped(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_secPerCount = 1.0 / (double)countsPerSec;
}

float DyTimer::TotalTime() const
{
	auto totalTime = m_stopped ? ((m_stopTime - m_pausedTime) - m_baseTime) * m_secPerCount
		: ((m_curTime - m_pausedTime) - m_baseTime) * m_secPerCount;
	return totalTime;
}

float DyTimer::DeltaTime() const
{
	return (float)m_deltaTime;
}

void DyTimer::Reset()
{
	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	m_baseTime = curTime;
	m_prevTime = curTime;
	m_stopTime = 0;
	m_stopped = false;
}

void DyTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_stopped)
	{
		m_pausedTime += startTime - m_stopTime;
		m_prevTime = startTime;
		m_stopTime = 0;
		m_stopped = false;
	}

}

void DyTimer::Stop()
{
	if (!m_stopped)
	{
		__int64 curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		m_stopTime = curTime;
		m_stopped = true;
	}
}

void DyTimer::Tick()
{
	if (m_stopped)
	{
		m_deltaTime = 0.0;
		return;
	}

	__int64 curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
	m_curTime = curTime;

	m_deltaTime = (m_curTime - m_prevTime) * m_secPerCount;

	m_prevTime = m_curTime;

	if (m_deltaTime < 0.0)
	{
		m_deltaTime = 0.0;
	}
}

DyTimer* DyTimer::instance = nullptr;
