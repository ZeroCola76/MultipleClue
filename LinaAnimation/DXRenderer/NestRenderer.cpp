#include "NestDevice.h"
#include "GUIManager.h"
#include "Camera.h"
#include "NestRenderer.h"


//dx11 ���̺귯��
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
	//hwnd �������� ���𿡼�
	m_hMainWnd = (HWND)hWnd;
	//�ػ� ������
	m_clientWidth = screenWidth;
	m_clientHeight = screenHeight;

	//device�� context ����
	nestDevice = new NestDevice();
	nestDevice->InitializeDevice(hWnd, screenWidth, screenHeight);
	GetInfoDevice();

	//���� ���� ���� //���ڴ� GetInfoDevice�Լ� ������ �ʱ�ȭ
	D3dUtilities::CreateDepthBuffer(m_pDevice, m_clientWidth, m_clientHeight, m_numQualityLevels, m_pDepthStencilView);

	/// �ν��Ͻ� ����
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

	// ī�޶��� ��ġ�� ���ӱ��̿� ������Ű�� �κ�
	m_localTranslation = guiManager->SetTranslation(camera->getLocalPosition());

	// ī�޶��� Ű �Է� (�̰� ������ �� ����..)
	if (GetAsyncKeyState('W') & 0x8000) { camera->MoveForward(10.0f * m_dTime); }
	if (GetAsyncKeyState('S') & 0x8000) { camera->MoveForward(-10.0f * m_dTime); }
	if (GetAsyncKeyState('A') & 0x8000) { camera->MoveRight(-10.0f * m_dTime); }
	if (GetAsyncKeyState('D') & 0x8000) { camera->MoveRight(10.0f * m_dTime); }
	if (GetAsyncKeyState('Q') & 0x8000) { camera->MoveUp(10.0f * m_dTime); }
	if (GetAsyncKeyState('E') & 0x8000) { camera->MoveUp(-10.0f * m_dTime); }
	

	// �� �κ� ���ְ�, imgui -> ī�޶� -> �׷��Ƚ��� �����͸� �ѱ�� ���� �ٲ� ����
	Matrix camViewRow = camera->GetViewRow();
	Matrix camProjRow = camera->GetProjRow();
	Vector3 camEyeWorld = camera->GetEyePos();

	for (auto e : m_meshGroupVec)
	{
		e->CamUpdate(m_dTime, m_pDevice, m_pContext, camViewRow, camProjRow, camEyeWorld);
	}
	
	///�̰� �� �ִ°���? �ڵ�ȸ����ų���߳�..?
	//static float rot = 0.0f;
	//rot += m_dTime;
	
	/*
	// ���� ��ȯ
	//meshGroupGround->objectVertexConstantData.world =
	//	Matrix::CreateRotationX(m_localRotation.x) * Matrix::CreateRotationY(m_localRotation.y) *
	//	Matrix::CreateRotationZ(m_localRotation.z) * Matrix::CreateTranslation(m_localTranslation);
	//meshGroupGround->objectVertexConstantData.world = meshGroupGround->objectVertexConstantData.world.Transpose();

	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.world;
	//meshGroupGround->objectVertexConstantData.invTranspose.Translation(Vector3(0.0f));
	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.invTranspose.Transpose().Invert();

	// ���� ��ȯ
	//meshgroupground->objectvertexconstantdata.view = xmmatrixlooktolh(m_vieweyepos, m_vieweyedir, m_viewup);
	//meshgroupground->objectpixelconstantdata.eyeworld =
	//    vector3::transform(vector3(0.0f), meshgroupground->objectvertexconstantdata.view.invert());
	//meshgroupground->objectvertexconstantdata.view = meshgroupground->objectvertexconstantdata.view.transpose();

	//m_viewEyePos = // gui ������ ���߿� ����Ǳ⶧���� �긦 �������� ���۵ȴ�..// �ӽ÷� �ϵ��ڵ��� -> ���߿� ��������
	//    Vector3(camEyeWorld.x, camEyeWorld.y, camEyeWorld.z + 1.0f);
	//m_viewEyeDir = Vector3(guiManager->GetViewEyeDir());

	//m_viewUp = Vector3(guiManager->GetViewUp());

	// ��������
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

	// ����� ���� gui���� �����´�.
	//meshGroupGround->objectNormalVertexConstantData = guiManager->GetNBuffer();
	//meshGroupGround->m_isDrawNormals = guiManager->GetDrawNormals();
	//meshGroupGround->m_isChangingNormalFlag = guiManager->GetChangingNormalFlag();

	///------������ ���� �Ϸ�, �Ʒ��� �ȼз� �з� �ϰ���(Riun)

	// gui�� ���� �����Ѵ�.
	m_materialDiffuse = guiManager->GetMaterialDiffuse();
	m_materialSpecular = guiManager->GetMaterialSpecular();

	// ���� ������ ���� ���� gui���� �޾ƿ´�.
	m_lightType = guiManager->GetLightType();
	m_lightFromGUI = guiManager->GetLightFromGUI();

	// ����Ʈ ������Ʈ + �ؽ�ó ��� ���� + ��� �����ϰ�
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
	///�Ϲ��� ������ ���� : ����Ʈ ���� -> VS ���� -> ������� ���� -> ���� Ÿ�� �� ���� ���ٽ� ���� -> ���� Ÿ�� Ŭ����

	// �������� ����Ʈ�� �����Ѵ�. (1����, m_pD3dScreenViewport ����)
	m_pContext->RSSetViewports(1, &m_pScreenViewport);

	// ��� �ʱ�ȭ ����(R, G, B, A)
	float clearColor[4] = { 17.0f / 255.0f, 51.0f / 255.0f, 85.0f / 255.0f, 1.0f };

	// ���� Ÿ�� �並 �����ϰ�, �ű⿡ �ʱ�ȭ ������ �־��ش�.
	m_pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);

	// ���� ���ٽ� ����(���� ���ٽ� ����, ���� | ���ٽ�, 1.0f, 0) // ���� 1.0f�� 0�� ���̿� ���ٽ��� �ʱ�ȭ ��
	m_pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//���� Ÿ�ٰ� ���� ���ٽ� ����
	m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	//�����Ͷ����� ���� ����
	if (m_isDrawAsWire) { m_pContext->RSSetState(m_pWireRasterizerSate.Get()); }
	else { m_pContext->RSSetState(m_pSolidRasterizerSate.Get()); }
}

void NestRenderer::DrawMesh()
{
	/// ��ü ������
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

/// ����̽��� ������ ������ �Լ�
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

/// �Ű����� [{std::vector<MeshData> meshes}, ������]
void NestRenderer::SetModelMeshData(NestGraphics::ModelData* _pModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys)
{
	// �޾ƿ� ������Ʈ ����

	/// �޾ƿ� ������Ʈ��ŭ ������Ʈ�� ������ �����ұ�? �ƴϸ� ��... ���� �� ���� �����ͼ� �׸�ŭ �� ���������� �͵� ��������� ('��')
	/// �̷��� �ٲٰ� �Ǹ� �׳� ObjectMaker�� �ٸ� ����.. ��.
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

