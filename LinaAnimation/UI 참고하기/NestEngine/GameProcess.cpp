#pragma once
#include <windows.h>
#include <memory>
#include<iostream>
#include "GameProcess.h"
#include "../NestModule/SoundManager.h"

GameProcess::GameProcess()
{
	//멤버변수를 초기화한다.
	m_hWnd = nullptr;
	m_Msg = {};

	//콘솔창에서 띄울 메세지
	std::cout << "GameProcess 생성" << std::endl;
}

GameProcess::~GameProcess()
{

}

HRESULT GameProcess::Initialize(HINSTANCE hInstance)
{
	//운영체제에서 창 클래스를 정의하고 등록한다. WNDCLASSEX는 창 클래스의 속성을 정의하는 구조체이다. 
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),					//구조체의 크기를 나타낸다. 
		CS_CLASSDC,							//창 클래스 스타일을 설정한다.
		NestGameEngine::WndProc,			//창 프로시저를 가리킨다.  창 프로시저는 창이 메시지를 어떻게 처리할지를 결정하는 함수이다. 
		0,
		0,
		GetModuleHandle(NULL),				//인스턴스 핸들을 가져온다. NULL일 시 현재 실행중인 인스턴스의 핸들을 가져온다.
		NULL,
		NULL,
		NULL,
		NULL,
		L"MangoWaffle",						// lpszClassName, L-string, 창 클래스의 이름을 설정한다. 
		NULL };

	RegisterClassEx(&wc);					//이 창을 시스템에 등록하는 함수의 호출, 이 창을 호출 함으로 우리는 이 창 클래스를 생성할수 있게 된다. 


	///Window 창을 생성하는 부분, CreateWindow함수는 주어진 매개변수에 따라 새로운 창을 생성하고, 그 창의 핸들을 반환한다.
	m_hWnd = CreateWindow(
		wc.lpszClassName,										   // 앞서 등록한 창클래스의 이름. 이름을 통해 CreateWindow함수는 어떤 창 클래스를 사용하여 창을 생성할지 알수 있다.
		L"MangoWaffle Graphics",								   // 생성될 창의 제목. 클래스의 이름과는 다르다.
		WS_OVERLAPPEDWINDOW,									   // 창의 스타일을 설정한다. WS_OVERLAPPEDWINDOW는 타이틀 바, 최대화/최소화 버튼 등을 포함하는 표준 창
		m_WindowPosX,											   //윈도우 좌측 상단의 x 좌표	
		m_WindowPosY,											   //윈도우 좌측 상단의 y 좌표	
		m_ScreenWidth,											   //윈도우 가로 방향 해상도	
		m_ScreenHeight,											   //윈도우 세로 방향 해상도	
		NULL,
		NULL,
		wc.hInstance,											   //앞서 GetModuleHandle(NULL) 함수를 통해 가져온 인스턴스 핸들
		NULL);

	if (!m_hWnd) 
	{ 
		std::cout << "m_hWnd - CreateWindow Error\n";
		return E_FAIL;
	}

	// 실제로 그려지는 해상도를 설정하기 위해
	RECT wr = { m_WindowPosX, m_WindowPosY, m_ScreenWidth, m_ScreenHeight };                   // Rect구조체를 사용하여 창의 크기를 결정한다. set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);	 // AdjustWindowRect 함수를 호출하여 창의 크기를 조정, adjust the size
	//함수는 주어진 스타일(WS_OVERLAPPEDWINDOW)에 따라 창의 크기를 조정한다.

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);							   // ShowWindow 함수를 호출하여 창을 화면에 표시, hwnd는 앞서 CreateWindow 함수를 통해 생성된 창의 핸들, SW_SHOWDEFAULT는 창을 기본 상태로 표시하도록 지시
	UpdateWindow(m_hWnd);											   // UpdateWindow 함수를 호출하여 창을 즉시 업데이트


	///⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐
	//게임엔진 초기화는 여기서 진행한다.
	///⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐⭐
	m_pNestGameEngine = new NestGameEngine();
	m_pNestGameEngine->Initialize(m_hWnd, m_ScreenWidth, m_ScreenHeight);


	return S_OK;
}

void GameProcess::Loop()
{
	m_pNestGameEngine->GameLoop();
}

void GameProcess::Finalize()
{
	delete m_pNestGameEngine;
}
