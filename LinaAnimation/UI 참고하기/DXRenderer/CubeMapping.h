#pragma once
#include <memory>
#include "Geometry.h" // cpp�� �ű��� ����
#include "directxtk/SimpleMath.h"
#include "D3dUtilities.h"

class D3dUtilities;
class GameObject;

using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

/// <summary>
/// ť�� ������ ��� �� �����ϴ� Ŭ����
/// 2024.1.24. ���� ���� ��
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
	ComPtr<ID3D11SamplerState> m_pCubeSamplerState;
	ComPtr<ID3D11VertexShader> m_pCubeVertexShader;
	ComPtr<ID3D11PixelShader> m_pCubePixelShader;
	ComPtr<ID3D11InputLayout> m_pCubeInputLayout;

	// ���� ����ϴ� �Լ����� ������ ���
	D3dUtilities* pd3dUtilities;

public:
	// ����, ����, ���� �Լ� : �������� �����
	void Initialize(ComPtr<ID3D11Device>& device, const wchar_t* originalFilename, const wchar_t* diffuseFilename, const wchar_t* specularFilename);
	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const Matrix& viewCol, const Matrix& projCol);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	//���� ������ ������ �˷��ִ� �Լ�
	static_assert((sizeof(CubeVertexConstantData) % 16) == 0, "Constant Buffer size must be 16-byte aligned");
};
