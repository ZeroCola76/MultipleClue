#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include "../Main_DemoGrapics/GameObject.h"

/// <summary>
/// 이걸 부모클래스로 바꾸겠다.(김예리나)
/// 내장 메쉬와 외장 메쉬를 구분해서 그리는 클래스
/// 2024.1.24. 순서 끝.
/// </summary>
class MeshGroup
{
public:
	MeshGroup();
	virtual ~MeshGroup();

protected:
	// 하나의 3D 모델이 내부적으로는 여러개의 메쉬로 구성
	std::vector<std::shared_ptr<Nest::Mesh>> m_pMeshes; // 오브젝트 메쉬의 구조체를 가리키는 포인터
	// 오브젝트 쉐이더를 저장하는 인터페이스
	ComPtr<ID3D11VertexShader> m_pVertexShader; // 정점
	ComPtr<ID3D11PixelShader> m_pPixelShader;   // 픽셸
	ComPtr<ID3D11InputLayout> m_pInputLayout; // 입력 레이아웃, 정점 셰이더에 입력되는 데이터의 형식을 정의
	// 오브젝트의 샘플러
	ComPtr<ID3D11SamplerState> m_pSamplerState;
	// 오브젝트의 상수 버퍼
	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;
	// 메쉬의 노멀 벡터 그리기
	ComPtr<ID3D11VertexShader> m_pNormalVertexShader; // 노멀 정점
	ComPtr<ID3D11PixelShader> m_pNormalPixelShader;   // 노멀 픽셸
	std::shared_ptr<Nest::Mesh> m_pNormalLines; // 노멀 메쉬의 구조체를 가리키는 포인터
	ComPtr<ID3D11Buffer> m_pObjectNormalVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pObjectNormalPixelConstantBuffer;

	// 자주 사용하는 함수들의 묶음을 사용
	D3dUtilities* d3dUtilities;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, const std::wstring& filename);
	virtual void Initialize(ComPtr<ID3D11Device>& device);
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	virtual void Render(ComPtr<ID3D11DeviceContext>& context);
	virtual void Update(/*float _dt*/Vector3 vector, Matrix view, Matrix proj);

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

	// 물체를 가져오기 위한 타입변수
	virtual std::vector<Nest::MeshData> GetMeshType();
};