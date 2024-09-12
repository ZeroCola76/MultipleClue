#pragma once
#include "IManager.h"
class TimeManager : public IManager
{
public:
	TimeManager();
	~TimeManager();

private:
	LARGE_INTEGER m_curCount;			//현재 프레임의 시간을 저장
	LARGE_INTEGER m_preCount;			//이전 프레임의 시간을 저장
	LARGE_INTEGER m_frequency;			//시스템 클록의 주파수를 얻어와 저장, 이후 틱을 초단위로 바꿀 때 사용

	double		  m_deltaTime;			//m_curCount와 m_preCount를 사용하여
										//두 프레임간의 시간 간격(경과 시간)을 나타내는 변수.
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

	double GetDeltaTime();

	double GetFPS();
};

