#pragma once
#include <windows.h>

/// <summary>
/// QueryPerformance�� �̿��� Timer class �Դϴ�.
/// �� �Ѱ��� ��ü�� ����ϱ� ���� ��Ŭ�� �������� �����Ͽ����ϴ�.
/// </summary>
class DyTimer
{
private:
	DyTimer();
	DyTimer(const DyTimer& _copy) = default;
	DyTimer& operator=(const DyTimer& _copy) = default;
	~DyTimer() {}
public:
	static DyTimer* GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new DyTimer();
			return instance;
		}
	}

	float TotalTime() const;
	float DeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:
	static DyTimer* instance;

	double m_secPerCount;
	double m_deltaTime;

	__int64 m_baseTime;
	__int64 m_pausedTime;
	__int64 m_stopTime;
	__int64 m_prevTime;
	__int64 m_curTime;

	bool m_stopped;
	
};

