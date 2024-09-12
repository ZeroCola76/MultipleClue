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

//오브젝트 매니저 전 일단 임시로 이렇게 씁시당
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

	/// CHANGE_Doyo : NestAsset의 Converter 객체입니다. 파일로부터 모델정보를 받아서 변환하는 클래스입니다.
	Converter* m_pTestConverter;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;

	//일단 임시로 객체를 설정할수 있다. 
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

