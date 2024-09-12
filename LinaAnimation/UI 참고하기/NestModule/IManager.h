#pragma once
#include <iostream>
#include <windows.h>
class IManager
{
public:
	IManager() {}
	virtual ~IManager() {}

	//매니저의 초기화를 명시적으로 할 수 있도록 한다. 
	virtual void Initialize() abstract;

	//내용이 바꿀 때 쓰일 업데이트 함수
	virtual void Update() abstract;

	//매니저의 해제 역시 명시적으로
	virtual void Finalize() abstract;
};