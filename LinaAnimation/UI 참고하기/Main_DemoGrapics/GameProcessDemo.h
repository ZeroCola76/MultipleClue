#pragma once
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include "../NestInclude/imgui/imgui.h"

class IRenderer;
class EXTimer;
class Converter;
class InputManager;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);

class GameProcessDemo
{
private:
	int m_screenWidth;
	int m_screenHeight;
	HWND m_hwnd;
	MSG m_msg;
	IRenderer* pRenderer;
	EXTimer* pTimer;

	/// CHANGE_Doyo : NestAsset�� Converter ��ü�Դϴ�. ���Ϸκ��� �������� �޾Ƽ� ��ȯ�ϴ� Ŭ�����Դϴ�.
	Converter* m_pTestConverter;
	InputManager* m_pInputManager;

public:
	GameProcessDemo();
	~GameProcessDemo();

	bool Initialize();
	void Loop();
	void Finalize();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

