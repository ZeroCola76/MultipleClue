#pragma once
#include "ConstantBuffer.h"
#include "D3dUtilities.h" // 그냥 헤더 파일을 위해 넣어둔 거... Imgui는 결국 버릴 거니까, 임의로 넣어둔다.
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

//using namespace graphicUse;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam);

class GUIManager
{
private:
	int m_GuiWidth;
	int m_GuiHeight;
	HWND m_HWnd;
	ComPtr<ID3D11Device> m_GUIDevice;
	ComPtr<ID3D11DeviceContext> m_GUIContext;

	NestGraphics::PixelShaderConstantBuffer m_guiPixelBuffer; //픽셀 버퍼
	bool m_IsusePerspectiveProjection = true;// 원근 투영 사용 초기값 : 사용함
	bool m_isDrawAsWire = false; //와이어를 그리는가?
	bool m_isDrawNormals = false; //노멀을 그리는가?
	NestGraphics::NormalShaderConstantBuffer m_NBuffer; //노멀상수버퍼
	bool m_changingNormalFlag = false;
	Vector3 m_localTranslation = Vector3(0.0f);					//물체 이동
	Vector3 m_localRotation = Vector3(1.0f, 0.5f, 0.0f);		// 물체 회전
	Vector3 m_localScaling = Vector3(0.5f);						// 물체 크기 : Grid는 스케일 값 무시됨
	Vector3 m_viewEyePos = { 0.0f, 0.0f, -2.0f }; // 현재 눈의 위치
	Vector3 m_viewEyeDir = { 0.0f, 0.0f, 1.0f };// 고개를 돌릴 때 -> 물체가 마치 구면을 움직이는 것처럼 움직임
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;// m_nearZ : 가장 가까운 평면
	float m_farZ = 100.0f;//m_farZ 가장 먼 평면

	int m_lightType = 0; //빛의 타입
	NestGraphics::Light m_lightFromGUI; //빛
	float m_materialDiffuse = 1.0f; //빛의 디퓨즈값
	float m_materialSpecular = 1.0f; //빛의 스펙큘러

	float m_aspect = 16.0f / 9.0f; // 가로 / 세로 화면 비율

public:
	GUIManager(int width, int height, HWND hwnd, ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context);
	~GUIManager();

	bool InitializeGUI();
	void UIUpdateGUI(); // 사용하는 툴의 조정값 추가
	// virtual bool WinGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void BeginGUI(); //gui 프레임 시작 및 프레임 표기
	void UISetEndGUI(); //UI를 셋팅하는 것을 끝내고 렌더링에 필요한 데이터 입력
	void RenderGUI(); // 렌더링

public:
	//값조절
	Matrix GetViewRow(); //Matrix::CreateTranslation(-m_localTranslation)
	Matrix SetViewRow(const Matrix& viewRow);
	Matrix GetProjRow();
	Matrix SetProjRow(const float& proY, const float& nearZ, const float& farZ);
	Vector3 GetEyePos();
	Vector3 SetEyePos(const Vector3& eyePos);
	Vector3 GetEyeRot();
	NestGraphics::PixelShaderConstantBuffer GetPSBuffer();
	NestGraphics::NormalShaderConstantBuffer GetNBuffer();
	int GetLightType() const { return m_lightType; }
	float GetMaterialDiffuse() const { return m_materialDiffuse; }
	float GetMaterialSpecular() const { return m_materialSpecular; }
	void MaterialSpecular(float val) { m_materialSpecular = val; }
	NestGraphics::Light GetLightFromGUI() const { return m_lightFromGUI; }
	Vector3 GetTranslation() const { return m_localTranslation; }
	Vector3 SetTranslation(const Vector3& localTranslation) { m_localTranslation = localTranslation; return m_localTranslation; }
	Vector3 GetViewEyeDir() const { return m_viewEyeDir; }
	Vector3 GetViewUp() const { return m_viewUp; }
	float GetProY() const { return m_projFovAngleY; }
	float GetNearZ() const { return m_nearZ; }
	float GetFarZ() const { return m_farZ; }
	bool GetIsusePerspectiveProjection() const { return m_IsusePerspectiveProjection; }
	bool GetDrawAsWire() const { return m_isDrawAsWire; }
	bool GetDrawNormals() const { return m_isDrawNormals; }
	bool GetChangingNormalFlag() const { return m_changingNormalFlag; }
};
