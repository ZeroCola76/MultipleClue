#include "NestDevice.h"
#include "GUIManager.h"
#include "Camera.h"
#include "NestRenderer.h"
#include "GraphicsResourceManager.h"
#include "Box.h"
#include "Sphere.h"
#include "NestGraphicFactory.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

NestRenderer::NestRenderer() : m_hMainWnd(0) {}

NestRenderer::~NestRenderer() {}

bool NestRenderer::Initialize(int hWnd, int screenWidth, int screenHeight)
{
	m_hMainWnd = (HWND)hWnd;
	m_clientWidth = screenWidth;
	m_clientHeight = screenHeight;

	nestDevice = new NestDevice();
	nestDevice->InitializeDevice(hWnd, screenWidth, screenHeight);
	GetInfoDevice();
	D3dUtilities::CreateDepthBuffer(m_pDevice, m_clientWidth, m_clientHeight, m_numQualityLevels, m_pDepthStencilView);
    
	cubeMapping = new CubeMapping();

	cubeMapping->Initialize(m_pDevice, L"../NestResources/cubemaps/skybox/cubemap_bgra.dds",
		L"../NestResources/cubemaps/skybox/cubemap_diffuse.dds", L"../NestResources/cubemaps/skybox/cubemap_specular.dds");

    ///객체 생성 
	meshGroupGround = new MeshGroup();
    ///이건 뭔가 이상해! 김예리나 TO DO : 객체를 생성하는 것 까진 좋은데 캐스트까지 해야하면 이게 효율은 아닌거같다
    MeshGroup* boxptr = new Box();
    MeshGroup* sphereptr = new Sphere();
	meshGroupBox = dynamic_cast<Box*>(boxptr);
	meshGroupSphere = dynamic_cast<Sphere*>(sphereptr);

    ///팩토리..
    m_NestFactory = new NestGraphicFactory();
    m_NestFactory->MakeDXObject(m_pDXObjects, meshGroupBox);
    m_NestFactory->MakeDXObject(m_pDXObjects, meshGroupSphere);

    for (MeshGroup* pObject : m_pDXObjects)
    {
        if (pObject != nullptr)
        {
            pObject->Initialize(m_pDevice);
        }
    }

	NestGraphics::MeshData ground = GameObject::MakeSquare(4.0f);
	ground.textureFilename = "../NestResources/image/uv_grid.png";
    meshGroupGround->Initialize(m_pDevice);
	//meshGroupGround->m_pObjectDiffuseResView = cubeMapping->m_pDiffuseResView;
	//meshGroupGround->m_pObjectSpecularResView = cubeMapping->m_pSpecularResView;

	// 바닥으로 사용하기 위해 회전
	Matrix modelMat = Matrix::CreateRotationX(DirectX::XM_PIDIV2); //DirectX::XM_PIDIV2
	Matrix invTransposeRow = modelMat;
	invTransposeRow.Translation(Vector3(0.0f));
	invTransposeRow = invTransposeRow.Invert().Transpose();

	// ConstantBuffer 초기화 (바닥은 변경 없음)
	meshGroupGround->objectVertexConstantData.world = modelMat.Transpose();
	meshGroupGround->objectVertexConstantData.invTranspose = invTransposeRow.Transpose();
	meshGroupGround->objectPixelConstantData.useTexture = true;
	meshGroupGround->objectPixelConstantData.material.diffuse = Vector3(1.0f);
	meshGroupGround->UpdateConstantBuffers(m_pDevice, m_pContext);

	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        {
		pObject->UpdateConstantBuffers(m_pDevice, m_pContext);
		//sphere.textureFilename = "../NestResources/image/earth.jpg";
		//box.textureFilename = "../NestResources/image/crate2_diffuse.png";
        }
	}

	/// 인스턴스 생성
	camera = new Camera();
	guiManager = new GUIManager(m_clientWidth, m_clientHeight, m_hMainWnd, m_pDevice, m_pContext);
	guiManager->InitializeGUI();

	return true;
}

bool NestRenderer::Update(float m_dTime)
{
	// gui
	guiManager->BeginGUI();
	guiManager->UIUpdateGUI();

	// 카메라의 키 입력 (이거 밖으로 뺄 거임..)
	if (GetAsyncKeyState('W') & 0x8000) { camera->MoveForward(10.0f * m_dTime); }
	if (GetAsyncKeyState('S') & 0x8000) { camera->MoveForward(-10.0f * m_dTime); }
	if (GetAsyncKeyState('A') & 0x8000) { camera->MoveRight(-10.0f * m_dTime); }
	if (GetAsyncKeyState('D') & 0x8000) { camera->MoveRight(10.0f * m_dTime); }


	// 이 부분 없애고, imgui -> 카메라 -> 그래픽스로 데이터를 넘기게 제작 바꿀 예정
	Matrix camViewRow = camera->GetViewRow();
	Matrix camProjRow = camera->GetProjRow();
	Vector3 camEyeWorld = camera->GetEyePos();

	//Matrix = guiManager->GetViewRow();
	//Matrix guiProjRow = guiManager->GetProjRow();
	//Vector3 = guiManager->GetEyePos();

	///큐브매핑 업뎃
	//cubeMapping->UpdateConstantBuffers(m_pDevice, m_pContext, camViewRow.Transpose(), camProjRow.Transpose());
	cubeMapping->UpdateConstantBuffers(m_pDevice, m_pContext, camViewRow.Transpose(), camProjRow.Transpose());

	meshGroupGround->objectPixelConstantData.useTexture = true;
	meshGroupGround->objectPixelConstantData.eyeWorld = camEyeWorld;
	meshGroupGround->objectVertexConstantData.view = camViewRow.Transpose();
	meshGroupGround->objectVertexConstantData.projection = camProjRow.Transpose();

	/// 물체 배치
	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        {
            pObject->Update(camEyeWorld, camViewRow, camProjRow);
        }
	}
	
    ///물체 업뎃
	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        { 
		pObject->UpdateConstantBuffers(m_pDevice, m_pContext);
        }
	}

	static float rot = 0.0f;
	rot += m_dTime;

	// 모델의 변환
	meshGroupGround->objectVertexConstantData.world =
		Matrix::CreateRotationX(m_localRotation.x) * Matrix::CreateRotationY(m_localRotation.y) *
		Matrix::CreateRotationZ(m_localRotation.z) * Matrix::CreateTranslation(m_localTranslation);
	meshGroupGround->objectVertexConstantData.world = meshGroupGround->objectVertexConstantData.world.Transpose();

	//m_localTranslation = guiManager->GetTranslation();
	//m_localRotation = guiManager->GetEyeRot();

	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.world;
	//meshGroupGround->objectVertexConstantData.invTranspose.Translation(Vector3(0.0f));
	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.invTranspose.Transpose().Invert();

	// 시점 변환
	//meshGroupGround->objectVertexConstantData.view = XMMatrixLookToLH(m_viewEyePos, m_viewEyeDir, m_viewUp);
	//meshGroupGround->objectPixelConstantData.eyeWorld =
	//    Vector3::Transform(Vector3(0.0f), meshGroupGround->objectVertexConstantData.view.Invert());
	//meshGroupGround->objectVertexConstantData.view = meshGroupGround->objectVertexConstantData.view.Transpose();

	//m_viewEyePos = // gui 움직임 나중에 수행되기때문에 얘를 기준으로 조작된다..// 임시로 하드코딩함 -> 나중에 수정예정
	//    Vector3(camEyeWorld.x, camEyeWorld.y, camEyeWorld.z + 1.0f);
	//m_viewEyeDir = Vector3(guiManager->GetViewEyeDir());

	//m_viewUp = Vector3(guiManager->GetViewUp());

	// 프로젝션
	//m_aspect = GetAspectRatio();
	//if (m_usePerspectiveProjection) 
	//{
	//     meshGroupGround->objectVertexConstantData.projection
	//        = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), m_aspect, m_nearZ, m_farZ);
	//} 
	//else 
	//{
	//    meshGroupGround->objectVertexConstantData.projection
	//        = XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, m_nearZ, m_farZ);
	//}
	//meshGroupGround->objectVertexConstantData.projection = meshGroupGround->objectVertexConstantData.projection.Transpose();

	//m_usePerspectiveProjection = guiManager->GetIsusePerspectiveProjection();

	//m_projFovAngleY = guiManager->GetProY();
	//m_nearZ = guiManager->GetNearZ();
	//m_farZ = guiManager->GetFarZ();

	// 노멀의 값을 gui에서 가져온다.
	meshGroupGround->objectNormalVertexConstantData = guiManager->GetNBuffer();

	meshGroupGround->m_isDrawNormals = guiManager->GetDrawNormals();
	m_isDrawAsWire = guiManager->GetDrawAsWire();
	meshGroupGround->m_isChangingNormalFlag = guiManager->GetChangingNormalFlag();

	///------위까지 정점 완료, 아래는 픽셸로 분류 하겠음(Riun)

	// gui의 값을 저장한다.
	//m_materialDiffuse = guiManager->GetMaterialDiffuse();
	//m_materialSpecular = guiManager->GetMaterialSpecular();

	//// 픽셸 버퍼 데이터에 디퓨즈와 스펙큘러 값을 저장한다.
	// meshGroupGround->objectPixelConstantData.material.diffuse = Vector3(m_materialDiffuse);
	// meshGroupGround->objectPixelConstantData.material.specular = Vector3(m_materialSpecular);
	// meshGroupGround->objectPixelConstantData.useTexture = guiManager->GetPSBuffer().useTexture;
	// meshGroupGround->objectNormalVertexConstantData.scale = guiManager->GetNBuffer().scale;

	// 빛의 종류와 값을 각각 gui에서 받아온다.
	m_lightType = guiManager->GetLightType();
	m_lightFromGUI = guiManager->GetLightFromGUI();

	// 여러 개 조명 사용하기 위해, 사용하지 않는 조명의 값을 강제로 0으로 조정해서 그림
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		// 다른 조명 끄기
		if (i != m_lightType)
		{
			meshGroupGround->objectPixelConstantData.lights[i].strength *= 0.0f;
		}
		else
		{
			meshGroupGround->objectPixelConstantData.lights[i] = m_lightFromGUI;
		}
	}

	guiManager->UISetEndGUI();

	/// TODO[Riun] : 배경이 바뀔 때만 Update하도록 바꿔준다. (if문 활용하기)[큐브매핑][2]

	return true;
}

void NestRenderer::BeginRender()
{
	///일반적 렌더링 순서 : 뷰포트 설정 -> VS 설정 -> 상수버퍼 설정 -> 렌더 타겟 및 깊이 스텐실 설정 -> 렌더 타겟 클리어

	// 렌더링할 뷰포트를 설정한다. (1번에, m_pD3dScreenViewport 설정)
	m_pContext->RSSetViewports(1, &m_pScreenViewport);

	// 배경 초기화 색상(R, G, B, A)
	float clearColor[4] = { 17.0f / 255.0f, 51.0f / 255.0f, 85.0f / 255.0f, 1.0f };

	// 렌더 타겟 뷰를 설정하고, 거기에 초기화 색상을 넣어준다.
	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);

	// 깊이 스텐실 버퍼(깊이 스텐실 버퍼, 깊이 | 스텐실, 1.0f, 0) // 각각 1.0f와 0은 깊이와 스텐실의 초기화 값
	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void NestRenderer::DrawSomething()
{
	BeginRender();

	m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	if (m_isDrawAsWire)	{m_pContext->RSSetState(m_pWireRasterizerSate.Get());}
	else {m_pContext->RSSetState(m_pSolidRasterizerSate.Get());}

    /// 물체 랜더링
    meshGroupGround->Render(m_pContext);
	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        {
		pObject->Render(m_pContext);

        }
	}

	// 물체 렌더링 후 큐브매핑
	cubeMapping->Render(m_pContext);
	guiManager->RenderGUI();

	EndRender();
}

void NestRenderer::EndRender()
{
	HRESULT hr = (m_pSwapChain->Present(1, 0));
	if (FAILED(hr))
	{
		LPWSTR output;
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&output, 0,
			NULL);
		MessageBox(NULL, output, L"EndRender Error", MB_OK);
	}
}

void NestRenderer::Finalize()
{
	//delete boxptr;
	//delete sphereptr;
	delete meshGroupGround;
	delete cubeMapping;
	delete nestDevice;
	delete guiManager;
	delete camera;
}

void NestRenderer::CurrentMousePos(const int& mouseX, const int& mouseY)
{
	camera->OnMouseMove(m_clientHeight, m_clientWidth, mouseX, mouseY);
}

/// <summary>
/// 디바이스의 정보를 얻어오는 함수
/// </summary>
void NestRenderer::GetInfoDevice() 
{
	m_numQualityLevels = nestDevice->getNumQualityLevels();
	m_pDevice = nestDevice->GetD3dDevice();
	m_pContext = nestDevice->GetD3dContext();
	m_pRenderTargetView = nestDevice->GetD3dRenderTargetView();
	m_pDepthStencilView = nestDevice->GetD3dDepthStencilView();
	m_pSolidRasterizerSate = nestDevice->GetD3dSolidRasterizerSate();
	m_pWireRasterizerSate = nestDevice->GetD3dWireRasterizerSate();
	m_pDepthStencilState = nestDevice->GetD3dDepthStencilState();
	m_pSwapChain = nestDevice->GetD3dSwapChain();
	m_pScreenViewport = nestDevice->GetD3dScreenViewport();
}
