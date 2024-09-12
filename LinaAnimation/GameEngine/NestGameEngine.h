#pragma once
#define NOMINMAX						 //matrial.inl 함수 내의 min 함수의 매크로를 막는다.
#include <windows.h>



class IRenderer;
class InputManager;
class ResourceManager;
class TimeManager;
class SoundManager;
class PathManager;
class ObjectManager;
class TRSComponent;

class NestGameEngine
{
public:
	NestGameEngine();
	~NestGameEngine();

private:
	//화면의 크기
	int m_ScreenWidth;
	int m_ScreenHeight;

	// 윈도 관련
	HWND m_hWnd;
	MSG m_Msg;

	//그래픽스 엔진
	IRenderer* tempGraphicsEngine;

public:
	HRESULT Initialize(HWND hWnd, const int screenWidth, const int screenHeight);
	void GameLoop();
	void Finalize();

	// 메시지 핸들러 (윈도 콜백)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//매니저 테스트
public:
	InputManager* m_InputManager;
	ResourceManager* m_ResourceManager;
	TimeManager* m_TimeManager;
	SoundManager* m_SoundManager;
	PathManager* m_PathManager;
	ObjectManager* m_ObjectManager;

	TRSComponent* myTRSComponent;
};

