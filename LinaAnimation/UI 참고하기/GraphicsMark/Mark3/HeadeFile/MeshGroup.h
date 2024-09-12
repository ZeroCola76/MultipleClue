#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include "../Main_DemoGrapics/GameObject.h"

/// <summary>
/// �̰� �θ�Ŭ������ �ٲٰڴ�.(�迹����)
/// ���� �޽��� ���� �޽��� �����ؼ� �׸��� Ŭ����
/// 2024.1.24. ���� ��.
/// </summary>
class MeshGroup
{
public:
	MeshGroup();
	virtual ~MeshGroup();

protected:
	// �ϳ��� 3D ���� ���������δ� �������� �޽��� ����
	std::vector<std::shared_ptr<Nest::Mesh>> m_pMeshes; // ������Ʈ �޽��� ����ü�� ����Ű�� ������
	// ������Ʈ ���̴��� �����ϴ� �������̽�
	ComPtr<ID3D11VertexShader> m_pVertexShader; // ����
	ComPtr<ID3D11PixelShader> m_pPixelShader;   // �ȼ�
	ComPtr<ID3D11InputLayout> m_pInputLayout; // �Է� ���̾ƿ�, ���� ���̴��� �ԷµǴ� �������� ������ ����
	// ������Ʈ�� ���÷�
	ComPtr<ID3D11SamplerState> m_pSamplerState;
	// ������Ʈ�� ��� ����
	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;
	// �޽��� ��� ���� �׸���
	ComPtr<ID3D11VertexShader> m_pNormalVertexShader; // ��� ����
	ComPtr<ID3D11PixelShader> m_pNormalPixelShader;   // ��� �ȼ�
	std::shared_ptr<Nest::Mesh> m_pNormalLines; // ��� �޽��� ����ü�� ����Ű�� ������
	ComPtr<ID3D11Buffer> m_pObjectNormalVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pObjectNormalPixelConstantBuffer;

	// ���� ����ϴ� �Լ����� ������ ���
	D3dUtilities* d3dUtilities;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, const std::wstring& filename);
	virtual void Initialize(ComPtr<ID3D11Device>& device);
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	virtual void Render(ComPtr<ID3D11DeviceContext>& context);
	virtual void Update(/*float _dt*/Vector3 vector, Matrix view, Matrix proj);

	// ExampleApp::Update()���� ����
	Nest::VertexConstantBuffer objectVertexConstantData;
	Nest::PixelShaderConstantBuffer objectPixelConstantData;

	// ExampleApp:Initialize()���� ����
	ComPtr<ID3D11ShaderResourceView> m_pObjectDiffuseResView;
	ComPtr<ID3D11ShaderResourceView> m_pObjectSpecularResView;

	// GUI���� ������Ʈ �� �� ���
	Nest::NormalShaderConstantBuffer objectNormalVertexConstantData;
	bool m_isChangingNormalFlag = true;
	bool m_isDrawNormals = false;

	// ��ü�� �������� ���� Ÿ�Ժ���
	virtual std::vector<Nest::MeshData> GetMeshType();
};