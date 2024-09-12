#include "DxRenderState.h"
#include "PvDefine.h"

DxRenderState::DxRenderState()
{

}

DxRenderState::~DxRenderState()
{

}

void DxRenderState::CreateRenderStates(ID3D11Device* pDevice)
{
	// Render State Сп Rasterizer State
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	DYTRACE(pDevice->CreateRasterizerState(&solidDesc, m_pSolidRasterizerState.GetAddressOf())
		, "CreateRaseterizerState Failed.");

	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	DYTRACE(pDevice->CreateRasterizerState(&wireframeDesc, m_pWireframeRasterizerState.GetAddressOf())
		, "CreateRaseterizerState Failed.");
}
