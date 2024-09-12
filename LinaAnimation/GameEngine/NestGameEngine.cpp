#pragma once
#include "NestGameEngine.h"
#include "NestRenderer.h"	
#include "InputManager.h"							//#include "..\NestModule\InputManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "PathManager.h"
#include "ObjectManager.h"
#include "EntityFactory.h"
#include "SceneManager.h"
#include "SceneEnum.h"
#include "Entity.h"
//#include "SceneComponent.h"
#include "InputEnum.h"
#include "Geometry.h"

NestGameEngine::NestGameEngine() : m_ScreenHeight{}, m_ScreenWidth{}, m_hWnd{}, m_Msg{}
{
	tempGraphicsEngine = nullptr;
	m_ResourceManager = nullptr;
	m_TimeManager = nullptr;

	m_InputManager = InputManager::getInstance();
	m_InputManager->SetHwnd(m_hWnd);
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


	m_TimeManager = new TimeManager();
	m_TimeManager->Initialize();

	//���� �Ŵ��� ���� ------------------------------------------------------------
	m_SoundManager = new SoundManager();
	m_SoundManager->Initialize();
	m_SoundManager->LoadSound("myLife.mp3");
	m_SoundManager->PlayBGM(NestSound::ETC01);

	// 	///���ҽ� �Ŵ��� ����---------------------------------------------------------
	m_ResourceManager = new ResourceManager();
	m_ResourceManager->Initialize();
	m_ResourceManager->AddResource();
	//NestGraphics::ModelData* cubeModel = myResourceManager->GetFBXConvert(L"box.fbx");	 //�̰� �ߵǴ°� �¾�? ���ƹ����ڳ�

	std::wcout << m_ResourceManager->GetResource(RESOURCE_TYPE::Model, L"box.fbx") << std::endl;

	//EntityFactory�� �ߵ��ư���..? //�� ���ư�
	//EntityFactory* myEntityFactory = new EntityFactory;
	//myEntityFactory->CreateEntity("������", { "TRSComponent","BoxColliderComponent" });

	//������Ʈ �Ŵ��� ���� --------------------------------------------------------
	m_ObjectManager = new ObjectManager;
	m_ObjectManager->Initialize();
	m_ObjectManager->AddObject("������", { "TRSComponent", "SceneComponent" ,"BoxColliderComponent" });
	m_ObjectManager->AddObject("��ä��", { "TRSComponent", "SceneComponent" ,"BoxColliderComponent" });
	m_ObjectManager->AddObject("�ձ⼮", { "TRSComponent", "SceneComponent" ,"BoxColliderComponent" });

	//entity���� SceneComponent�� SceneType ���� �׽�Ʈ --------------------------------SceneManager ����
	auto temp = (m_ObjectManager->GetObject("������"))->GetComponent("SceneComponent");
	std::dynamic_pointer_cast<SceneComponent>(temp)->SetSceneType(SCENE_TYPE::TEST01);
	temp = (m_ObjectManager->GetObject("��ä��"))->GetComponent("SceneComponent");
	std::dynamic_pointer_cast<SceneComponent>(temp)->SetSceneType(SCENE_TYPE::TEST01);
	temp = (m_ObjectManager->GetObject("�ձ⼮"))->GetComponent("SceneComponent");
	std::dynamic_pointer_cast<SceneComponent>(temp)->SetSceneType(SCENE_TYPE::TEST03);

	auto sung = m_ObjectManager->GetEntityVecAddress();
	SceneManager* sung2 = new SceneManager();
	sung2->SetTempEntityVector(sung);

	//�׷��Ƚ� ���� ---------------------------------------------------------------------------
	tempGraphicsEngine = new NestRenderer;
	tempGraphicsEngine->Initialize((int)m_hWnd, m_ScreenWidth, m_ScreenHeight);


	myTRSComponent = new TRSComponent();

	NestGraphics::ModelData* cubeModel = new NestGraphics::ModelData();
	cubeModel = m_ResourceManager->GetFBXConvert(L"box3.fbx");
	tempGraphicsEngine->SetCubeMeshData(cubeModel, m_ResourceManager->GetResource(RESOURCE_TYPE::cubemaps, L"cubemap_bgra.dds"),
		m_ResourceManager->GetResource(RESOURCE_TYPE::cubemaps, L"cubemap_diffuse.dds"),
		m_ResourceManager->GetResource(RESOURCE_TYPE::cubemaps, L"cubemap_specular.dds"));

	//shared_ptr<Entity> tempEntity = myEntityFactory->CreateEntity("������", { "TRSComponent","BoxColliderComponent" });
	////TODO : ������Ʈ �Ŵ������� �� entity ���� ���ҽ��� Ű���� ������ �־�� �� �� �ϴ�.(�� �������.)_ DJ

	auto tempbox = m_ResourceManager->GetFBXConvert(L"box.fbx"); // T(z, y, x), R(x, y, z) / S(x, y, z)
	Matrix tempTRS1 = myTRSComponent->MakeTRSMatrix({ 0.f, 0.5f, 0.f }, { 0.f, 0.0f, 0.5f }, { 0.01f, 0.01f, 0.01f });
	tempGraphicsEngine->SetModelMeshData(tempbox, tempTRS1, { m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"crate2_diffuse.png") });

	auto temp2 = m_ResourceManager->GetFBXConvert(L"zeldaPosed001.fbx"); // z, y, x / z, y, x / x, y, z?
	Matrix tempTRS2 = myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.0f }, { 0.f, 0.f, 0.f }, { 0.01f, 0.01f, 0.01f });
	tempGraphicsEngine->SetModelMeshData(temp2, tempTRS2, { m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"eyes_diff.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"zelda.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"hair_diff.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"zelda.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"zelda.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"fire_diff.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"zeldaSetCave.png"),
												   m_ResourceManager->GetResource(RESOURCE_TYPE::image, L"misc_diff.png") });

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

		m_InputManager->Update();
		m_SoundManager->Update();

		m_TimeManager->Update();
		std::cout << "\rFPS : " << m_TimeManager->GetFPS();

		//�ϴ� ���ư� ���߿� �ٸ� Ŭ���� �������� ���ư��� ���ô�.
		for (int i = 0; i < static_cast<int>(KEY::LAST); i++)
		{
			if (KEY_STATE::TAP == m_InputManager->GetKeyState(static_cast<enum class KEY>(i)))
			{
				std::cout <<" [ " << (char)m_InputManager->m_key[i] << "���� ]\n";
			}
			if (KEY_STATE::AWAY == m_InputManager->GetKeyState(static_cast<enum class KEY>(i)))
			{
				std::cout<<" [ " << (char)m_InputManager->m_key[i] << "���� ] \n";
			}

		}

		//tempGraphicsEngine->DrawSomething();

		//tempGraphicsEngine->EndRender();

		//SetTRSMatrix(myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.f }, { 0.f, 0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }));
		tempGraphicsEngine->BeginRender();

		tempGraphicsEngine->Update(m_TimeManager->GetDeltaTime()); // �ִϸ��̼� ���� ��ȭ
		tempGraphicsEngine->CurrentMousePos(m_InputManager->GetMousePos().x, m_InputManager->GetMousePos().y);

		//�ϴ� ���ư� ���߿� �ٸ� Ŭ���� �������� ���ư��� ���ô�.
		for (int i = 0; i < static_cast<int>(KEY::LAST); i++)
		{
			if (KEY_STATE::HOLD == m_InputManager->GetKeyState(static_cast<enum class KEY>(i)))
			{
				tempGraphicsEngine->CurrentKeyPressed(m_InputManager->m_key[i]);
			}
		}

		tempGraphicsEngine->DrawMesh();

		tempGraphicsEngine->EndRender();

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
