#include "../Main_DemoGrapics/GameObject.h"
#include "CubeMapping.h"

CubeMapping::CubeMapping()
{
	pd3dUtilities = new D3dUtilities();
}

CubeMapping::~CubeMapping()
{
	delete pd3dUtilities;
}

void CubeMapping::Initialize(ComPtr<ID3D11Device>& device, const wchar_t* originalFilename, const wchar_t* diffuseFilename, const wchar_t* specularFilename)
{
	pd3dUtilities->CreateCubemapTexture(device, originalFilename, m_pOriginalResView);
	pd3dUtilities->CreateCubemapTexture(device, diffuseFilename, m_pDiffuseResView);
	pd3dUtilities->CreateCubemapTexture(device, specularFilename, m_pSpecularResView);

	m_pCubeMesh = std::make_shared<NestGraphics::Mesh>();

	pd3dUtilities->CreateConstantBuffer(device, m_CubeVertexConstantData, m_pCubeMesh->m_vertexConstantBuffer);
	NestGraphics::MeshData cubeMeshData = GameObject::MakeBox(20.0f);
	//MeshData cubeMeshData = GeometryGenerator::MakeSphere(10.0f, 10, 10);
	std::reverse(cubeMeshData.indices.begin(), cubeMeshData.indices.end()); // 뒤집어서 그려준다.(안쪽면이 보이게)

	pd3dUtilities->CreateVertexBuffer(device, cubeMeshData.vertices, m_pCubeMesh->m_vertexBuffer);
	m_pCubeMesh->m_indexCount = UINT(cubeMeshData.indices.size());
	pd3dUtilities->CreateIndexBuffer(device, cubeMeshData.indices, m_pCubeMesh->m_indexBuffer);

	//다른 쉐이더와 동일한 InputLayout, 실제로는 "POSITION"만 사용
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	pd3dUtilities->CreateVertexShaderAndInputLayout(device, L"../NestResources/hlsl/CubeMappingVertexShader.hlsl", inputElements,
		m_pCubeVertexShader, m_pCubeInputLayout);
	// CreateVertexShaderAndInputLayout()와 구조 동일
	pd3dUtilities->CreatePixelShader(device, L"../NestResources/hlsl/CubeMappingPixelShader.hlsl", m_pCubePixelShader);

	// Texture sampler 만들기
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the Sample State
	device->CreateSamplerState(&sampDesc, m_pCubeSamplerState.GetAddressOf());
}

void CubeMapping::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const Matrix& viewCol, const Matrix& projCol)
{
	m_CubeVertexConstantData.viewProj = projCol * viewCol;

	pd3dUtilities->UpdateBuffer(device, context, m_CubeVertexConstantData, m_pCubeMesh->m_vertexConstantBuffer);
}

void CubeMapping::Render(ComPtr<ID3D11DeviceContext>& context)
{
	/// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(NestGraphics::Vertex);
	UINT offset = 0;

	context->IASetInputLayout(m_pCubeInputLayout.Get());
	context->IASetVertexBuffers(0, 1, m_pCubeMesh->m_vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_pCubeMesh->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0); // 굳이 R32로 바꾸지 않겠지만, 필요시 변경할 것
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->VSSetShader(m_pCubeVertexShader.Get(), 0, 0);
	context->VSSetConstantBuffers(0, 1, m_pCubeMesh->m_vertexConstantBuffer.GetAddressOf());
	ID3D11ShaderResourceView* views[1] = { m_pOriginalResView.Get() };
	context->PSSetShaderResources(0, 1, views);
	context->PSSetShader(m_pCubePixelShader.Get(), 0, 0);
	context->PSSetSamplers(0, 1, m_pCubeSamplerState.GetAddressOf());
	context->DrawIndexed(m_pCubeMesh->m_indexCount, 0, 0);
}
