#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11의 Device와 DeviceContext 생성
/// Mass관련 멤버변수를 그냥 로컬로 던져버릴지 고민할것
/// </summary>
class DxDevice
{
public:
	DxDevice();
	~DxDevice();

	bool CreateDevice(int hWnd, int screenWidth, int screenHeight);
	void Present();

	ID3D11Device* GetDevice() const { return m_pDevice.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return m_pDeviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() const { return m_pSwapChain.Get(); }

private:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;

	D3D_FEATURE_LEVEL m_featureLevel;

};

