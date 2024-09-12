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

	//사운드 매니저 예시 ------------------------------------------------------------
	m_SoundManager = new SoundManager();
	m_SoundManager->Initialize();
	m_SoundManager->LoadSound("myLife.mp3");
	m_SoundManager->PlayBGM(NestSound::ETC01);

	// 	///리소스 매니저 예시---------------------------------------------------------
	m_ResourceManager = new ResourceManager();
	m_ResourceManager->Initialize();
	m_ResourceManager->AddResource();
	//NestGraphics::ModelData* cubeModel = myResourceManager->GetFBXConvert(L"box.fbx");	 //이거 잘되는거 맞어? 돌아버리겠네

	std::wcout << m_ResourceManager->GetResource(RESOURCE_TYPE::Model, L"box.fbx") << std::endl;

	//EntityFactory가 잘돌아가나..? //잘 돌아감
	//EntityFactory* myEntityFactory = new EntityFactory;
	//myEntityFactory->CreateEntity("성동재", { "TRSComponent","BoxColliderComponent" });

	//오브젝트 매니저 예시 --------------------------------------------------------
	m_ObjectManager = new ObjectManager;
	m_ObjectManager->Initialize();
	m_ObjectManager->AddObject("성동재", { "TRSComponent", "SceneComponent" ,"BoxColliderComponent" });
	m_ObjectManager->AddObject("허채윤", { "TRSComponent", "SceneComponent" ,"BoxColliderComponent" });
	m_ObjectManager->AddObject("손기석", { "TRSComponent", "SceneComponent" ,"BoxColliderComponent" });

	//entity안의 SceneComponent의 SceneType 설정 테스트 --------------------------------SceneManager 관련
	auto temp = (m_ObjectManager->GetObject("성동재"))->GetComponent("SceneComponent");
	std::dynamic_pointer_cast<SceneComponent>(temp)->SetSceneType(SCENE_TYPE::TEST01);
	temp = (m_ObjectManager->GetObject("허채윤"))->GetComponent("SceneComponent");
	std::dynamic_pointer_cast<SceneComponent>(temp)->SetSceneType(SCENE_TYPE::TEST01);
	temp = (m_ObjectManager->GetObject("손기석"))->GetComponent("SceneComponent");
	std::dynamic_pointer_cast<SceneComponent>(temp)->SetSceneType(SCENE_TYPE::TEST03);

	auto sung = m_ObjectManager->GetEntityVecAddress();
	SceneManager* sung2 = new SceneManager();
	sung2->SetTempEntityVector(sung);

	//그래픽스 관련 ---------------------------------------------------------------------------
	tempGraphicsEngine = new NestRenderer;
	tempGraphicsEngine->Initialize((int)m_hWnd, m_ScreenWidth, m_ScreenHeight);


	myTRSComponent = new TRSComponent();

	NestGraphics::ModelData* cubeModel = new NestGraphics::ModelData();
	cubeModel = m_ResourceManager->GetFBXConvert(L"box3.fbx");
	tempGraphicsEngine->SetCubeMeshData(cubeModel, m_ResourceManager->GetResource(RESOURCE_TYPE::cubemaps, L"cubemap_bgra.dds"),
		m_ResourceManager->GetResource(RESOURCE_TYPE::cubemaps, L"cubemap_diffuse.dds"),
		m_ResourceManager->GetResource(RESOURCE_TYPE::cubemaps, L"cubemap_specular.dds"));

	//shared_ptr<Entity> tempEntity = myEntityFactory->CreateEntity("성동재", { "TRSComponent","BoxColliderComponent" });
	////TODO : 오브젝트 매니저에서 각 entity 마다 리소스의 키값을 가지고 있어야 할 듯 하다.(얼른 만들겠음.)_ DJ

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

		//일단 돌아감 나중에 다른 클래스 내에서도 돌아가나 봅시다.
		for (int i = 0; i < static_cast<int>(KEY::LAST); i++)
		{
			if (KEY_STATE::TAP == m_InputManager->GetKeyState(static_cast<enum class KEY>(i)))
			{
				std::cout <<" [ " << (char)m_InputManager->m_key[i] << "눌림 ]\n";
			}
			if (KEY_STATE::AWAY == m_InputManager->GetKeyState(static_cast<enum class KEY>(i)))
			{
				std::cout<<" [ " << (char)m_InputManager->m_key[i] << "떼짐 ] \n";
			}

		}

		//tempGraphicsEngine->DrawSomething();

		//tempGraphicsEngine->EndRender();

		//SetTRSMatrix(myTRSComponent->MakeTRSMatrix({ 0.f, 0.f, 0.f }, { 0.f, 0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }));
		tempGraphicsEngine->BeginRender();

		tempGraphicsEngine->Update(m_TimeManager->GetDeltaTime()); // 애니메이션 같은 변화
		tempGraphicsEngine->CurrentMousePos(m_InputManager->GetMousePos().x, m_InputManager->GetMousePos().y);

		//일단 돌아감 나중에 다른 클래스 내에서도 돌아가나 봅시다.
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
