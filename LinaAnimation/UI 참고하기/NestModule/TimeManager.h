#pragma once
#include "IManager.h"
class TimeManager : public IManager
{
public:
	TimeManager();
	~TimeManager();

private:
	LARGE_INTEGER m_curCount;			//���� �������� �ð��� ����
	LARGE_INTEGER m_preCount;			//���� �������� �ð��� ����
	LARGE_INTEGER m_frequency;			//�ý��� Ŭ���� ���ļ��� ���� ����, ���� ƽ�� �ʴ����� �ٲ� �� ���

	double		  m_deltaTime;			//m_curCount�� m_preCount�� ����Ͽ�
										//�� �����Ӱ��� �ð� ����(��� �ð�)�� ��Ÿ���� ����.
public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

	double GetDeltaTime();

	double GetFPS();
};

