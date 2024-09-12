#pragma once
#include <iostream>
#include <windows.h>
class IManager
{
public:
	IManager() {}
	virtual ~IManager() {}

	//�Ŵ����� �ʱ�ȭ�� ��������� �� �� �ֵ��� �Ѵ�. 
	virtual void Initialize() abstract;

	//������ �ٲ� �� ���� ������Ʈ �Լ�
	virtual void Update() abstract;

	//�Ŵ����� ���� ���� ���������
	virtual void Finalize() abstract;
};