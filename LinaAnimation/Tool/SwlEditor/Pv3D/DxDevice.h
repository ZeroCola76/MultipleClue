#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11�� Device�� DeviceContext ����
/// Mass���� ��������� �׳� ���÷� ���������� ����Ұ�
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

