#pragma once
#include "ConstantBuffer.h"
#include "D3dUtilities.h" // �׳� ��� ������ ���� �־�� ��... Imgui�� �ᱹ ���� �Ŵϱ�, ���Ƿ� �־�д�.
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

	NestGraphics::PixelShaderConstantBuffer m_guiPixelBuffer; //�ȼ� ����
	bool m_IsusePerspectiveProjection = true;// ���� ���� ��� �ʱⰪ : �����
	bool m_isDrawAsWire = false; //���̾ �׸��°�?
	bool m_isDrawNormals = false; //����� �׸��°�?
	NestGraphics::NormalShaderConstantBuffer m_NBuffer; //��ֻ������
	bool m_changingNormalFlag = false;
	Vector3 m_localTranslation = Vector3(0.0f);					//��ü �̵�
	Vector3 m_localRotation = Vector3(1.0f, 0.5f, 0.0f);		// ��ü ȸ��
	Vector3 m_localScaling = Vector3(0.5f);						// ��ü ũ�� : Grid�� ������ �� ���õ�
	Vector3 m_viewEyePos = { 0.0f, 0.0f, -2.0f }; // ���� ���� ��ġ
	Vector3 m_viewEyeDir = { 0.0f, 0.0f, 1.0f };// ���� ���� �� -> ��ü�� ��ġ ������ �����̴� ��ó�� ������
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;// m_nearZ : ���� ����� ���
	float m_farZ = 100.0f;//m_farZ ���� �� ���

	int m_lightType = 0; //���� Ÿ��
	NestGraphics::Light m_lightFromGUI; //��
	float m_materialDiffuse = 1.0f; //���� ��ǻ�
	float m_materialSpecular = 1.0f; //���� ����ŧ��

	float m_aspect = 16.0f / 9.0f; // ���� / ���� ȭ�� ����

public:
	GUIManager(int width, int height, HWND hwnd, ComPtr<ID3D11Device> device,
		ComPtr<ID3D11DeviceContext> context);
	~GUIManager();

	bool InitializeGUI();
	void UIUpdateGUI(); // ����ϴ� ���� ������ �߰�
	// virtual bool WinGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void BeginGUI(); //gui ������ ���� �� ������ ǥ��
	void UISetEndGUI(); //UI�� �����ϴ� ���� ������ �������� �ʿ��� ������ �Է�
	void RenderGUI(); // ������

public:
	//������
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
