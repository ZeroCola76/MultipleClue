#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11�� RenderTarget
/// MRT�� ���� Ŭ������ �������.
/// Device�� ���� Com��ü���� ���� �����ͷ� ������ ������, �׳� Device��ü�� �����ͷ� ������ ������ ����Ұ�
/// ->RenderTargetView�� DepthStencilView�� ����µ� ���� �ʿ��ϱ� ������ Com��ü���� ���� �����ͷ� ������������
/// ������ ���ο��� N���� ����Ʈ�� ������ �������ε�, D3D11_VIEWPORT�� ����� ������ ���� ������ ������ ����Ұ�
/// -> ���µ�? �׳� viewport�� Ŭ������ �����
/// MRT�� ���� RenderTargetView�� ��� �������� ����Ұ�
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

