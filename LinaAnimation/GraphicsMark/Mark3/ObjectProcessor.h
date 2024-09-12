#pragma once
#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"

/// <summary>
/// ������Ʈ�� �׸��� Ŭ����
/// 2024.1.25. �迹���� ���� �ǵ������ ������ �ͼ� �ٰŸ� ��� �ٲ��. DJ.
/// </summary>

class ObjectProcessor
{
public:
	ObjectProcessor();
	virtual ~ObjectProcessor();

protected:
	// �ϳ��� 3D ���� ���������δ� �������� �޽��� ����
	std::vector<std::shared_ptr<NestGraphics::Mesh>> m_pMeshesVec; // ������Ʈ �޽��� ����ü�� ����Ű�� ������
	// �Ž� �����͸� �޾ƿ��� ���� ������
	std::vector<NestGraphics::MeshData>* m_pModelMeshDataVec;
	std::wstring m_modelFilePath;
	float m_modelScale;

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

	// �޽��� ��� ���� �׸���
	ComPtr<ID3D11VertexShader> m_pNormalVertexShader;			// ��� ���� ���̴�
	ComPtr<ID3D11PixelShader> m_pNormalPixelShader;				// ��� �ȼ� ���̴�

	std::shared_ptr<NestGraphics::Mesh> m_pNormalLines;			// ��� �޽��� ����ü�� ����Ű�� ������

	/// ��������ȭ ���.
	ComPtr<ID3D11Buffer> m_pObjectNormalVertexConstantBuffer;	// ������Ʈ�� �븻 ���ؽ� ������� �����ϴ� ����
	ComPtr<ID3D11Buffer> m_pObjectNormalPixelConstantBuffer;	// ������Ʈ�� �븻 �ȼ� ������� �����ϴ� ����

	// ���� ����ϴ� �Լ����� ������ ���
	D3dUtilities* m_pD3dUtilities;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, 
							Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys);	// assimp��  ������ �������� �޽� �ʱ�ȭ
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);	// ������� ������Ʈ
	virtual void Render(ComPtr<ID3D11DeviceContext>& context);												// ����
	virtual void CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, 
						   Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld);					// ������Ʈ
	virtual void LightUpdate(float m_diffuse,float m_specualr, bool m_useTexture, float m_scale, int m_lightType, NestGraphics::Light m_lightFromGUI);
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
	std::vector<NestGraphics::MeshData>* getModelMeshData() const { return m_pModelMeshDataVec; };
	void setModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) { m_pModelMeshDataVec = _ModelMeshData; }
	std::string WstringToString(const std::wstring& wstr);
};