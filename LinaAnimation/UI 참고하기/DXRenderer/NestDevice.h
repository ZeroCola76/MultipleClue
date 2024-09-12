#pragma once
#include "DXDefine.h"
// ���⿡ �����ϴ� �͵��� ���� �⺻���� ��ġ�μ�, �׷��Ƚ��� ���ư��� ���� �ʼ������� �ʿ��� ��ġ���̶�� ���� �ȴ�.
class NestDevice
{
private:
	D3D_DRIVER_TYPE m_DriverType;
	D3D_FEATURE_LEVEL m_FeatureLevel;
	UINT numQualityLevels = 1;
	ComPtr<ID3D11Texture2D> m_pD3dDepthStencilBuffer;

	ComPtr<ID3D11Device> m_pD3dDevice;//�׷��� ���ҽ��� ���� �� �����ϱ� ���� �߿��� �������̽�
	ComPtr<ID3D11DeviceContext> m_pD3dContext;
	ComPtr<ID3D11RenderTargetView> m_pD3dRenderTargetView; // ���� �����ϴ� Ÿ�� ���� ������ �����Ѵ�.
	ComPtr<ID3D11DepthStencilView> m_pD3dDepthStencilView; // ���߿� ����Ϸ��� ��������, �Ŀ� ��� ������ �߰� �ּ� ����
	ComPtr<ID3D11RasterizerState> m_pD3dSolidRasterizerSate; // �ָ���� �������� ����
	ComPtr<ID3D11RasterizerState> m_pD3dWireRasterizerSate; // �̰��� ���̾� ������Ʈ�� ���� ������ �ָ��� 1���� �־ ���α׷��� ���ư���.
	ComPtr<ID3D11DepthStencilState> m_pD3dDepthStencilState; // ���߿� ����Ϸ��� ��������, �Ŀ� ��� ������ �߰� �ּ� ����
	ComPtr<IDXGISwapChain> m_pD3dSwapChain; // ���� ü��, GPU <-> CPU 
	D3D11_VIEWPORT m_pD3dScreenViewport;  // �� �״�� �� ��Ʈ

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
