#pragma once
#define NOMINMAX						 //matrial.inl �Լ� ���� min �Լ��� ��ũ�θ� ���´�.
#include <windows.h>

class IRenderer;
class InputManager;
class ResourceManager;
class TimeManager;
class SoundManager;

//assimp���ø� ����..---------------
class Converter;

class NestGameEngine
{
public:
	NestGameEngine();
	~NestGameEngine();

private:
	//ȭ���� ũ��
	int m_ScreenWidth;
	int m_ScreenHeight;

	// ���� ����
	HWND m_hWnd;
	MSG m_Msg;

	//�׷��Ƚ� ����
	IRenderer* tempGraphicsEngine;

public:
	HRESULT Initialize(HWND hWnd, const int screenWidth, const int screenHeight);
	void GameLoop();
	void Finalize();

	// �޽��� �ڵ鷯 (���� �ݹ�)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//�Ŵ��� �׽�Ʈ
public:
	InputManager* m_InputManager;
	ResourceManager* m_ResourceManager;
	TimeManager* m_TimeManager;
	SoundManager* m_SoundManager;

	//assimp�׽�Ʈ
public:
	Converter* m_pTestConverter;
};

