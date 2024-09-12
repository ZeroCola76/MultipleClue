#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"

class ObjectProcessor
{
public:
	ObjectProcessor();
	~ObjectProcessor();

public:
	void Initialize(ComPtr<ID3D11Device>& device, const std::wstring& filename);
	void Initialize(ComPtr<ID3D11Device>& device, const std::vector<Nest::MeshData>& meshes);
	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	// ExampleApp::Update()에서 접근
	Nest::VertexConstantBuffer objectVertexConstantData;
	Nest::PixelShaderConstantBuffer objectPixelConstantData;

	// ExampleApp:Initialize()에서 접근
	ComPtr<ID3D11ShaderResourceView> m_pObjectDiffuseResView;
	ComPtr<ID3D11ShaderResourceView> m_pObjectSpecularResView;

	// GUI에서 업데이트 할 때 사용
	Nest::NormalShaderConstantBuffer objectNormalVertexConstantData;
	bool m_isChangingNormalFlag = true;
	bool m_isDrawNormals = false;

private:
	// 하나의 3D 모델이 내부적으로는 여러개의 메쉬로 구성
	std::vector<std::shared_ptr<Nest::Mesh>> m_pMeshes; // 오브젝트 메쉬의 구조체를 가리키는 포인터

	// 오브젝트 쉐이더를 저장하는 인터페이스
	ComPtr<ID3D11VertexShader> m_pVertexShader; // 정점
	ComPtr<ID3D11PixelShader> m_pPixelShader;   // 픽셸
	ComPtr<ID3D11InputLayout> m_pInputLayout; // 입력 레이아웃, 정점 셰이더에 입력되는 데이터의 형식을 정의

	ComPtr<ID3D11SamplerState> m_pSamplerState;

	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;

	// 메쉬의 노멀 벡터 그리기
	ComPtr<ID3D11VertexShader> m_pNormalVertexShader; // 노멀 정점
	ComPtr<ID3D11PixelShader> m_pNormalPixelShader;   // 노멀 픽셸
	std::shared_ptr<Nest::Mesh> m_pNormalLines; // 노멀 메쉬의 구조체를 가리키는 포인터

	ComPtr<ID3D11Buffer> m_pObjectNormalVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pObjectNormalPixelConstantBuffer;

private:
	D3dUtilities* d3dUtilities;

};