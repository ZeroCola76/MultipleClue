#include "GameProcessDemo.h"
#include "NestRenderer.h"
#include "../NestModule/InputManager.h"
#include "EXTimer.h"
#include "GameObject.h"
#include "Converter.h"

/// 2023.12.27.
/// TODO : IMGUI 붙이자.

GameProcessDemo::GameProcessDemo()
{
	pRenderer = new NestRenderer();
}

GameProcessDemo::~GameProcessDemo()
{
	delete pRenderer;
}

bool GameProcessDemo::Initialize()
{
	m_screenWidth = 1920;
	m_screenHeight = 1080;

	WNDCLASSEX wc = 
	{ 
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL,
		NULL,NULL,NULL,L"5th Academy", NULL 
	};

	if (!RegisterClassEx(&wc)) 
	{
		std::cout << "RegisterClassEx() failed." << std::endl;
		return false;
	}

	RECT wr = { 0, 0, m_screenWidth, m_screenHeight };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindow(wc.lpszClassName, L"4Q 그래픽스 엔진 데모", WS_OVERLAPPEDWINDOW,
						  0, 0, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, wc.hInstance, NULL);

	if (!m_hwnd) 
	{
		std::cout << "IsInitializeWindow() failed." << std::endl;
		return false;
	}

	ShowWindow(m_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(m_hwnd);
	m_pInputManager = new InputManager(m_hwnd);
	m_pInputManager->Initialize();


	pTimer = new EXTimer();
	pTimer->Reset();

	pRenderer->Initialize((int)m_hwnd, m_screenWidth, m_screenHeight);

	//pRenderer->GetMeshData(GameObject::MakeGrid(2.0f, 1.7f, 5, 1, 0.1));
	//pRenderer->GetMeshData(GameObject::MakeBox(1.0f));
	//pRenderer->GetMeshModelData(GameObject::ReadFromFile(L"../NestResources/Model/dragon.fbx"));
	//pRenderer->GetMeshData(GameObject::MakeCylinder(1, 1, 4, 10, 5));
	//pRenderer->GetMeshData(GameObject::MakeSphere(2, 10, 10));

	//pRenderer->GetTexturePath("../NestResources/image/crate2_diffuse.png", "../NestResources/image/coco.jpg", L"../NestResources/hlsl/VertexShader.hlsl", L"../NestResources/hlsl/PixelShader.hlsl");
	
	/// CHANGE_Doyo : NestAsset의 파일 데이터를 가져오는 클래스입니다. 
	/// 테스트용으로 fbx파일의 index를 콘솔창에 출력하는 코드를 추가하였습니다.
	//m_pTestConverter = new Converter();
	//m_pTestConverter->ReadFile(L"../NestResources/Model/dragon.fbx");
	//FileData::ModelData* model = m_pTestConverter->GetModelData();
	//for (int i = 0; i < model->meshes.size(); i++)
	//{
	//	for (int j = 0; j < model->meshes[i].indices.size(); j++)
	//	{
	//		std::cout << model->meshes[i].indices[j] << " ";
	//	}
	//	for (int j = 0; j < model->meshes[i].vertices.size(); j++)
	//	{
	//		std::cout << model->meshes[i].vertices[j].position.x << " ";
	//	}
	//}

	//return true;
}



void GameProcessDemo::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}
		m_pInputManager->Update();
		// 매 프레임 시간을 계산한다.
		pTimer->Tick();

		// 매 프레임의 deltaTime
		pTimer->DeltaTime();
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;

		frameCnt++;

		// Compute averages over one second period.
		if ((pTimer->TotalTime() - timeElapsed) >= 1.0f)
		{
			float fps = (float)frameCnt; // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			frameCnt = 0;
			timeElapsed += 1.0f;
		}

		pRenderer->Update(pTimer->DeltaTime()); // 애니메이션 같은 변화
		pRenderer->CurrentMousePos(m_pInputManager->GetMousePos().x, m_pInputManager->GetMousePos().y);
		pRenderer->DrawSomething();
	}
}

void GameProcessDemo::Finalize()
{
	//메모리 삭제
	pRenderer->Finalize();
}

LRESULT GameProcessDemo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	///TO DO: 반드시 수정할것!!!! 김예리나
	//pRenderer->WinGUI(hWnd,message,wParam,lParam);

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		// Reset and resize swapchain
		break;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_MOUSEMOVE:
		// cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << endl;
		break;
	case WM_LBUTTONUP:
		// cout << "WM_LBUTTONUP Left mouse button" << endl;
		break;
	case WM_RBUTTONUP:
		// cout << "WM_RBUTTONUP Right mouse button" << endl;
		break;
	case WM_KEYDOWN:
		// cout << "WM_KEYDOWN " << (int)wParam << endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}