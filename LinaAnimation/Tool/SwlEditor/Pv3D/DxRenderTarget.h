#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11의 RenderTarget
/// MRT를 위해 클래스로 만들었다.
/// Device로 묶은 Com객체들을 각각 포인터로 가지고 있을지, 그냥 Device객체만 포인터로 가지고 있을지 고민할것
/// ->RenderTargetView와 DepthStencilView를 만드는데 각각 필요하기 때문에 Com객체들을 각각 포인터로 가지도록하자
/// 렌더러 내부에서 N개의 뷰포트를 가지고 있을것인데, D3D11_VIEWPORT을 멤버로 가지고 있을 이유가 있을지 고민할것
/// -> 없는듯? 그냥 viewport도 클래스로 묶어둠
/// MRT를 위해 RenderTargetView를 어디서 관리할지 고민할것
/// </summary>
class DxRenderTarget
{
public:
	DxRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, IDXGISwapChain* pSwapChain, int width, int height);
	~DxRenderTarget();

	void OnBeginRender(const float* color);
	void SetRenderTarget();
	void SetDepthStencilView();
	void ClearRenderTargetView(const float* color);
	void ClearDepthStencilView();

	ID3D11RenderTargetView* GetRenderTargetView() const { return m_pRenderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_pDepthStencilView.Get(); }

private:
	void CreateTexture();
	void CreateRenderTargetView();
	void CreateShaderResourceView();
	void CreateDepthStencilView();

private:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11Texture2D> m_pTextureBuffer;
	ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;

	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;

	int m_width;
	int m_height;

	// D3D11_VIEWPORT m_viewport;
};

