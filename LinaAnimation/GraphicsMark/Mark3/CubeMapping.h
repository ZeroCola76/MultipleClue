#pragma once
#include<iostream>
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;
class D3dUtilities;

/// <summary>
/// ť�� ������ ��� �� ������Ʈ �ϴ� Ŭ����
/// </summary>
class CubeMapping
{
public:
	// ť������� ��� ���� ����ü
	struct CubeVertexConstantData {Matrix viewProj;}; // �̸� ���ؼ� ���

public:
	CubeMapping();
	~CubeMapping();

public:
	// Pre-filter�� ���� ���� ���� �ؽ���
	ComPtr<ID3D11ShaderResourceView> m_pOriginalResView;

	// IBL�� ���� �ٸ� ��ü�� �׸��� ���    
	ComPtr<ID3D11ShaderResourceView> m_pDiffuseResView;
	ComPtr<ID3D11ShaderResourceView> m_pSpecularResView;

private:
	// ������Ʈ ����
	std::shared_ptr<NestGraphics::Mesh> m_pCubeMesh;
	// ť����� ���ؽ� ���
	CubeMapping::CubeVertexConstantData m_CubeVertexConstantData;

	// ť�� ������ ������ ������ ���� �� ���÷�
	ComPtr<ID3D11SamplerState> m_pCubeSamplerState; // ť�� ���� ���÷�
	ComPtr<ID3D11VertexShader> m_pCubeVertexShader;	// ť�� ���ؽ� ���̴�
	ComPtr<ID3D11PixelShader> m_pCubePixelShader;	// ť�� �ȼ� ���̴�
	ComPtr<ID3D11InputLayout> m_pCubeInputLayout;	// �Ժ� ��ǲ ���̾ƿ�

	// ���� ����ϴ� �Լ����� ������ ���
	D3dUtilities* m_pD3dUtilities;

public:
	// ����, ����, ���� �Լ� : �������� �����
	void Initialize(ComPtr<ID3D11Device>& device, const wchar_t* originalFilename, const wchar_t* diffuseFilename, const wchar_t* specularFilename, NestGraphics::ModelData* modelMeshDataes);
	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const Matrix& viewCol, const Matrix& projCol);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	//���� ������ ������ �˷��ִ� �Լ�
	static_assert((sizeof(CubeVertexConstantData) % 16) == 0, "Constant Buffer size must be 16-byte aligned");
};
