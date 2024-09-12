#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include <array>

/// <summary>
/// ������Ʈ�� �׸��� Ŭ����
/// 2024.1.25. �迹���� ���� �ǵ������ ������ �ͼ� �ٰŸ� ��� �ٲ��. DJ.
/// </summary> 

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
	NestGraphics::ModelData* m_pModelData;	// shared-ptr�� �����ؾ���


	// ������Ʈ ���̴��� �����ϴ� �������̽�
	ComPtr<ID3D11VertexShader> m_pVertexShader;					// ���� ���̴�
	ComPtr<ID3D11PixelShader> m_pPixelShader;					// �ȼ� ���̴�
	ComPtr<ID3D11InputLayout> m_pInputLayout;					// �Է� ���̾ƿ�, ���� ���̴��� �ԷµǴ� �������� ������ ����

	// ������Ʈ�� ���÷�
	ComPtr<ID3D11SamplerState> m_pSamplerState;

	/// ��������ȭ ���.
	// ������Ʈ�� ��� ����
	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;				// ������Ʈ ���ؽ� ��������� �Ҵ��� ���� �ѹ��� ���̴� ����
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;				// ������Ʈ �ȼ� ��������� �Ҵ��� ���� �ѹ��� ���̴� ����

	// ���� ����ϴ� �Լ����� ������ ���
	D3dUtilities* d3dUtilities;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys);	// assimp��  ������ �������� �޽� �ʱ�ȭ
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);	// ������� ������Ʈ
	virtual void Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);												// ����
	virtual void CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld);																					// ������Ʈ
	virtual void LightUpdate(float m_diffuse,float m_specualr, bool m_useTexture, float m_scale, int m_lightType, NestGraphics::Light m_lightFromGUI);
	// ExampleApp::Update()���� ����
	NestGraphics::VertexConstantBuffer objectVertexConstantData;		   // ������Ʈ ������ ������ ��� ���ؽ� �������
	NestGraphics::PixelShaderConstantBuffer objectPixelConstantData;	   // ������Ʈ ������ ������ ��� �ȼ� �������

	// ExampleApp:Initialize()���� ����
	ComPtr<ID3D11ShaderResourceView> m_pObjectDiffuseResView;			   ///ť����ο� ���ɰ� ����, ���� �̱���.
	ComPtr<ID3D11ShaderResourceView> m_pObjectSpecularResView;			   ///ť����ο� ���ɰ� ����, ���� �̱���.

public:
	std::vector<NestGraphics::MeshData>* getModelMeshData() const { return m_ModelMeshData; };
	void setModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) { m_ModelMeshData = _ModelMeshData; }
	std::string WstringToString(const std::wstring& wstr);

};