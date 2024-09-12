#include "GUIManager.h"

GUIManager::GUIManager(int width, int height, HWND hwnd,
	ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context)
{
	m_GuiWidth = width;
	m_GuiHeight = height;
	m_HWnd = hwnd;
	m_GUIDevice = device;
	m_GUIContext = context;
}

GUIManager::~GUIManager()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool GUIManager::InitializeGUI()
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.DisplaySize = ImVec2(float(m_GuiWidth), float(m_GuiHeight));
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	if (!ImGui_ImplDX11_Init(m_GUIDevice.Get(), m_GUIContext.Get()))
	{
		return false;
	}

	if (!ImGui_ImplWin32_Init(m_HWnd))
	{
		return false;
	}

	return true;
}

void GUIManager::UIUpdateGUI()
{
	ImGui::Checkbox("Use Texture", &m_guiPixelBuffer.useTexture);
	ImGui::Checkbox("use Perspective Projection", &m_IsusePerspectiveProjection);
	ImGui::Checkbox("Draw As Wire", &m_isDrawAsWire);
	ImGui::Checkbox("Draw Normals", &m_isDrawNormals);
	if (ImGui::SliderFloat("Normal scale", &m_NBuffer.scale, 0.0f, 1.0f))
	{
		m_changingNormalFlag = true;
	}

	ImGui::SliderFloat3("m_localTranslation", &m_localTranslation.x, -2.0f, 2.0f);
	ImGui::SliderFloat3("m_localRotation(Rad)", &m_localRotation.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("m_localScaling", &m_localScaling.x, 0.1f, 2.0f);

	ImGui::SliderFloat3("m_viewEyePos", &m_viewEyePos.x, -4.0f, 4.0f);
	ImGui::SliderFloat3("m_viewEyeDir", &m_viewEyeDir.x, -4.0f, 4.0f);
	ImGui::SliderFloat3("m_viewUp", &m_viewUp.x, -2.0f, 2.0f);

	ImGui::SliderFloat("m_projFovAngleY(Deg)", &m_projFovAngleY, 10.0f, 180.0f);
	ImGui::SliderFloat("m_nearZ", &m_nearZ, 0.01f, 10.0f);
	ImGui::SliderFloat("m_farZ", &m_farZ, 0.01f, 10.0f);
	// ImGui::SliderFloat("m_testAspect", &m_testAspect, 1.0f, 3.0f);
	ImGui::SliderFloat("Material Shininess", &m_guiPixelBuffer.material.shininess, 1.0f, 256.0f);

	if (ImGui::RadioButton("Directional Light", GetLightType() == 0))
	{
		m_lightType = 0;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Point Light", GetLightType() == 1))
	{
		m_lightType = 1;
	}
	ImGui::SameLine();
	if (ImGui::RadioButton("Spot Light", GetLightType() == 2))
	{
		m_lightType = 2;
	}

	ImGui::SliderFloat("Material Diffuse", &m_materialDiffuse, 0.0f, 1.0f);
	ImGui::SliderFloat("Material Specular", &m_materialSpecular, 0.0f, 1.0f);
	ImGui::SliderFloat3("Light Position", &m_lightFromGUI.position.x, -5.0f, 5.0f);
	ImGui::SliderFloat("Light fallOffStart", &m_lightFromGUI.fallOffStart, 0.0f, 5.0f);
	ImGui::SliderFloat("Light fallOffEnd", &m_lightFromGUI.fallOffEnd, 0.0f, 10.0f);
	ImGui::SliderFloat("Light spotPower", &m_lightFromGUI.spotPower, 1.0f, 512.0f);
}

void GUIManager::BeginGUI()
{
	ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame(); // 어떤 것들을 렌더링 할지 기록 시작
	ImGui::Begin("Scene Control");

	// ImGui가 측정해주는 Framerate 출력
	ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);
}

void GUIManager::UISetEndGUI() // ◆리나 : 혼동을 피하기 위해 함수 이름 변경했어요.
{
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f)); // ImGui를 0,0부터 그리도록 고정
	ImGui::End(); // UI 작성이 끝났음을 나타내고, 그 후의 렌더링 프로세스를 시작하기 때문에 Render 전에 넣어준다.
	ImGui::Render(); // 렌더링할 것들 기록 끝
}

void GUIManager::RenderGUI()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링
}



Vector3 GUIManager::GetEyeRot() { return m_localRotation; }



Matrix GUIManager::GetViewRow() { return Matrix::CreateTranslation(-m_localTranslation); }

Matrix GUIManager::SetViewRow(const Matrix& viewRow)
{
	Matrix::CreateTranslation(-m_localTranslation) = viewRow; return Matrix::CreateTranslation(-m_localTranslation);
}

Matrix GUIManager::GetProjRow()
{
	return GetIsusePerspectiveProjection()
		? DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(GetProY()),	m_aspect, GetNearZ(), 
			GetFarZ())
		: DirectX::XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, GetNearZ(), GetFarZ());
}



Matrix GUIManager::SetProjRow(const float& proY, const float& nearZ, const float& farZ)
{
	return GetIsusePerspectiveProjection()
		? DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(proY), m_aspect, nearZ,
			farZ)
		: DirectX::XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, nearZ, farZ);
}

Vector3 GUIManager::GetEyePos() { return m_viewEyePos; }

Vector3 GUIManager::SetEyePos(const Vector3& eyePos)
{
	m_viewEyePos = eyePos;
	return m_viewEyePos;
}

NestGraphics::PixelShaderConstantBuffer GUIManager::GetPSBuffer() { return m_guiPixelBuffer; }

NestGraphics::NormalShaderConstantBuffer GUIManager::GetNBuffer() { return m_NBuffer; }

