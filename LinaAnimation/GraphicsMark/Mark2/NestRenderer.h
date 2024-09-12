#pragma once
#include "DXDefine.h"
#include "IRenderer.h"
#include "Geometry.h" // cpp로 옮기지 말것
#include "CubeMapping.h"  // cpp로 옮기지 말것
#include "MeshGroup.h"

class Camera;
class GUIManager;
class NestDevice;
class NestDXObject;
class NestGraphicFactory;
class GraphicsResourceManager;
class CubeMapping;
class MeshGroup;
class GameObject;
class Box;
class Sphere;

/// <summary>
/// 렌더러 함수 : 인터페이스의 상속을 받는다.
/// 2024.1.24. 순서 끝.
/// </summary>
class NestRenderer : public IRenderer
{
public:
	NestRenderer();
	~NestRenderer();

private:
	//팩토리(아직 미구현)
	NestGraphicFactory* m_NestFactory;
	//오브젝트와 관련된 변수
	std::vector<MeshGroup*> m_pDXObjects;
	///일단 이건 필요하다. 그려지는 메쉬들
	MeshGroup* meshGroupGround;
	MeshGroup* meshGroupSphere;
	MeshGroup* meshGroupBox;
	// 큐브 메쉬 데이터를 받아오기 위한 포인터
	std::vector<NestGraphics::MeshData>* m_CubeMeshData;
	// 매쉬 데이터를 받아오기 위한 포인터
	std::vector<NestGraphics::MeshData>* m_ModelMeshData;
	std::wstring m_modelFilePath;
	float m_modelScale;
	// 디바이스와 관련된 변수
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
	NestDevice* nestDevice;
	// 카메라
	Camera* camera;
	// 아임구이 매니저
	GUIManager* guiManager;
	// 큐브 매핑
	CubeMapping* cubeMapping;

public:
	// 화면의 출력과 관련된 변수들
	int m_clientWidth; // 화면 가로
	int m_clientHeight;    // 화면 세로
	HWND m_hMainWnd;       // 핸들
	float m_aspect = GetAspectRatio(); // 화면 비율

public:
	/// <summary>
	/// IMGUI 변수
	/// </summary>
	// 간접 imgui 변수
	bool m_isChangingNormalFlag = true;
	int m_lightType = 0;
	// 직접 imgui 변수 // 이 부분은 아임구이 -> (카메라) -> 그래픽스로 받아올 예정임, 현재 camera.h에 변수 설명 적혀있다.
	bool m_usePerspectiveProjection = true;
	bool m_isDrawNormals = true;
	bool m_isDrawAsWire = false; // 와이어로 그릴 건지, 솔리드로 그릴 건지 체크하는 변수
	Vector3 m_localTranslation = Vector3(0.0f);
	Vector3 m_localRotation = Vector3(1.0f, 0.5f, 0.0f);
	Vector3 m_localScaling = Vector3(0.5f);
	Vector3 m_viewEyePos = { 0.0f, 0.0f, -2.0f };
	Vector3 m_viewEyeDir = { 0.0f, 0.0f, 1.0f };
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;
	// 라이트
	NestGraphics::Light m_lightFromGUI;
	float m_materialDiffuse = 1.0f;
	float m_materialSpecular = 1.0f;
	// 시야 관련
	Matrix camViewRow = Matrix();
	Matrix camProjRow = Matrix();
	Vector3 camEyeWorld = Vector3(0.0f);

public:
	// 렌더 관련 함수
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) override;
	virtual bool Update(float m_dTime) override;
	virtual void BeginRender() override;
	virtual void DrawMesh() override;
	virtual void EndRender() override;
	virtual void Finalize() override;

	// 디바이스의 정보를 가져오는 함수
	void GetInfoDevice();

	// 모델 데이터의 meshData 값을 받아오는 함수
	virtual void SetModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) override;
	virtual void SetCubeMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) override;
	//virtual void SetMeshModelFilePath(std::vector<NestGraphics::MeshData>* _ModelMeshData, const std::wstring& _filePath, const float& _scale) override;

	// 마우스 위치를 가져오는 함수
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) override;

    // 화면 비율을 결정하는 함수
    float GetAspectRatio() /*const*/ {return float(m_clientWidth)/m_clientHeight;}
	// 아임구이 정보를 업데이트하는 함수
	void ImGuiUpdate();
	// 카메라 정보를 업데이트하는 함수
	void CameraUpdate();

public:
	// 받아온 모델메쉬데이터를 다른 클래스에 보내주는 함수
	std::vector<NestGraphics::MeshData>* getModelMeshData() const { return m_ModelMeshData; };
};