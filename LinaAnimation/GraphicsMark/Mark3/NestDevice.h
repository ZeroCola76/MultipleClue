#pragma once
#include "D3dUtilities.h" // 이것도 건들지 마세요.
// 여기에 존재하는 것들은 가장 기본적인 장치로서, 그래픽스가 돌아가기 위해 필수적으로 필요한 장치들이라고 보면 된다.
class D3dUtilities;

class NestDevice
{
private:
	UINT m_numQualityLevels = 1; //깊이 버퍼 생성때 필요한 변수

	ComPtr<ID3D11Device> m_pD3dDevice;//그래픽 리소스를 생성 및 관리하기 위한 중요한 인터페이스
	ComPtr<ID3D11DeviceContext> m_pD3dContext; //dxcontext
	ComPtr<ID3D11RenderTargetView> m_pD3dRenderTargetView; // 현재 렌더하는 타겟 뷰의 개수를 설정한다.
	ComPtr<ID3D11ShaderResourceView> m_pD3dShaderResourceView; // 블러 같은 효과를 추가하기 위해 사용함.
	ComPtr<IDXGISwapChain> m_pD3dSwapChain; // 스왑 체인, GPU <-> CPU

	ComPtr<ID3D11RasterizerState> m_pD3dSolidRasterizerSate; // 솔리드용 레지스터 상태
	ComPtr<ID3D11RasterizerState> m_pD3dWireRasterizerSate; // 이것은 와이어 스테이트를 위한 것으로 솔리드 1개만 있어도 프로그램은 돌아간다.

	// Depth buffer 관련
	ComPtr<ID3D11Texture2D> m_pD3dDepthStencilBuffer; //말그대로 깊이 스텐실 버퍼
	ComPtr<ID3D11DepthStencilView> m_pD3dDepthStencilView; // 나중에 사용하려고 만들어놓음, 후에 기능 생성시 추가 주석 예정
	ComPtr<ID3D11DepthStencilState> m_pD3dDepthStencilState; // 나중에 사용하려고 만들어놓음, 후에 기능 생성시 추가 주석 예정

	D3D11_VIEWPORT m_pD3dScreenViewport;  // 뷰 포트

	D3dUtilities* m_pD3dUtilities;

	bool CreateRenderTargetView(); //렌더타겟이 만들어졌는지 체크
	void SetViewport(int m_screenWidth, int m_screenHeight); //뷰포트 세팅

public:
	NestDevice(); //디바이스와 컨텐스트 생성의 생성자
	~NestDevice(); //소멸자

	HRESULT InitializeDevice(int m_hWnd, int m_screenWidth, int m_screenHeight); //화면 초기화
	HRESULT Present(); //EndRender에서 해준다.

	UINT getNumQualityLevels() const { return m_numQualityLevels; } //깊이 버퍼와 관련된 get

	ComPtr<ID3D11Device> GetD3dDevice() const { return m_pD3dDevice; } //디바이스 get
	ComPtr<ID3D11DeviceContext> GetD3dContext() const { return m_pD3dContext; } //context get
	ComPtr<ID3D11RenderTargetView> GetD3dRenderTargetView() const { return m_pD3dRenderTargetView; } // 렌더타겟 get
	ComPtr<ID3D11ShaderResourceView> GetD3dShaderResourceView() const { return m_pD3dShaderResourceView; } // 렌더타겟 get
	ComPtr<IDXGISwapChain> GetD3dSwapChain() const { return m_pD3dSwapChain; } //스왑체인 get
	
	ComPtr<ID3D11RasterizerState> GetD3dSolidRasterizerSate() const { return m_pD3dSolidRasterizerSate; } //솔리드용 레지스터 get
	ComPtr<ID3D11RasterizerState> GetD3dWireRasterizerSate() const { return m_pD3dWireRasterizerSate; } //와이어 스테이트 get
	
	ComPtr<ID3D11Texture2D> getD3dDepthStencilBuffer()const { return m_pD3dDepthStencilBuffer; }; //깊이 스텐실 get
	ComPtr<ID3D11DepthStencilView> GetD3dDepthStencilView() const { return m_pD3dDepthStencilView; } //아직 사용x 
	ComPtr<ID3D11DepthStencilState> GetD3dDepthStencilState() const { return m_pD3dDepthStencilState; } //아직 사용 x
	D3D11_VIEWPORT GetD3dScreenViewport() const { return m_pD3dScreenViewport; } // 뷰포트 get
};
