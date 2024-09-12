#include "DxViewport.h"

DxViewport::DxViewport(ID3D11DeviceContext* pDeviceContext, Vector4 leftTopWidthHeight)
{
	m_pDeviceContext = pDeviceContext;

	m_viewport.TopLeftX = 0;
	m_viewport.TopLeftY = 0;
	m_viewport.Width = leftTopWidthHeight.z;
	m_viewport.Height = leftTopWidthHeight.w;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;

}

DxViewport::~DxViewport()
{

}

void DxViewport::SetViewport()
{
	m_pDeviceContext->RSSetViewports(1, &m_viewport);
}
