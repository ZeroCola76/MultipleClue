#include "GraphicsResourceManager.h"

GraphicsResourceManager::GraphicsResourceManager(ComPtr<ID3D11Device> pD3dDevice, ComPtr<ID3D11DeviceContext> pD3dContext)
	: m_pD3dDevice(pD3dDevice), m_pD3dContext(pD3dContext)
{
}

GraphicsResourceManager::~GraphicsResourceManager()
{
}
