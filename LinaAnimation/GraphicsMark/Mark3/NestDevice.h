#pragma once
#include "D3dUtilities.h" // �̰͵� �ǵ��� ������.
// ���⿡ �����ϴ� �͵��� ���� �⺻���� ��ġ�μ�, �׷��Ƚ��� ���ư��� ���� �ʼ������� �ʿ��� ��ġ���̶�� ���� �ȴ�.
class D3dUtilities;

class NestDevice
{
private:
	UINT m_numQualityLevels = 1; //���� ���� ������ �ʿ��� ����

	ComPtr<ID3D11Device> m_pD3dDevice;//�׷��� ���ҽ��� ���� �� �����ϱ� ���� �߿��� �������̽�
	ComPtr<ID3D11DeviceContext> m_pD3dContext; //dxcontext
	ComPtr<ID3D11RenderTargetView> m_pD3dRenderTargetView; // ���� �����ϴ� Ÿ�� ���� ������ �����Ѵ�.
	ComPtr<ID3D11ShaderResourceView> m_pD3dShaderResourceView; // �� ���� ȿ���� �߰��ϱ� ���� �����.
	ComPtr<IDXGISwapChain> m_pD3dSwapChain; // ���� ü��, GPU <-> CPU

	ComPtr<ID3D11RasterizerState> m_pD3dSolidRasterizerSate; // �ָ���� �������� ����
	ComPtr<ID3D11RasterizerState> m_pD3dWireRasterizerSate; // �̰��� ���̾� ������Ʈ�� ���� ������ �ָ��� 1���� �־ ���α׷��� ���ư���.

	// Depth buffer ����
	ComPtr<ID3D11Texture2D> m_pD3dDepthStencilBuffer; //���״�� ���� ���ٽ� ����
	ComPtr<ID3D11DepthStencilView> m_pD3dDepthStencilView; // ���߿� ����Ϸ��� ��������, �Ŀ� ��� ������ �߰� �ּ� ����
	ComPtr<ID3D11DepthStencilState> m_pD3dDepthStencilState; // ���߿� ����Ϸ��� ��������, �Ŀ� ��� ������ �߰� �ּ� ����

	D3D11_VIEWPORT m_pD3dScreenViewport;  // �� ��Ʈ

	D3dUtilities* m_pD3dUtilities;

	bool CreateRenderTargetView(); //����Ÿ���� ����������� üũ
	void SetViewport(int m_screenWidth, int m_screenHeight); //����Ʈ ����

public:
	NestDevice(); //����̽��� ���ٽ�Ʈ ������ ������
	~NestDevice(); //�Ҹ���

	HRESULT InitializeDevice(int m_hWnd, int m_screenWidth, int m_screenHeight); //ȭ�� �ʱ�ȭ
	HRESULT Present(); //EndRender���� ���ش�.

	UINT getNumQualityLevels() const { return m_numQualityLevels; } //���� ���ۿ� ���õ� get

	ComPtr<ID3D11Device> GetD3dDevice() const { return m_pD3dDevice; } //����̽� get
	ComPtr<ID3D11DeviceContext> GetD3dContext() const { return m_pD3dContext; } //context get
	ComPtr<ID3D11RenderTargetView> GetD3dRenderTargetView() const { return m_pD3dRenderTargetView; } // ����Ÿ�� get
	ComPtr<ID3D11ShaderResourceView> GetD3dShaderResourceView() const { return m_pD3dShaderResourceView; } // ����Ÿ�� get
	ComPtr<IDXGISwapChain> GetD3dSwapChain() const { return m_pD3dSwapChain; } //����ü�� get
	
	ComPtr<ID3D11RasterizerState> GetD3dSolidRasterizerSate() const { return m_pD3dSolidRasterizerSate; } //�ָ���� �������� get
	ComPtr<ID3D11RasterizerState> GetD3dWireRasterizerSate() const { return m_pD3dWireRasterizerSate; } //���̾� ������Ʈ get
	
	ComPtr<ID3D11Texture2D> getD3dDepthStencilBuffer()const { return m_pD3dDepthStencilBuffer; }; //���� ���ٽ� get
	ComPtr<ID3D11DepthStencilView> GetD3dDepthStencilView() const { return m_pD3dDepthStencilView; } //���� ���x 
	ComPtr<ID3D11DepthStencilState> GetD3dDepthStencilState() const { return m_pD3dDepthStencilState; } //���� ��� x
	D3D11_VIEWPORT GetD3dScreenViewport() const { return m_pD3dScreenViewport; } // ����Ʈ get
};
