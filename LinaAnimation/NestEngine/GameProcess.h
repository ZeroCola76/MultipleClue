#pragma once
//#include <imgui.h>
#include "..\GameEngine\NestGameEngine.h"
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
// 	LPARAM lParam);

class SoundManager;

class GameProcess
{
public:
	GameProcess();
	~GameProcess();
private:
	// 윈도 초기 위치
	const int m_WindowPosX = 300;
	const int m_WindowPosY = 300;

	// 화면의 크기
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;

	// 윈도 관련
	HWND m_hWnd;
	MSG m_Msg;

	// 게임 엔진
	NestGameEngine* m_pNestGameEngine;
	SoundManager* m_pSoundManager;
public:
	//게임을 초기화한다.
	HRESULT Initialize(HINSTANCE hInstance);

	//메인 게임 루프, 실제 게임이 진행되는 반복 구문
	void Loop();

	//게임을 정리한다. 
	void Finalize();

};

