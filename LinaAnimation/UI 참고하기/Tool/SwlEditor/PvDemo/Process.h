#pragma once
#include <windows.h>
#include "Resource.h"
#include "framework.h"

class PvRenderer;
class DyTimer;
class JsonUtil;

class Process
{
public:
	Process() {}
	~Process() {}

	bool Initialize(HINSTANCE hInstance);
	void Loop();
	void Finalize();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void Update();
	void Render();
	void CreateConsole();

	void TestCode();

	/*
	void SetObject(const Json::Value& val, std::wstring key)
	{
		//m_data;	// 클래스의 멤버변수
		m_data[key] = val;
	}
	
	void SetObject(const Json::Value& val, std::wstring key, std::initializer_list<std::wstring>& keys)
	{
		//m_data;	// 클래스의 멤버변수
		Json::Value curObj = m_data;
		for (const auto& key : keys)
		{

		}
	}
	*/


	MSG m_msg;

	const int m_screenWidth = 1024;
	const int m_screenHeight = 720;
	const int m_screenPosX = 100;
	const int m_screenPosY = 100;

	DyTimer* m_pTimer;
	PvRenderer* m_p3DRenderer;
	JsonUtil* m_pJsonUtil;
};

