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

	//���ҽ� �Ŵ��� ���� ------------------------------------------------------------
 	std::shared_ptr<GameEngine::Mesh> mySung1 = std::make_shared<GameEngine::Mesh>(ResourceType::MESH);
 	m_ResourceManager->Add(L"MyKey", mySung1);
 	m_ResourceManager->Get<GameEngine::Mesh>(L"MyKey");

	std::shared_ptr<GameEngine::Shader> mySung2 = std::make_shared<GameEngine::Shader>(ResourceType::SHADER);
	mySung2->SetFilePath(L"�̾��̾�ȣ");
	mySung2->GetFilePath();
	m_ResourceManager->Add(L"MyShader", mySung2);
	m_ResourceManager->Get<GameEngine::Shader>(L"MyShader");


	//���� �Ŵ��� ���� ------------------------------------------------------------
	m_SoundManager = new SoundManager();
	m_SoundManager->Initialize();
	m_SoundManager->LoadSound("odyssey.mp3");
	m_SoundManager->PlayBGM(NestSound::ETC01);

	//assimp���� -------------------------------------------------------------------
	/// �׽�Ʈ������ fbx������ index�� �ܼ�â�� ����ϴ� �ڵ带 �߰��Ͽ����ϴ�.
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
	std::cout << "�� ��� �ʹ� ������ �Ƹ� �������⼭ ���̺귯���� �� �ҷ��� ���� �ֱ� �����ΰ� ����.\n" << std::endl;
	
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
		///�׷��Ƚ� ������ ���� �ϴ��� �ּ�ó���� ���ӿ����� §��.
 		//tempGraphicsEngine->Update(ImGui::GetIO().DeltaTime); // �ִϸ��̼� ���� ��ȭ
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
//���ӱ��� ����
// 	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
// 		return true;

	switch (message)
	{

		case WM_SIZE:																//�������� ũ�Ⱑ ����� �� �߻��ϴ� �޼��� 
			// Reset and resize swapchain											// �� �޼����� ó���Ͽ� ����ü���� �缳���Ѵ�.
			return 0;
		case WM_SYSCOMMAND:															//�ý��� ����� �߻��� �� �߻��ϴ� �޽���
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
