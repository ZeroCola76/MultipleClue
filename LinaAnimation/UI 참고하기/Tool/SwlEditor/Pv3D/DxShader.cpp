#include "DxShader.h"

DxShader::DxShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;

	CreateVertexShader();
	CreatePixelShader();
	CreateComstantBuffer();
}

DxShader::~DxShader()
{

}

void DxShader::Update()
{

}

void DxShader::SetShader()
{

}

void DxShader::CreateVertexShader()
{
}

void DxShader::CreatePixelShader()
{

}

void DxShader::CreateComstantBuffer()
{

}
