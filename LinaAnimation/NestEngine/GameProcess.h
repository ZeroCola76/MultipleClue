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
	// ���� �ʱ� ��ġ
	const int m_WindowPosX = 300;
	const int m_WindowPosY = 300;

	// ȭ���� ũ��
	const int m_ScreenWidth = 1920;
	const int m_ScreenHeight = 1080;

	// ���� ����
	HWND m_hWnd;
	MSG m_Msg;

	// ���� ����
	NestGameEngine* m_pNestGameEngine;
	SoundManager* m_pSoundManager;
public:
	//������ �ʱ�ȭ�Ѵ�.
	HRESULT Initialize(HINSTANCE hInstance);

	//���� ���� ����, ���� ������ ����Ǵ� �ݺ� ����
	void Loop();

	//������ �����Ѵ�. 
	void Finalize();

};

