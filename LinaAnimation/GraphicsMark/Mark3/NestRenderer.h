#pragma once
#include "IRenderer.h"
#include "Geometry.h" // cpp로 옮기지 말것
#include "CubeMapping.h"
#include "ObjectProcessor.h"

class Camera;
class GUIManager;
class NestDevice;
class CubeMapping;
class ObjectProcessor;
class TRSComponent;

/// <summary>
/// 렌더러 함수 : 인터페이스의 상속을 받는다.
/// 2024.2.6. 대략적인 정리를 끝내고, 마우스 피킹 작업을 시작함.
/// </summary>
class NestRenderer : public IRenderer
{
public:
	NestRenderer();
	~NestRenderer();

private:

	///일단 이건 필요하다. 그려지는 메쉬들
	// 자동으로 그려지게 하기 위해 테스트를 진행 중임
	ObjectProcessor* m_objectProcessor;
	std::vector<ObjectProcessor*> m_objectProcessorVec;

	// 모델 데이터 벡터 받아오기 위한 포인터
	NestGraphics::ModelData* m_pModelsData;

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
	NestDevice* m_pNestDevice;
	// 카메라
	Camera* m_pCamera;
	// 아임구이 매니저
	GUIManager* m_pGuiManager;
	// 큐브 매핑
	CubeMapping* m_cubeMapping;
	// 자주 사용하는 함수들의 묶음
	D3dUtilities* m_pD3dUtilities;
	// 임시로 만든 것
	TRSComponent* m_pMyTrsComponent;
	
public:
	// 화면의 출력과 관련된 변수들
	int m_clientWidth; // 화면 가로
	int m_clientHeight;    // 화면 세로
	HWND m_hMainWnd;       // 핸들
	float m_aspect = GetAspectRatio(); // 화면 비율

	//임시
	Matrix tempTRSMat;

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
	Matrix m_camViewRow = Matrix();
	Matrix m_camProjRow = Matrix();
	Vector3 m_camEyeWorld = Vector3(0.0f);

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
	virtual void SetModelMeshData(NestGraphics::ModelData*, Matrix _trsMatrix, std::initializer_list<std::wstring> _textureFileKeys) override;
	virtual void SetCubeMeshData(NestGraphics::ModelData* _ModelMeshData, std::wstring _originalkey,
									std::wstring _diffusekey, std::wstring _specularkey) override;

	// 마우스 위치를 가져오는 함수
	virtual void CurrentMousePos(const int& _mouseX, const int& _mouseY) override;

	// 현재 키의 값을 카메라에 전달하기 위한 함수
	virtual void CurrentKeyPressed(int keyCode) override;

    // 화면 비율을 결정하는 함수
    float GetAspectRatio() /*const*/ {return float(m_clientWidth)/m_clientHeight;}

public:
	//임시 변수
	float m_myDTime;
};