#pragma once
#include "DXDefine.h"
// 여기에 존재하는 것들은 가장 기본적인 장치로서, 그래픽스가 돌아가기 위해 필수적으로 필요한 장치들이라고 보면 된다.
class NestDevice
{
private:
	D3D_DRIVER_TYPE m_DriverType;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	UINT numQualityLevels = 1;
	ComPtr<ID3D11Texture2D> m_pD3dDepthStencilBuffer;

	ComPtr<ID3D11Device> m_pD3dDevice;//그래픽 리소스를 생성 및 관리하기 위한 중요한 인터페이스
	ComPtr<ID3D11DeviceContext> m_pD3dContext;
	ComPtr<ID3D11RenderTargetView> m_pD3dRenderTargetView; // 현재 렌더하는 타겟 뷰의 개수를 설정한다.
	ComPtr<ID3D11DepthStencilView> m_pD3dDepthStencilView; // 나중에 사용하려고 만들어놓음, 후에 기능 생성시 추가 주석 예정
	ComPtr<ID3D11RasterizerState> m_pD3dSolidRasterizerSate; // 솔리드용 레지스터 상태
	ComPtr<ID3D11RasterizerState> m_pD3dWireRasterizerSate; // 이것은 와이어 스테이트를 위한 것으로 솔리드 1개만 있어도 프로그램은 돌아간다.
	ComPtr<ID3D11DepthStencilState> m_pD3dDepthStencilState; // 나중에 사용하려고 만들어놓음, 후에 기능 생성시 추가 주석 예정
	ComPtr<IDXGISwapChain> m_pD3dSwapChain; // 스왑 체인, GPU <-> CPU 
	D3D11_VIEWPORT m_pD3dScreenViewport;  // 말 그대로 뷰 포트

	bool CreateRenderTargetView();
	void SetViewport(int m_screenWidth, int m_screenHeight);

public:
	NestDevice();
	~NestDevice();

	HRESULT InitializeDevice(int m_hWnd, int m_screenWidth, int m_screenHeight);
	HRESULT Present();

	UINT getNumQualityLevels() const { return numQualityLevels; }
	ComPtr<ID3D11Texture2D> getD3dDepthStencilBuffer()const { return m_pD3dDepthStencilBuffer; };
	ComPtr<ID3D11Device> GetD3dDevice() const { return m_pD3dDevice; }
	ComPtr<ID3D11DeviceContext> GetD3dContext() const { return m_pD3dContext; }
	ComPtr<ID3D11RenderTargetView> GetD3dRenderTargetView() const { return m_pD3dRenderTargetView; }
	ComPtr<ID3D11DepthStencilView> GetD3dDepthStencilView() const { return m_pD3dDepthStencilView; }
	ComPtr<ID3D11RasterizerState> GetD3dSolidRasterizerSate() const { return m_pD3dSolidRasterizerSate; }
	ComPtr<ID3D11RasterizerState> GetD3dWireRasterizerSate() const { return m_pD3dWireRasterizerSate; }
	ComPtr<ID3D11DepthStencilState> GetD3dDepthStencilState() const { return m_pD3dDepthStencilState; }
	D3D11_VIEWPORT GetD3dScreenViewport() const { return m_pD3dScreenViewport; }
	ComPtr<IDXGISwapChain> GetD3dSwapChain() const { return m_pD3dSwapChain; }
};
