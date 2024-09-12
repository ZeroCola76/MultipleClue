#include "DxRenderTarget.h"
#include "PvDefine.h"

DxRenderTarget::DxRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext
	, IDXGISwapChain* pSwapChain, int width, int height)
	:m_width(width), m_height(height)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
	m_pSwapChain = pSwapChain;

	CreateTexture();
	CreateRenderTargetView();
	CreateShaderResourceView();
	CreateDepthStencilView();
}

DxRenderTarget::~DxRenderTarget()
{

}

void DxRenderTarget::CreateTexture()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	m_pDevice->CreateTexture2D(&textureDesc, nullptr, m_pTextureBuffer.GetAddressOf());
}

void DxRenderTarget::CreateRenderTargetView()
{
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDesc.Texture2D.MipSlice = 0;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	ComPtr<ID3D11Texture2D> backBuffer;
	DYTRACE(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()))," ");
	DYTRACE(m_pDevice->CreateRenderTargetView(backBuffer.Get() /*m_pTextureBuffer.Get()*/, 0/*&renderTargetViewDesc*/
		, m_pRenderTargetView.GetAddressOf()), "CreateRenderTarget Error.");
}

void DxRenderTarget::CreateShaderResourceView()
{
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

	m_pDevice->CreateShaderResourceView(m_pTextureBuffer.Get(), &shaderResourceViewDesc
		, m_pShaderResourceView.GetAddressOf());
}

void DxRenderTarget::CreateDepthStencilView()
{
	// Create the depth stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
	ZeroMemory(&depthStencilTextureDesc, sizeof(depthStencilTextureDesc));
	depthStencilTextureDesc.Width = m_width;
	depthStencilTextureDesc.Height = m_height;
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.ArraySize = 1;
	depthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthStencilTextureDesc.SampleDesc.Count = 4;
	depthStencilTextureDesc.SampleDesc.Quality = -1;

	depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilTextureDesc.CPUAccessFlags = 0;
	depthStencilTextureDesc.MiscFlags = 0;

	DYTRACE(m_pDevice->CreateTexture2D(&depthStencilTextureDesc, 0, m_pDepthStencilBuffer.GetAddressOf())
		, "DepthStencilBuffer Error.");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Texture2D.MipSlice = 0;
	depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	DYTRACE(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), 0/*&depthStencilDesc*/
		, m_pDepthStencilView.GetAddressOf()), "DepthStencilBuffer Error.");
}

void DxRenderTarget::OnBeginRender(const float* color)
{
	// Clear RenderTargetView
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get()
		, color/*reinterpret_cast<const float*>(&Colors::Crimson)*/);

	// Clear DepthStencilView
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get()
		, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

}

void DxRenderTarget::SetRenderTarget()
{
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void DxRenderTarget::SetDepthStencilView()
{
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 0);
}

void DxRenderTarget::ClearRenderTargetView(const float* color)
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
}

void DxRenderTarget::ClearDepthStencilView()
{

	// DS버퍼 초기화
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get()
		, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}
