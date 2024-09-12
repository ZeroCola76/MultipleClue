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
	Sphere* meshGroupSphere;
	Box* meshGroupBox;
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

public:
	// 디바이스의 정보를 가져오는 함수
	void GetInfoDevice();

	// 렌더 관련 함수
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) override;
	virtual bool Update(float m_dTime) override;
	virtual void BeginRender() override;
	//virtual void DrawMesh() override;
	/*virtual void GetGeometry(std::vector<NestGraphics::Vertex> m_vertece,
		std::vector<uint16_t> m_indices, std::string m_string, std::string m_textureStr,
		ComPtr<ID3D11Buffer> m_vBuffer, ComPtr<ID3D11Buffer> m_iBuffer, ComPtr<ID3D11Texture2D> m_texture,
		ComPtr<ID3D11ShaderResourceView> m_textureView, ComPtr<ID3D11Buffer> m_vConstBuffer,
		ComPtr<ID3D11Buffer> m_pConstBuffer) override;*/
	virtual void DrawSomething() override;
	virtual void EndRender() override;
	virtual void Finalize() override;

    // 화면 비율을 결정하는 함수
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) override;
    float GetAspectRatio() /*const*/ {return float(m_clientWidth)/m_clientHeight;}
};