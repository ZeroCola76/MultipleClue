#include "NestDevice.h"
#include "GUIManager.h"
#include "Camera.h"
#include "NestRenderer.h"


//dx11 라이브러리
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

NestRenderer::NestRenderer() : m_hMainWnd(0)
{
}

NestRenderer::~NestRenderer()
{
}

bool NestRenderer::Initialize(int hWnd, int screenWidth, int screenHeight)
{
	//hwnd 가져오기 데모에서
	m_hMainWnd = (HWND)hWnd;
	//해상도 가져옴
	m_clientWidth = screenWidth;
	m_clientHeight = screenHeight;

	//device와 context 생성
	nestDevice = new NestDevice();
	nestDevice->InitializeDevice(hWnd, screenWidth, screenHeight);
	GetInfoDevice();

	//깊이 버퍼 생성 //인자는 GetInfoDevice함수 내에서 초기화
	D3dUtilities::CreateDepthBuffer(m_pDevice, m_clientWidth, m_clientHeight, m_numQualityLevels, m_pDepthStencilView);

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

	// 카메라의 위치를 아임구이와 연동시키는 부분
	m_localTranslation = guiManager->SetTranslation(camera->getLocalPosition());

	// 카메라의 키 입력 (이거 밖으로 뺄 거임..)
	if (GetAsyncKeyState('W') & 0x8000) { camera->MoveForward(10.0f * m_dTime); }
	if (GetAsyncKeyState('S') & 0x8000) { camera->MoveForward(-10.0f * m_dTime); }
	if (GetAsyncKeyState('A') & 0x8000) { camera->MoveRight(-10.0f * m_dTime); }
	if (GetAsyncKeyState('D') & 0x8000) { camera->MoveRight(10.0f * m_dTime); }
	if (GetAsyncKeyState('Q') & 0x8000) { camera->MoveUp(10.0f * m_dTime); }
	if (GetAsyncKeyState('E') & 0x8000) { camera->MoveUp(-10.0f * m_dTime); }
	

	// 이 부분 없애고, imgui -> 카메라 -> 그래픽스로 데이터를 넘기게 제작 바꿀 예정
	Matrix camViewRow = camera->GetViewRow();
	Matrix camProjRow = camera->GetProjRow();
	Vector3 camEyeWorld = camera->GetEyePos();

	for (auto e : m_meshGroupVec)
	{
		e->CamUpdate(m_dTime, m_pDevice, m_pContext, camViewRow, camProjRow, camEyeWorld);
	}
	
	///이건 왜 있는거지? 자동회전시킬라했나..?
	//static float rot = 0.0f;
	//rot += m_dTime;
	
	/*
	// 모델의 변환
	//meshGroupGround->objectVertexConstantData.world =
	//	Matrix::CreateRotationX(m_localRotation.x) * Matrix::CreateRotationY(m_localRotation.y) *
	//	Matrix::CreateRotationZ(m_localRotation.z) * Matrix::CreateTranslation(m_localTranslation);
	//meshGroupGround->objectVertexConstantData.world = meshGroupGround->objectVertexConstantData.world.Transpose();

	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.world;
	//meshGroupGround->objectVertexConstantData.invTranspose.Translation(Vector3(0.0f));
	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.invTranspose.Transpose().Invert();

	// 시점 변환
	//meshgroupground->objectvertexconstantdata.view = xmmatrixlooktolh(m_vieweyepos, m_vieweyedir, m_viewup);
	//meshgroupground->objectpixelconstantdata.eyeworld =
	//    vector3::transform(vector3(0.0f), meshgroupground->objectvertexconstantdata.view.invert());
	//meshgroupground->objectvertexconstantdata.view = meshgroupground->objectvertexconstantdata.view.transpose();

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
	*/

	m_isDrawAsWire = guiManager->GetDrawAsWire();

	// 노멀의 값을 gui에서 가져온다.
	//meshGroupGround->objectNormalVertexConstantData = guiManager->GetNBuffer();
	//meshGroupGround->m_isDrawNormals = guiManager->GetDrawNormals();
	//meshGroupGround->m_isChangingNormalFlag = guiManager->GetChangingNormalFlag();

	///------위까지 정점 완료, 아래는 픽셸로 분류 하겠음(Riun)

	// gui의 값을 저장한다.
	m_materialDiffuse = guiManager->GetMaterialDiffuse();
	m_materialSpecular = guiManager->GetMaterialSpecular();

	// 빛의 종류와 값을 각각 gui에서 받아온다.
	m_lightType = guiManager->GetLightType();
	m_lightFromGUI = guiManager->GetLightFromGUI();

	// 라이트 업데이트 + 텍스처 사용 유무 + 노멀 스케일값
	for (auto e : m_meshGroupVec)
	{
		e->LightUpdate(m_materialDiffuse, m_materialSpecular, guiManager->GetPSBuffer().useTexture,
			guiManager->GetNBuffer().scale, m_lightType, m_lightFromGUI);
	}

	guiManager->UISetEndGUI();

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

	//렌더 타겟과 깊이 스텐실 설정
	m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	//래스터라이저 상태 설정
	if (m_isDrawAsWire) { m_pContext->RSSetState(m_pWireRasterizerSate.Get()); }
	else { m_pContext->RSSetState(m_pSolidRasterizerSate.Get()); }
}

void NestRenderer::DrawMesh()
{
	/// 물체 랜더링
	for (auto e : m_meshGroupVec)
	{
		e->Render(m_pDevice, m_pContext);
	}

	guiManager->RenderGUI();
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
	delete m_meshGroups;
	delete nestDevice;
	delete guiManager;
	delete camera;
}

/// 디바이스의 정보를 얻어오는 함수
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

/// 매개인자 [{std::vector<MeshData> meshes}, 포지션]
void NestRenderer::SetModelMeshData(NestGraphics::ModelData* _pModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys)
{
	// 받아온 오브젝트 배정

	/// 받아온 오브젝트만큼 오브젝트의 개수를 생성할까? 아니면 걍... 냅다 모델 벡터 가져와서 그만큼 또 돌려버리는 것도 방법일지도 ('ㅅ')
	/// 이렇게 바꾸게 되면 그냥 ObjectMaker와 다름 없다.. 흠.
	m_meshGroups = new MeshGroup;
	m_meshGroups->Initialize(m_pDevice, m_pContext, _pModelMeshData, trsMatrix, textureFileKeys);

	m_meshGroupVec.push_back(m_meshGroups);
}

void NestRenderer::CurrentMousePos(const int& mouseX, const int& mouseY)
{
	camera->OnMouseMove(m_clientHeight, m_clientWidth, mouseX, mouseY);
}

void NestRenderer::SetSkeltaModelMeshData(NestGraphics::ModelData* _pModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys)
{
	m_skelta = new SkeltaMeshGroup;
	m_skelta->Initialize(m_pDevice, m_pContext, _pModelMeshData, trsMatrix, textureFileKeys);

	m_meshGroupVec.push_back(m_skelta);
}

