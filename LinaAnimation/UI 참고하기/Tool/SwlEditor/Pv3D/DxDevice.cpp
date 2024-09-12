#include "DxDevice.h"

DxDevice::DxDevice()
{

}

DxDevice::~DxDevice()
{

}

bool DxDevice::CreateDevice(int hWnd, int screenWidth, int screenHeight)
{
	UINT createDeviceFlags = 0;
#if defined(DEBUG)||defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, 0, 0, D3D11_SDK_VERSION,
		&m_pDevice, &m_featureLevel, &m_pDeviceContext);

	bool enable4xMsaa = false;
	UINT msaaQuality4x = 0;

	DYTRACE(SUCCEEDED(hr), "D3D11CreateDevice Failed.\n");

	DYTRACE(m_featureLevel == D3D_FEATURE_LEVEL_11_0, "D3D_FEATURE_LEVEL_11_0 unsupported.");

	DYTRACE(SUCCEEDED(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM
		, 4, &msaaQuality4x)), "CheckMultisampleQualityLevels Error.");

	DYTRACE(msaaQuality4x > 0, "4xMsaa Error.");

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = msaaQuality4x - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = reinterpret_cast<HWND>(hWnd);
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// Create SwapChain
	ComPtr<IDXGIDevice> dxgiDevice;

	DYTRACE(SUCCEEDED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice)
		, (void**)dxgiDevice.GetAddressOf())), "DxgiDevice Error.");

	ComPtr<IDXGIAdapter> dxgiAdapter;

	DYTRACE(SUCCEEDED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter)
		, (void**)dxgiAdapter.GetAddressOf())), "DxgiDevice Error.");

	ComPtr<IDXGIFactory> dxgiFactory;

	DYTRACE(SUCCEEDED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory)
		, (void**)dxgiFactory.GetAddressOf())), "DxgiAdapter Error.");

	DYTRACE(SUCCEEDED(dxgiFactory->CreateSwapChain(m_pDevice.Get()
		, &sd, m_pSwapChain.GetAddressOf())), "DxgiFactory Error.");

	return true;
}

void DxDevice::Present()
{
	DYTRACE(SUCCEEDED(m_pSwapChain->Present(0, 0)), "SwapChain Present Error.");
}
