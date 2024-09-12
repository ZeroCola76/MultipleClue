#include "../Main_DemoGrapics/GameObject.h"
#include "ObjectProcessor.h"

ObjectProcessor::ObjectProcessor()
{
	d3dUtilities = new D3dUtilities();
}

ObjectProcessor::~ObjectProcessor()
{
	delete d3dUtilities;
}

void ObjectProcessor::Initialize(ComPtr<ID3D11Device>& device, const std::wstring& filename)
{
	auto meshes = GameObject::ReadFromFile(filename);
	Initialize(device, meshes);
}

void ObjectProcessor::Initialize(ComPtr<ID3D11Device>& device, const std::vector<Nest::MeshData>& meshes)
{
	// Sampler 만들기
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf());

	// 버퍼의 현재 뷰를 설정
	objectVertexConstantData.world = Matrix();
	objectVertexConstantData.view = Matrix();
	objectVertexConstantData.projection = Matrix();

	d3dUtilities->CreateConstantBuffer(device, objectVertexConstantData, m_pVertexConstantBuffer);
	d3dUtilities->CreateConstantBuffer(device, objectPixelConstantData, m_pPixelConstantBuffer);

	for (const auto& meshData : meshes)
	{
		auto newMesh = std::make_shared<Nest::Mesh>();
		d3dUtilities->CreateVertexBuffer(device, meshData.vertices, newMesh->m_vertexBuffer);
		newMesh->m_indexCount = UINT(meshData.indices.size());
		d3dUtilities->CreateIndexBuffer(device, meshData.indices, newMesh->m_indexBuffer);

		if (!meshData.textureFilename.empty())
		{
			std::cout << meshData.textureFilename << std::endl;
			d3dUtilities->CreateTexture(device, meshData.textureFilename, newMesh->texture, newMesh->textureResourceView);
		}

		newMesh->m_vertexConstantBuffer = m_pVertexConstantBuffer;
		newMesh->m_pixelConstantBuffer = m_pPixelConstantBuffer;

		this->m_pMeshes.push_back(newMesh);
	}

	// 입력되는 데이터값이 어떤 형태인지 저장
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// 버텍스의 hlsl로부터 정보를 가져와서 입력 데이터에 넣는다. // 자세한 설명은 관련 함수 템플릿에 적어둠
	d3dUtilities->CreateVertexShaderAndInputLayout(device, L"../NestResources/hlsl/VertexShader.hlsl", inputElements,	m_pVertexShader, m_pInputLayout);
	// CreateVertexShaderAndInputLayout()와 구조 동일
	d3dUtilities->CreatePixelShader(device, L"../NestResources/hlsl/PixelShader.hlsl", m_pPixelShader);

	/// 위에서 생성한 메쉬와 버텍스를 공유하여, 반드시 메쉬 생성 후 노멀 벡터를 그린다.
	
	m_pNormalLines = std::make_shared<Nest::Mesh>();

	std::vector<Nest::Vertex> normalVertices;
	std::vector<uint16_t> normalIndices;

	size_t offset = 0;
	for (const auto& meshData : meshes) {
		for (size_t i = 0; i < meshData.vertices.size(); i++) {

			auto v = meshData.vertices[i];

			v.texcoord.x = 0.0f; // 시작점 표시
			normalVertices.push_back(v);

			v.texcoord.x = 1.0f; // 끝점 표시
			normalVertices.push_back(v);

			normalIndices.push_back(uint32_t(2 * (i + offset)));
			normalIndices.push_back(uint32_t(2 * (i + offset) + 1));
		}
		offset += meshData.vertices.size();
	}

	d3dUtilities->CreateVertexBuffer(device, normalVertices, m_pNormalLines->m_vertexBuffer);
	m_pNormalLines->m_indexCount = UINT(normalIndices.size());
	d3dUtilities->CreateIndexBuffer(device, normalIndices, m_pNormalLines->m_indexBuffer);
	d3dUtilities->CreateVertexShaderAndInputLayout(device, L"../NestResources/hlsl/NormalVertexShader.hlsl",
		inputElements, m_pNormalVertexShader, m_pInputLayout);
	d3dUtilities->CreateConstantBuffer(device, objectNormalVertexConstantData, m_pObjectNormalVertexConstantBuffer);
	d3dUtilities->CreatePixelShader(device, L"../NestResources/hlsl/NormalPixelShader.hlsl", m_pNormalPixelShader);
}

void ObjectProcessor::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	d3dUtilities->UpdateBuffer(device, context, objectVertexConstantData, m_pVertexConstantBuffer);
	if (m_isDrawNormals && m_isChangingNormalFlag)
	{
		///[노멀] 버텍스 값을 업데이트 한다.
		d3dUtilities->UpdateBuffer(device, context, objectNormalVertexConstantData, m_pObjectNormalVertexConstantBuffer);
		m_isChangingNormalFlag = false;
	}
	d3dUtilities->UpdateBuffer(device, context, objectPixelConstantData, m_pPixelConstantBuffer);
}

void ObjectProcessor::Render(ComPtr<ID3D11DeviceContext>& context)
{
	/// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(Nest::Vertex);
	UINT offset = 0;

	/// 버텍스 쉐이더를 설정한다.(수정 위치)
	context->VSSetShader(m_pVertexShader.Get(), 0, 0);
	context->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	context->PSSetShader(m_pPixelShader.Get(), 0, 0);

	// 버텍스/인덱스 버퍼 설정
	for (const auto& meshes : m_pMeshes)
	{
		context->VSSetConstantBuffers(0, 1, meshes->m_vertexConstantBuffer.GetAddressOf());
		// 물체 렌더링할 때 큐브맵도 같이 사용
		ID3D11ShaderResourceView* resViews[3] = { meshes->textureResourceView.Get(),
			m_pObjectDiffuseResView.Get(),	m_pObjectSpecularResView.Get() };
		context->PSSetShaderResources(0, 3, resViews);
		context->PSSetConstantBuffers(0, 1, meshes->m_pixelConstantBuffer.GetAddressOf());
		context->IASetInputLayout(m_pInputLayout.Get());
		context->IASetVertexBuffers(0, 1, meshes->m_vertexBuffer.GetAddressOf(), &stride, &offset);
		context->IASetIndexBuffer(meshes->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexed(meshes->m_indexCount, 0, 0);
	}

	if (m_isDrawNormals)
	{
		context->IASetVertexBuffers(0, 1, m_pNormalLines->m_vertexBuffer.GetAddressOf(), &stride, &offset); /// slot을 1부터 시작하면 안 된다. vertex 정보 받아와야 함. 공유한다.
		context->IASetIndexBuffer(m_pNormalLines->m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		context->VSSetShader(m_pNormalVertexShader.Get(), 0, 0);
		ID3D11Buffer* pptr[2] = { m_pMeshes[0]->m_vertexConstantBuffer.Get(), m_pNormalLines->m_vertexConstantBuffer.Get() };
		context->VSSetConstantBuffers(1, 1, m_pNormalLines->m_vertexConstantBuffer.GetAddressOf());
		context->PSSetShader(m_pNormalPixelShader.Get(), 0, 0);
		context->DrawIndexed(m_pNormalLines->m_indexCount, 0, 0);
	}
}
