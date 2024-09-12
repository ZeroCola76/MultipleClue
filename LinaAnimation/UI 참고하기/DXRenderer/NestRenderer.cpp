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

    ///��ü ���� 
	meshGroupGround = new MeshGroup();
    ///�̰� ���� �̻���! �迹���� TO DO : ��ü�� �����ϴ� �� ���� ������ ĳ��Ʈ���� �ؾ��ϸ� �̰� ȿ���� �ƴѰŰ���
    MeshGroup* boxptr = new Box();
    MeshGroup* sphereptr = new Sphere();
	meshGroupBox = dynamic_cast<Box*>(boxptr);
	meshGroupSphere = dynamic_cast<Sphere*>(sphereptr);

    ///���丮..
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

	// �ٴ����� ����ϱ� ���� ȸ��
	Matrix modelMat = Matrix::CreateRotationX(DirectX::XM_PIDIV2); //DirectX::XM_PIDIV2
	Matrix invTransposeRow = modelMat;
	invTransposeRow.Translation(Vector3(0.0f));
	invTransposeRow = invTransposeRow.Invert().Transpose();

	// ConstantBuffer �ʱ�ȭ (�ٴ��� ���� ����)
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

	// ī�޶��� Ű �Է� (�̰� ������ �� ����..)
	if (GetAsyncKeyState('W') & 0x8000) { camera->MoveForward(10.0f * m_dTime); }
	if (GetAsyncKeyState('S') & 0x8000) { camera->MoveForward(-10.0f * m_dTime); }
	if (GetAsyncKeyState('A') & 0x8000) { camera->MoveRight(-10.0f * m_dTime); }
	if (GetAsyncKeyState('D') & 0x8000) { camera->MoveRight(10.0f * m_dTime); }


	// �� �κ� ���ְ�, imgui -> ī�޶� -> �׷��Ƚ��� �����͸� �ѱ�� ���� �ٲ� ����
	Matrix camViewRow = camera->GetViewRow();
	Matrix camProjRow = camera->GetProjRow();
	Vector3 camEyeWorld = camera->GetEyePos();

	//Matrix = guiManager->GetViewRow();
	//Matrix guiProjRow = guiManager->GetProjRow();
	//Vector3 = guiManager->GetEyePos();

	///ť����� ����
	//cubeMapping->UpdateConstantBuffers(m_pDevice, m_pContext, camViewRow.Transpose(), camProjRow.Transpose());
	cubeMapping->UpdateConstantBuffers(m_pDevice, m_pContext, camViewRow.Transpose(), camProjRow.Transpose());

	meshGroupGround->objectPixelConstantData.useTexture = true;
	meshGroupGround->objectPixelConstantData.eyeWorld = camEyeWorld;
	meshGroupGround->objectVertexConstantData.view = camViewRow.Transpose();
	meshGroupGround->objectVertexConstantData.projection = camProjRow.Transpose();

	/// ��ü ��ġ
	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        {
            pObject->Update(camEyeWorld, camViewRow, camProjRow);
        }
	}
	
    ///��ü ����
	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        { 
		pObject->UpdateConstantBuffers(m_pDevice, m_pContext);
        }
	}

	static float rot = 0.0f;
	rot += m_dTime;

	// ���� ��ȯ
	meshGroupGround->objectVertexConstantData.world =
		Matrix::CreateRotationX(m_localRotation.x) * Matrix::CreateRotationY(m_localRotation.y) *
		Matrix::CreateRotationZ(m_localRotation.z) * Matrix::CreateTranslation(m_localTranslation);
	meshGroupGround->objectVertexConstantData.world = meshGroupGround->objectVertexConstantData.world.Transpose();

	//m_localTranslation = guiManager->GetTranslation();
	//m_localRotation = guiManager->GetEyeRot();

	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.world;
	//meshGroupGround->objectVertexConstantData.invTranspose.Translation(Vector3(0.0f));
	//meshGroupGround->objectVertexConstantData.invTranspose = meshGroupGround->objectVertexConstantData.invTranspose.Transpose().Invert();

	// ���� ��ȯ
	//meshGroupGround->objectVertexConstantData.view = XMMatrixLookToLH(m_viewEyePos, m_viewEyeDir, m_viewUp);
	//meshGroupGround->objectPixelConstantData.eyeWorld =
	//    Vector3::Transform(Vector3(0.0f), meshGroupGround->objectVertexConstantData.view.Invert());
	//meshGroupGround->objectVertexConstantData.view = meshGroupGround->objectVertexConstantData.view.Transpose();

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

	// ����� ���� gui���� �����´�.
	meshGroupGround->objectNormalVertexConstantData = guiManager->GetNBuffer();

	meshGroupGround->m_isDrawNormals = guiManager->GetDrawNormals();
	m_isDrawAsWire = guiManager->GetDrawAsWire();
	meshGroupGround->m_isChangingNormalFlag = guiManager->GetChangingNormalFlag();

	///------������ ���� �Ϸ�, �Ʒ��� �ȼз� �з� �ϰ���(Riun)

	// gui�� ���� �����Ѵ�.
	//m_materialDiffuse = guiManager->GetMaterialDiffuse();
	//m_materialSpecular = guiManager->GetMaterialSpecular();

	//// �ȼ� ���� �����Ϳ� ��ǻ��� ����ŧ�� ���� �����Ѵ�.
	// meshGroupGround->objectPixelConstantData.material.diffuse = Vector3(m_materialDiffuse);
	// meshGroupGround->objectPixelConstantData.material.specular = Vector3(m_materialSpecular);
	// meshGroupGround->objectPixelConstantData.useTexture = guiManager->GetPSBuffer().useTexture;
	// meshGroupGround->objectNormalVertexConstantData.scale = guiManager->GetNBuffer().scale;

	// ���� ������ ���� ���� gui���� �޾ƿ´�.
	m_lightType = guiManager->GetLightType();
	m_lightFromGUI = guiManager->GetLightFromGUI();

	// ���� �� ���� ����ϱ� ����, ������� �ʴ� ������ ���� ������ 0���� �����ؼ� �׸�
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		// �ٸ� ���� ����
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

	/// TODO[Riun] : ����� �ٲ� ���� Update�ϵ��� �ٲ��ش�. (if�� Ȱ���ϱ�)[ť�����][2]

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
}

void NestRenderer::DrawSomething()
{
	BeginRender();

	m_pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);

	if (m_isDrawAsWire)	{m_pContext->RSSetState(m_pWireRasterizerSate.Get());}
	else {m_pContext->RSSetState(m_pSolidRasterizerSate.Get());}

    /// ��ü ������
    meshGroupGround->Render(m_pContext);
	for (MeshGroup* pObject : m_pDXObjects)
	{
        if (pObject != nullptr)
        {
		pObject->Render(m_pContext);

        }
	}

	// ��ü ������ �� ť�����
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
/// ����̽��� ������ ������ �Լ�
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
