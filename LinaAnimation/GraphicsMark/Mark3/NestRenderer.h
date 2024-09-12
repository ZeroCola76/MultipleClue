#pragma once
#include "IRenderer.h"
#include "Geometry.h" // cpp�� �ű��� ����
#include "CubeMapping.h"
#include "ObjectProcessor.h"

class Camera;
class GUIManager;
class NestDevice;
class CubeMapping;
class ObjectProcessor;
class TRSComponent;

/// <summary>
/// ������ �Լ� : �������̽��� ����� �޴´�.
/// 2024.2.6. �뷫���� ������ ������, ���콺 ��ŷ �۾��� ������.
/// </summary>
class NestRenderer : public IRenderer
{
public:
	NestRenderer();
	~NestRenderer();

private:

	///�ϴ� �̰� �ʿ��ϴ�. �׷����� �޽���
	// �ڵ����� �׷����� �ϱ� ���� �׽�Ʈ�� ���� ����
	ObjectProcessor* m_objectProcessor;
	std::vector<ObjectProcessor*> m_objectProcessorVec;

	// �� ������ ���� �޾ƿ��� ���� ������
	NestGraphics::ModelData* m_pModelsData;

	// ����̽��� ���õ� ����
	UINT m_numQualityLevels;
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pContext;
	D3D11_VIEWPORT m_pScreenViewport;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11RasterizerState> m_pSolidRasterizerSate;
	ComPtr<ID3D11RasterizerState> m_pWireRasterizerSate;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	NestDevice* m_pNestDevice;
	// ī�޶�
	Camera* m_pCamera;
	// ���ӱ��� �Ŵ���
	GUIManager* m_pGuiManager;
	// ť�� ����
	CubeMapping* m_cubeMapping;
	// ���� ����ϴ� �Լ����� ����
	D3dUtilities* m_pD3dUtilities;
	// �ӽ÷� ���� ��
	TRSComponent* m_pMyTrsComponent;
	
public:
	// ȭ���� ��°� ���õ� ������
	int m_clientWidth; // ȭ�� ����
	int m_clientHeight;    // ȭ�� ����
	HWND m_hMainWnd;       // �ڵ�
	float m_aspect = GetAspectRatio(); // ȭ�� ����

	//�ӽ�
	Matrix tempTRSMat;

public:
	/// <summary>
	/// IMGUI ����
	/// </summary>
	// ���� imgui ����
	bool m_isChangingNormalFlag = true;
	int m_lightType = 0;
	// ���� imgui ���� // �� �κ��� ���ӱ��� -> (ī�޶�) -> �׷��Ƚ��� �޾ƿ� ������, ���� camera.h�� ���� ���� �����ִ�.
	bool m_usePerspectiveProjection = true;
	bool m_isDrawNormals = true;
	bool m_isDrawAsWire = false; // ���̾�� �׸� ����, �ָ���� �׸� ���� üũ�ϴ� ����
	Vector3 m_localTranslation = Vector3(0.0f);
	Vector3 m_localRotation = Vector3(1.0f, 0.5f, 0.0f);
	Vector3 m_localScaling = Vector3(0.5f);
	Vector3 m_viewEyePos = { 0.0f, 0.0f, -2.0f };
	Vector3 m_viewEyeDir = { 0.0f, 0.0f, 1.0f };
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;
	// ����Ʈ
	NestGraphics::Light m_lightFromGUI;
	float m_materialDiffuse = 1.0f;
	float m_materialSpecular = 1.0f;
	// �þ� ����
	Matrix m_camViewRow = Matrix();
	Matrix m_camProjRow = Matrix();
	Vector3 m_camEyeWorld = Vector3(0.0f);

public:
	// ���� ���� �Լ�
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) override;
	virtual bool Update(float m_dTime) override;
	virtual void BeginRender() override;
	virtual void DrawMesh() override;
	virtual void EndRender() override;
	virtual void Finalize() override;

	// ����̽��� ������ �������� �Լ�
	void GetInfoDevice();

	// �� �������� meshData ���� �޾ƿ��� �Լ�
	virtual void SetModelMeshData(NestGraphics::ModelData*, Matrix _trsMatrix, std::initializer_list<std::wstring> _textureFileKeys) override;
	virtual void SetCubeMeshData(NestGraphics::ModelData* _ModelMeshData, std::wstring _originalkey,
									std::wstring _diffusekey, std::wstring _specularkey) override;

	// ���콺 ��ġ�� �������� �Լ�
	virtual void CurrentMousePos(const int& _mouseX, const int& _mouseY) override;

	// ���� Ű�� ���� ī�޶� �����ϱ� ���� �Լ�
	virtual void CurrentKeyPressed(int keyCode) override;

    // ȭ�� ������ �����ϴ� �Լ�
    float GetAspectRatio() /*const*/ {return float(m_clientWidth)/m_clientHeight;}

public:
	//�ӽ� ����
	float m_myDTime;
};