#pragma once
#include <windows.h>

/// <summary>
/// QueryPerformance를 이용한 Timer class 입니다.
/// 단 한개의 객체만 사용하기 위해 싱클턴 패턴으로 설계하였습니다.
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

