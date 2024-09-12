#pragma once
#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include "NestRenderer.h"

/// <summary>
/// ������Ʈ�� �׸��� Ŭ����
/// 2024.1.25. �迹���� ���� �ǵ������ ������ �ͼ� �ٰŸ� ��� �ٲ��. DJ.
/// </summary>

class NestRenderer;

class MeshGroup
{
public:
	MeshGroup();
	virtual ~MeshGroup();

protected:
	// �ϳ��� 3D ���� ���������δ� �������� �޽��� ����
	std::vector<std::shared_ptr<NestGraphics::Mesh>> m_pMeshes; // ������Ʈ �޽��� ����ü�� ����Ű�� ������
	// �Ž� �����͸� �޾ƿ��� ���� ������
	std::vector<NestGraphics::MeshData>* m_ModelMeshData;
	std::wstring m_modelFilePath;
	float m_modelScale;

	// ������Ʈ ���̴��� �����ϴ� �������̽�
	ComPtr<ID3D11VertexShader> m_pVertexShader;					// ���� ���̴�
	ComPtr<ID3D11PixelShader> m_pPixelShader;					// �ȼ� ���̴�
	ComPtr<ID3D11InputLayout> m_pInputLayout;					// �Է� ���̾ƿ�, ���� ���̴��� �ԷµǴ� �������� ������ ����

	// ������Ʈ�� ���÷�
	ComPtr<ID3D11SamplerState> m_pSamplerState;

	// ������Ʈ�� ��� ����
	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;				// ������Ʈ ���ؽ� ��������� �Ҵ��� ���� �ѹ��� ���̴� ����
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;				// ������Ʈ �ȼ� ��������� �Ҵ��� ���� �ѹ��� ���̴� ����

	// �޽��� ��� ���� �׸���
	ComPtr<ID3D11VertexShader> m_pNormalVertexShader;			// ��� ���� ���̴�
	ComPtr<ID3D11PixelShader> m_pNormalPixelShader;				// ��� �ȼ� ���̴�

	std::shared_ptr<NestGraphics::Mesh> m_pNormalLines;			// ��� �޽��� ����ü�� ����Ű�� ������

	ComPtr<ID3D11Buffer> m_pObjectNormalVertexConstantBuffer;	// ������Ʈ�� �븻 ���ؽ� ������� �����ϴ� ����
	ComPtr<ID3D11Buffer> m_pObjectNormalPixelConstantBuffer;	// ������Ʈ�� �븻 �ȼ� ������� �����ϴ� ����

	// ���� ����ϴ� �Լ����� ������ ���
	D3dUtilities* d3dUtilities;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, vector<NestGraphics::MeshData>* modelMeshDataes);	// assimp��  ������ �������� �޽� �ʱ�ȭ
	//virtual void Initialize(ComPtr<ID3D11Device>& device, std::vector<NestGraphics::MeshData> meshes);		// ť�� ���ν� ���̴� �ʱ�ȭ
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);	// ������� ������Ʈ
	virtual void Render(ComPtr<ID3D11DeviceContext>& context);												// ����
	
	// ExampleApp::Update()���� ����
	NestGraphics::VertexConstantBuffer objectVertexConstantData;		   // ������Ʈ ������ ������ ��� ���ؽ� �������
	NestGraphics::PixelShaderConstantBuffer objectPixelConstantData;	   // ������Ʈ ������ ������ ��� �ȼ� �������

	// ExampleApp:Initialize()���� ����
	ComPtr<ID3D11ShaderResourceView> m_pObjectDiffuseResView;			   ///ť����ο� ���ɰ� ����, ���� �̱���.
	ComPtr<ID3D11ShaderResourceView> m_pObjectSpecularResView;			   ///ť����ο� ���ɰ� ����, ���� �̱���.

	// GUI���� ������Ʈ �� �� ���
	NestGraphics::NormalShaderConstantBuffer objectNormalVertexConstantData;	// ������Ʈ ������ ������ ��� ���ؽ� ��� �������
	bool m_isChangingNormalFlag = true;											// ��� ������ ���� ���ϴ��� ���ο� ���� �÷���
	bool m_isDrawNormals = false;												// ��� ���� �׸����� üũ(IMGUI)

public:
	std::vector<NestGraphics::MeshData>* getModelMeshData() const { return m_ModelMeshData; };
	void setModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) { m_ModelMeshData = _ModelMeshData; }
	//void SetMeshModelFilePath(std::vector<NestGraphics::MeshData>* _ModelMeshData, const std::wstring& _filePath, const float& _scale);
};