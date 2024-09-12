#pragma once
#define NOMINMAX
#include <windows.h>
#include <iostream>
#include "imgui/imgui.h"

class IRenderer;
class EXTimer;
class Converter;
class InputManager;
class ModelData;
class ResourceManager;

//������Ʈ �Ŵ��� �� �ϴ� �ӽ÷� �̷��� ���ô�
class EntityFactory;
class TRSComponent;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);

class GameProcessDemo
{
private:
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	HWND m_hwnd;
	MSG m_msg;
	IRenderer* pRenderer = nullptr;
	EXTimer* pTimer = nullptr;

	/// CHANGE_Doyo : NestAsset�� Converter ��ü�Դϴ�. ���Ϸκ��� �������� �޾Ƽ� ��ȯ�ϴ� Ŭ�����Դϴ�.
	Converter* m_pTestConverter;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;

	//�ϴ� �ӽ÷� ��ü�� �����Ҽ� �ִ�. 
	EntityFactory* myEntityFactory;
	TRSComponent* myTRSComponent;
	
public:
	GameProcessDemo();
	~GameProcessDemo();

	bool Initialize();
	void Update();
	void Loop();
	void Finalize();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

