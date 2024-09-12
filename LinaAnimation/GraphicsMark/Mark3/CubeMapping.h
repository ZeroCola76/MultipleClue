#pragma once
#include<iostream>
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
class D3dUtilities;

/// <summary>
/// 큐브 매핑을 사용 및 업데이트 하는 클래스
/// </summary>
class CubeMapping
{
public:
	// 큐브매핑의 상수 버퍼 구조체
	struct CubeVertexConstantData {Matrix viewProj;}; // 미리 곱해서 사용

public:
	CubeMapping();
	~CubeMapping();

public:
	// Pre-filter가 되지 않은 원본 텍스춰
	ComPtr<ID3D11ShaderResourceView> m_pOriginalResView;

	// IBL을 위해 다른 물체들 그릴때 사용    
	ComPtr<ID3D11ShaderResourceView> m_pDiffuseResView;
	ComPtr<ID3D11ShaderResourceView> m_pSpecularResView;

private:
	// 오브젝트 선언
	std::shared_ptr<NestGraphics::Mesh> m_pCubeMesh;
	// 큐브매핑 버텍스 상수
	CubeMapping::CubeVertexConstantData m_CubeVertexConstantData;

	// 큐브 매핑의 정보를 저장할 공간 및 샘플러
	ComPtr<ID3D11SamplerState> m_pCubeSamplerState; // 큐브 매핑 샘플러
	ComPtr<ID3D11VertexShader> m_pCubeVertexShader;	// 큐브 버텍스 쉐이더
	ComPtr<ID3D11PixelShader> m_pCubePixelShader;	// 큐브 픽셀 쉐이더
	ComPtr<ID3D11InputLayout> m_pCubeInputLayout;	// 규브 인풋 레이아웃

	// 자주 사용하는 함수들의 묶음을 사용
	D3dUtilities* m_pD3dUtilities;

public:
	// 생성, 업뎃, 렌더 함수 : 렌더러에 사용함
	void Initialize(ComPtr<ID3D11Device>& device, const wchar_t* originalFilename, const wchar_t* diffuseFilename, const wchar_t* specularFilename, NestGraphics::ModelData* modelMeshDataes);
	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const Matrix& viewCol, const Matrix& projCol);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	//버퍼 사이즈 오류를 알려주는 함수
	static_assert((sizeof(CubeVertexConstantData) % 16) == 0, "Constant Buffer size must be 16-byte aligned");
};
