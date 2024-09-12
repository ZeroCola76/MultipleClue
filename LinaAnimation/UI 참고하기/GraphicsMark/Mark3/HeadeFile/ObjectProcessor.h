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

private:
	// �ϳ��� 3D ���� ���������δ� �������� �޽��� ����
	std::vector<std::shared_ptr<Nest::Mesh>> m_pMeshes; // ������Ʈ �޽��� ����ü�� ����Ű�� ������

	// ������Ʈ ���̴��� �����ϴ� �������̽�
	ComPtr<ID3D11VertexShader> m_pVertexShader; // ����
	ComPtr<ID3D11PixelShader> m_pPixelShader;   // �ȼ�
	ComPtr<ID3D11InputLayout> m_pInputLayout; // �Է� ���̾ƿ�, ���� ���̴��� �ԷµǴ� �������� ������ ����

	ComPtr<ID3D11SamplerState> m_pSamplerState;

	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;

	// �޽��� ��� ���� �׸���
	ComPtr<ID3D11VertexShader> m_pNormalVertexShader; // ��� ����
	ComPtr<ID3D11PixelShader> m_pNormalPixelShader;   // ��� �ȼ�
	std::shared_ptr<Nest::Mesh> m_pNormalLines; // ��� �޽��� ����ü�� ����Ű�� ������

	ComPtr<ID3D11Buffer> m_pObjectNormalVertexConstantBuffer;
	ComPtr<ID3D11Buffer> m_pObjectNormalPixelConstantBuffer;

private:
	D3dUtilities* d3dUtilities;

};