#pragma once
#include "NestGameEngine.h"
#include "NestRenderer.h"	
#include "InputManager.h"							//#include "..\NestModule\InputManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "SoundManager.h"


#include "Converter.h"

NestGameEngine::NestGameEngine() : m_ScreenHeight{}, m_ScreenWidth{}, m_hWnd{}, m_Msg{}
{
	tempGraphicsEngine = nullptr;
	m_InputManager = nullptr;
	m_ResourceManager = nullptr;
	m_TimeManager = nullptr;
}

NestGameEngine::~NestGameEngine()
{
	Finalize();
}

HRESULT NestGameEngine::Initialize(HWND hWnd, const int screenWidth, const int screenHeight)
{
	m_hWnd = hWnd;
	m_ScreenHeight = screenHeight;
	m_ScreenWidth = screenWidth;

	//tempGraphicsEngine = new NestRenderer;
	//tempGraphicsEngine->Initialize((int)m_hWnd, m_ScreenWidth, m_ScreenHeight);
	//tempGraphicsEngine->InitializeGUI();

	m_InputManager = new InputManager(hWnd);
	m_ResourceManager = new ResourceManager();
	m_TimeManager = new TimeManager();
	m_TimeManager->Initialize();

	//리소스 매니저 예시 ------------------------------------------------------------
 	std::shared_ptr<GameEngine::Mesh> mySung1 = std::make_shared<GameEngine::Mesh>(ResourceType::MESH);
 	m_ResourceManager->Add(L"MyKey", mySung1);
 	m_ResourceManager->Get<GameEngine::Mesh>(L"MyKey");

	std::shared_ptr<GameEngine::Shader> mySung2 = std::make_shared<GameEngine::Shader>(ResourceType::SHADER);
	mySung2->SetFilePath(L"이야이야호");
	mySung2->GetFilePath();
	m_ResourceManager->Add(L"MyShader", mySung2);
	m_ResourceManager->Get<GameEngine::Shader>(L"MyShader");


	//사운드 매니저 예시 ------------------------------------------------------------
	m_SoundManager = new SoundManager();
	m_SoundManager->Initialize();
	m_SoundManager->LoadSound("odyssey.mp3");
	m_SoundManager->PlayBGM(NestSound::ETC01);

	//assimp예시 -------------------------------------------------------------------
	/// 테스트용으로 fbx파일의 index를 콘솔창에 출력하는 코드를 추가하였습니다.
	m_pTestConverter = new Converter();
	m_pTestConverter->ReadFile(L"../NestResources/Model/box.fbx");
	NestGraphics::ModelData* model = m_pTestConverter->GetModelData(0);
	for (int i = 0; i < model->meshes.size(); i++)
	{
		for (int j = 0; j < model->meshes[i].indices.size(); j++)
		{
			std::cout << model->meshes[i].indices[j] << " ";
		}
		for (int j = 0; j < model->meshes[i].vertices.size(); j++)
		{
			std::cout << model->meshes[i].vertices[j].position.x << " ";
		}
	}
	std::cout << "아 경고 너무 많은데 아마 여기저기서 라이브러리를 막 불러다 쓰고 있기 때문인거 같아.\n" << std::endl;
	
	//tempGraphicsEngine->GetMeshData(GameObject::MakeSphere(2, 10, 10));
	//tempGraphicsEngine->GetTexturePath("crate2_diffuse.png", "coco.jpg", L"../NestEngine/ColorVertexShader.hlsl", L"../NestEngine/ColorPixelShader.hlsl");
	
	return S_OK;
}

void NestGameEngine::GameLoop()
{
	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;

			DispatchMessage(&m_Msg);
		}
		else
		{

		}
		///그래픽스 엔진이 터져 일단은 주석처리후 게임엔진을 짠다.
 		//tempGraphicsEngine->Update(ImGui::GetIO().DeltaTime); // 애니메이션 같은 변화
 		m_InputManager->Update();
		m_SoundManager->Update();

		m_TimeManager->Update();
		std::cout <<"\rFPS : " << m_TimeManager->GetFPS();

		//tempGraphicsEngine->BeginRender();
 
 		//tempGraphicsEngine->DrawSomething();
 
 		//tempGraphicsEngine->EndRender();
	
	}
}

void NestGameEngine::Finalize()
{
	delete tempGraphicsEngine;
	delete m_ResourceManager;

}

LRESULT CALLBACK NestGameEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//아임구이 관련
// 	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
// 		return true;

	switch (message)
	{

		case WM_SIZE:																//윈도우의 크기가 변경될 때 발생하는 메세지 
			// Reset and resize swapchain											// 이 메세지를 처리하여 스왑체인을 재설정한다.
			return 0;
		case WM_SYSCOMMAND:															//시스템 명령이 발생할 때 발생하는 메시지
			if ((wParam & 0xfff0) == SC_KEYMENU)
				return 0;
			break;
		case WM_KEYDOWN:
			if (wParam == 27)
			{
				PostQuitMessage(0);
			}
			//std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
	}

	return ::DefWindowProc(hWnd, message, wParam, lParam);
}
