#include "NestDevice.h"

NestDevice::NestDevice() : m_pD3dScreenViewport(D3D11_VIEWPORT()) {}
NestDevice::~NestDevice() {}

HRESULT NestDevice::InitializeDevice(int m_hWnd, int m_screenWidth, int m_screenHeight) 
{
    UINT createDeviceFlags = 0;
    m_DriverType = D3D_DRIVER_TYPE_HARDWARE;
#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    const D3D_FEATURE_LEVEL featureLevels[2] = 
    {
        D3D_FEATURE_LEVEL_11_0, // 더 높은 버전이 먼저 오도록 설정
        D3D_FEATURE_LEVEL_9_3
    };
    D3D_FEATURE_LEVEL featureLevel;

    if (FAILED(D3D11CreateDevice(nullptr, m_DriverType, 0, createDeviceFlags, featureLevels,
                                 ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, device.GetAddressOf(),
                                 &featureLevel, context.GetAddressOf()
                                 /// TODO[R&K] : Here, now we're using Immediate Context,
                                 /// After that, we'll optimize to change to Deffered Context.
                                 ))) 
    {
        std::cout << "D3D11CreateDevice() failed." << std::endl;
        return false;
    }

    if (featureLevel != D3D_FEATURE_LEVEL_11_0) 
    {
        std::cout << "D3D Feature Level 11 unsupported." << std::endl;
        return false;
    }

    // 4X MSAA 지원하는지 확인
    UINT numQualityLevels;
    device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &numQualityLevels);
    if (numQualityLevels <= 0) 
    {
        std::cout << "MSAA not supported." << std::endl;
    }

    // numQualityLevels = 0; // MSAA 강제로 끄기 -> 디퍼드 렌더링을 간접 체험할 때 사용해보자.

    if (FAILED(device.As(&m_pD3dDevice))) 
    {
        std::cout << "device.AS() failed." << std::endl;
        return false;
    }

    if (FAILED(context.As(&m_pD3dContext))) 
    {
        std::cout << "context.As() failed." << std::endl;
        return false;
    }

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferDesc.Width = m_screenWidth;
    sd.BufferDesc.Height = m_screenHeight;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferCount = 2;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)m_hWnd;
    sd.Windowed = TRUE;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    if (numQualityLevels > 0) 
    {
        sd.SampleDesc.Count = 4; // how many multisamples
        sd.SampleDesc.Quality = numQualityLevels - 1;
    }
    else 
    {
        sd.SampleDesc.Count = 1; // how many multisamples
        sd.SampleDesc.Quality = 0;
    }

    // IDXGIFactory를 이용한 CreateSwapChain()
    ComPtr<IDXGIDevice3> dxgiDevice;
    m_pD3dDevice.As(&dxgiDevice);

    ComPtr<IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(&dxgiAdapter);

    ComPtr<IDXGIFactory> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));

    ComPtr<IDXGISwapChain> swapChain;
    dxgiFactory->CreateSwapChain(m_pD3dDevice.Get(), &sd, &swapChain);

    swapChain.As(&m_pD3dSwapChain);

    // CreateRenderTargetView
    CreateRenderTargetView();

    SetViewport(m_screenWidth,m_screenHeight);

    // Create a rasterizer state
    D3D11_RASTERIZER_DESC rastDesc;
    ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC)); // Need this
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    // rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    rastDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rastDesc.FrontCounterClockwise = false;
    rastDesc.DepthClipEnable = true;

    m_pD3dDevice->CreateRasterizerState(&rastDesc, m_pD3dSolidRasterizerSate.GetAddressOf());
    rastDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME; // 와이어 모드
    m_pD3dDevice->CreateRasterizerState(&rastDesc, m_pD3dWireRasterizerSate.GetAddressOf());



    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    depthStencilDesc.DepthEnable = true; // false
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
    if (FAILED(m_pD3dDevice->CreateDepthStencilState(&depthStencilDesc, m_pD3dDepthStencilState.GetAddressOf()))) 
    {
        std::cout << "CreateDepthStencilState() failed." << std::endl;
    }
}

HRESULT NestDevice::Present() { return E_NOTIMPL; }

bool NestDevice::CreateRenderTargetView() 
{
    ComPtr<ID3D11Texture2D> pBackBuffer;
    m_pD3dSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));
    if (pBackBuffer) 
    {
        m_pD3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, m_pD3dRenderTargetView.GetAddressOf());
    }
    else 
    {
        std::cout << "CreateRenderTargetView() failed." << std::endl;
        return false;
    }
    return true;
}

void NestDevice::SetViewport(int m_screenWidth, int m_screenHeight) 
{
    ZeroMemory(&m_pD3dScreenViewport, sizeof(D3D11_VIEWPORT));
    m_pD3dScreenViewport.TopLeftX = 0;
    m_pD3dScreenViewport.TopLeftY = 0;
    m_pD3dScreenViewport.Width = float(m_screenWidth);
    m_pD3dScreenViewport.Height = float(m_screenHeight);
    // m_screenViewport.Width = static_cast<float>(m_screenHeight);
    m_pD3dScreenViewport.MinDepth = 0.0f;
    m_pD3dScreenViewport.MaxDepth = 1.0f; // Note: important for depth buffering

    m_pD3dContext->RSSetViewports(1, &m_pD3dScreenViewport);
}


