#pragma once
#include "GameProcessDemo.h"
#include "NestRenderer.h"
#include "InputManager.h"
#include "EXTimer.h"
#include "Geometry.h"
#include "ResourceManager.h"
#include "directxtk/SimpleMath.h"
#include "EntityFactory.h"
#include "TRSComponent.h"
#include <tuple>
using DirectX::SimpleMath::Vector3;

GameProcessDemo::GameProcessDemo()
{
	
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

	m_hwnd = CreateWindow(wc.lpszClassName, L"4Q �׷��Ƚ� ���� ����", WS_OVERLAPPEDWINDOW,
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

	m_pResourceManager = new ResourceManager();
	m_pResourceManager->Initialize();
	m_pResourceManager->AddResource();

	//EntityFactory�� �ߵ��ư���..?
	myEntityFactory = new EntityFactory;
	myTRSComponent = new TRSComponent;

	pRenderer = new NestRenderer();
	pRenderer->Initialize((int)m_hwnd, m_screenWidth, m_screenHeight);
	//shared_ptr<Entity> tempEntity = myEntityFactory->CreateEntity("������", { "TRSComponent","BoxColliderComponent" });
	////TODO : ������Ʈ �Ŵ������� �� entity ���� ���ҽ��� Ű���� ������ �־�� �� �� �ϴ�.(�� �������.)_ DJ
	
	///�迹����
	//auto temp = m_pResourceManager->GetFBXConvert(L"dancing.fbx"); // T(z, y, x), R(x, y, z) / S(x, y, z)
	////auto temp = m_pResourceManager->GetFBXConvert(L"KingTest1.fbx"); // T(z, y, x), R(x, y, z) / S(x, y, z)
	//Matrix tempTRS1 = myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.f }, { 90.f, 0.0f, 0.0f }, { 0.00001f, 0.00001f, 0.00001f });
	//pRenderer->SetSkeltaModelMeshData(temp, tempTRS1, { m_pResourceManager->GetResource(ResourceType::image, L"testE.png") });
	////pRenderer->SetModelMeshData(temp, tempTRS1, { m_pResourceManager->GetResource(ResourceType::image, L"testK.png") });

	auto temp = m_pResourceManager->GetFBXConvert(L"B_idle.fbx"); // T(z, y, x), R(x, y, z) / S(x, y, z)
	Matrix tempTRS1 = myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.f }, { 0.f, 0.0f, 0.0f }, { 0.001f, 0.001f, 0.001f });
	pRenderer->SetSkeltaModelMeshData(temp, tempTRS1, { m_pResourceManager->GetResource(ResourceType::image, L"B_tex.png") });
	///

	//auto temp = m_pResourceManager->GetFBXConvert(L"zeldaPosed001.fbx"); // T(z, y, x), R(x, y, z) / S(x, y, z)
	////auto temp = m_pResourceManager->GetFBXConvert(L"KingTest1.fbx"); // T(z, y, x), R(x, y, z) / S(x, y, z)
	//Matrix tempTRS1 = myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.f }, { 90.f, 0.0f, 0.0f }, { 0.00001f, 0.00001f, 0.00001f });
	//pRenderer->SetModelMeshData(temp, tempTRS1, { m_pResourceManager->GetResource(ResourceType::image, L"testK.png") });
	////pRenderer->SetModelMeshData(temp, tempTRS1, { m_pResourceManager->GetResource(ResourceType::image, L"testK.png") });

	//auto temp2 = m_pResourceManager->GetFBXConvert(L"zeldaPosed001.fbx"); // z, y, x / z, y, x / x, y, z?
	//Matrix tempTRS2 = myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.0f }, { 0.f, 0.f, 0.f }, { 0.01f, 0.01f, 0.01f });
	//pRenderer->SetModelMeshData(temp2, tempTRS2, { m_pResourceManager->GetResource(ResourceType::image, L"eyes_diff.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"zelda.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"hair_diff.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"zelda.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"zelda.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"fire_diff.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"zeldaSetCave.png"),
	//											   m_pResourceManager->GetResource(ResourceType::image, L"misc_diff.png") });

}


void GameProcessDemo::Update()
{

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
		// �� ������ �ð��� ����Ѵ�.
		pTimer->Tick();

		// �� �������� deltaTime
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

		pRenderer->CurrentMousePos(m_pInputManager->GetMousePos().x, m_pInputManager->GetMousePos().y);
		pRenderer->BeginRender();

		pRenderer->Update(pTimer->DeltaTime()); // �ִϸ��̼� ���� ��ȭ

		pRenderer->DrawMesh();

		pRenderer->EndRender();

	}
}

void GameProcessDemo::Finalize()
{
	//�޸� ����
	pRenderer->Finalize();
}

LRESULT GameProcessDemo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	///TO DO: �ݵ�� �����Ұ�!!!! �迹����
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