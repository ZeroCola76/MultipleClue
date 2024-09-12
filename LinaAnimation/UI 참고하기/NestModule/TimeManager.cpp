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
	QueryPerformanceCounter(&m_preCount);						//���� �ð��� m_preCount�� ���´�. -> �ʱ�ȭ �Լ��⶧����
	QueryPerformanceFrequency(&m_frequency);					//�ý��� Ŭ���� ���ļ��� ���� m_frequency������ �����Ѵ�.
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&m_curCount);						// ���� �ð��� m_curCount�� �����Ѵ�. 

	//����/���� �������� �ð� ���� ī���� ���ļ��� ������ �� ������ ��ȯ�Ѵ�.
	m_deltaTime = static_cast<double>(m_curCount.QuadPart - m_preCount.QuadPart) / static_cast<double>(m_frequency.QuadPart);

	//������ ������ ���� ���������� ���� �����Ѵ�.
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
		return (double)(1.0 / m_deltaTime);					  //1�ʸ� deltatime���� ������ fps�� ����Ѵ�.
	}
	return 0;
}
