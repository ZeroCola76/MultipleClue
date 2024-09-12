#include "TimeManager.h"

TimeManager::TimeManager() : m_curCount{}, m_preCount{}, m_frequency{}, m_deltaTime{ 0 }
{
	Initialize();
}

TimeManager::~TimeManager()
{

}

void TimeManager::Initialize()
{
	QueryPerformanceCounter(&m_preCount);						//현재 시간을 m_preCount에 얻어온다. -> 초기화 함수기때문에
	QueryPerformanceFrequency(&m_frequency);					//시스템 클록의 주파수를 얻어와 m_frequency변수에 저장한다.
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&m_curCount);						// 현재 시간을 m_curCount에 저장한다. 

	//현재/이전 프레임의 시간 차를 카운터 주파수로 나누어 초 단위로 변환한다.
	m_deltaTime = static_cast<double>(m_curCount.QuadPart - m_preCount.QuadPart) / static_cast<double>(m_frequency.QuadPart);

	//현재의 프레임 값을 이전프레임 값에 대입한다.
	m_preCount = m_curCount;
}

void TimeManager::Finalize()
{

}

double TimeManager::GetDeltaTime()
{
	return m_deltaTime;
}

double TimeManager::GetFPS()
{
	if (m_deltaTime != 0)
	{
		return (double)(1.0 / m_deltaTime);					  //1초를 deltatime으로 나누어 fps를 계산한다.
	}
	return 0;
}
