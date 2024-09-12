#include <initializer_list>
#include "MeshGroup.h"
#include <stb_image.h>

MeshGroup::MeshGroup()
{
	d3dUtilities = new D3dUtilities();
}

MeshGroup::~MeshGroup()
{
	delete d3dUtilities;
}

void MeshGroup::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix _trsMatrix, std::initializer_list<std::wstring> textureFileKeys)
{

	std::vector<NestGraphics::MeshData>* m_modelMeshData = &modelMeshDataes->meshes;
	std::vector<NestGraphics::Bone>* m_modelBoneData = &modelMeshDataes->bones;
	std::vector<NestGraphics::asAnimation>* m_modelAniData = &modelMeshDataes->animations;

	m_pModelData = modelMeshDataes;

	//std::vector<NestGraphics::MeshData> a = modelMeshDataes->meshes;

	std::initializer_list<std::wstring>::iterator textureFileKeysIterator = textureFileKeys.begin();
	for (size_t i = 0; i < m_modelMeshData->size() && i < textureFileKeys.size(); i++, textureFileKeysIterator++)
	{
		(*m_modelMeshData)[i].textureFilename = WstringToString(*textureFileKeysIterator);
	}

	// ������ ���� �並 ����
	objectVertexConstantData.world = Matrix();			// ��ü�� ���ؽ� ��� ������ ���� ���� 4X4��Ŀ� �����Ѵ�.
	objectVertexConstantData.view = Matrix();			// ��ü�� ���ؽ� ��� ������ ī�޶� �� ���� 4X4��Ŀ� �����Ѵ�.
	objectVertexConstantData.projection = Matrix();		// ��ü�� ���ؽ� ��� ������ ���� �� ���� 4X4��Ŀ� �����Ѵ�.

	d3dUtilities->CreateConstantBuffer(device, objectVertexConstantData, m_pVertexConstantBuffer); // ������Ʈ ���� ������ �����͸� �ʱ� �Ҵ� ���ۿ� ���� �� �����͸� �������� �ʱ� �Ҵ� ���۸� ����!
	d3dUtilities->CreateConstantBuffer(device, objectPixelConstantData, m_pPixelConstantBuffer); // ������Ʈ �ȼ� ������ �����͸� �ʱ� �Ҵ� ���ۿ� ���� �� �����͸� �������� �ʱ� �Ҵ� ���۸� ����!

	///for���� ���鼭 ���ӿ������� ���� �����͸� ������Ʈ ��Ų��.
	for (const auto& meshData : (*m_modelMeshData))
	{
		std::shared_ptr<NestGraphics::Mesh> newMesh = std::make_shared<NestGraphics::Mesh>();	// Mesh ����ü�� �������� �Ҵ��Ѵ�.  
		d3dUtilities->CreateVertexBuffer(device, meshData.vertices, newMesh->m_vertexBuffer);	// ���ؽ� ���� �����͸� �޾ƿ��� �����͸� �������� �����Ѵ�.
		newMesh->m_indexCount = UINT(meshData.indices.size());									// �ε��� ������ �����Ѵ�. 
		d3dUtilities->CreateIndexBuffer(device, meshData.indices, newMesh->m_indexBuffer);		// �ε��� ���۸� �����ϰ� �־��� �����͸� �������� �����Ѵ�.


		if (!meshData.textureFilename.empty())																					 //�����Ͱ� ������ ���
		{
			std::cout << meshData.textureFilename << std::endl;
			d3dUtilities->CreateTexture(device, meshData.textureFilename, newMesh->texture, newMesh->textureResourceView);		 //�ý��ĸ� �����Ѵ�.
		}

		newMesh->m_vertexConstantBuffer = m_pVertexConstantBuffer;																 //newMesh���� vertexConstantBuffer�� ����
		newMesh->m_pixelConstantBuffer = m_pPixelConstantBuffer;																 //newMesh���� pixelConstantBuffer�� ����

		newMesh->boneIndex = meshData.boneIndex;

		this->m_pMeshes.push_back(newMesh);																						 //���� �ȿ� �ִ´�.
	}

	// �ԷµǴ� �����Ͱ��� � �������� ����	///���̴� ����
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
 		{"BLEND_INDICES", 0 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,D3D11_INPUT_PER_VERTEX_DATA,0},
 		{"BLEND_WEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	// ���ؽ��� hlsl�κ��� ������ �����ͼ� �Է� �����Ϳ� �ִ´�. // �ڼ��� ������ ���� �Լ� ���ø��� �����
	d3dUtilities->CreateVertexShaderAndInputLayout(device, L"../NestResources/hlsl/VertexShader.hlsl", inputElements, m_pVertexShader, m_pInputLayout);
	// CreateVertexShaderAndInputLayout()�� ���� ����
	d3dUtilities->CreatePixelShader(device, L"../NestResources/hlsl/PixelShader.hlsl", m_pPixelShader);

	// Sampler �����
	D3D11_SAMPLER_DESC sampDesc;						// ���÷� ���¸� ����
	ZeroMemory(&sampDesc, sizeof(sampDesc));			// ���÷��� �ʱ�ȭ �Ѵ�.
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���÷��� ���͸� ���� : ���, ���� �� �� ���� ���ø��� ���� ������ ���
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// 0���� 1 ���� �ۿ� �ִ� u �ؽ�ó ��ǥ�� Ȯ���ϴ� �� ��� : ��� (u,v) ���� ���տ��� �ؽ�ó�� Ÿ��ȭ
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// 0���� 1 ���� �ۿ� �ִ� v �ؽ�ó ��ǥ�� Ȯ���ϴ� �� ��� : "
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// 0���� 1 ���� �ۿ� �ִ� w �ؽ�ó ��ǥ�� Ȯ���ϴ� �� ����� �޼��� : "
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	// ���ø��� �����͸� ���� ���ø��� �����Ϳ� ���ϴ� �Լ� : �� �Ұ�
	sampDesc.MinLOD = 0;								// �׼����� ������ mipmap ������ �Ʒ��� ��(float)
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;				// �׼����� ������ mipmap ������ ���� ��
	device->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()); // �ؽ�ó�� ���� ���ø� ������ ĸ��ȭ�ϴ� ���÷� ���� ��ü�� �����. 
	// sampDesc : ���÷� ���� ���� ���� ������, m_pSamplerState : ���÷� ���� ��ü

	/// ���������� : SRV ���� -> ��� ���� ���� -> ��ǲ ����� ���� -> VS ���̴� -> PS ���̴�

	/// ���� �� �κ��� ������Ʈ ���۳��� ������ �� �Ͼ��? �׷����� ���� �� ����.

	//��������� ����°� �ν� �����ִ� �κ�
	//�޽��� ��ġ //���� ����� ���� ���� ���͸� ��ȯ �� �������� ������ ����

	Matrix invTRS = _trsMatrix;					// ������� ��� ������ ������ �����.
	invTRS.Translation(Vector3(0.0f));			// �̵� ��� �κи� 0, 0, 0���� �ٲ��ش�.
	invTRS = invTRS.Invert().Transpose();		//�����, ��ġ����� ���� �־��ش�.
	objectVertexConstantData.world = _trsMatrix.Transpose();	// �޾ƿ� ����� ��ġ ����� ���� ��ǥ�� �־��ش�.
	objectVertexConstantData.invTranspose = invTRS.Transpose();
	objectPixelConstantData.useTexture = false;
	objectPixelConstantData.material.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	objectPixelConstantData.material.specular = Vector3(0.0f);
	UpdateConstantBuffers(device, context);

	NestGraphics::VertexConstantBuffer a;
	std::cout << "123123123123123123123123123123123123123123123\n";
	std::cout << sizeof(a)<<std::endl;
}

// ��� ���۸� ������Ʈ �ϴ� �Լ�(����̽�, ������ ����� �����ϴ� ����̽� ���ؽ�Ʈ)
void MeshGroup::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	d3dUtilities->UpdateBuffer(device, context, objectVertexConstantData, m_pVertexConstantBuffer); // ���ؽ� ��� ���� ������ ���� ���� ���۸� ������Ʈ �Ѵ�.
	d3dUtilities->UpdateBuffer(device, context, objectPixelConstantData, m_pPixelConstantBuffer);	// ����� �ȼ� ���� ������ ������Ʈ �ȴ�.

}

/// <summary>
/// �޽��� �����ϴ� �Լ�
/// </summary>
/// <param name="context">�������� �����ϴ� ���ؽ�Ʈ</param>
void MeshGroup::Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) /// ���⿡ int�� SRV ������ �Ű������� �޾ƿ;� �ϰ� �� ��..
{
	// ���ؽ�/�ε��� ���� ����
	UINT stride = sizeof(NestGraphics::Vertex); //���ؽ��� �� ��� ������ ������ ��Ÿ���� �����Դϴ�. (���ؽ� ���ۿ��� ���� ���ؽ��� �̵��� �� ���)
	UINT offset = 0;							//���ؽ� ������ ���� ��ġ������ ������

	// ���ؽ� ���̴��� �����Ѵ�
	context->VSSetShader(m_pVertexShader.Get(), 0, 0);				// ���ؽ� ���̴� ����
	context->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());	// �ȼ� ���÷� ����
	context->PSSetShader(m_pPixelShader.Get(), 0, 0);				// �ȼ� ���̴� ����

	// ���ؽ�/�ε��� ���� ����
	for (const auto& meshes : m_pMeshes)
	{

		ID3D11ShaderResourceView* resViews[1] =												// �ȼ� ���̴����� ����� ���̴� ���ҽ� �並 ����(�ؽ�ó ������ŭ �ִ´�.)
		{ meshes->textureResourceView.Get()};
		context->PSSetShaderResources(0, 1, resViews);										// �ȼ� ���̴��� ���̴� ���ҽ� �並 ���ε�
		context->IASetInputLayout(m_pInputLayout.Get());									//�Է� ���̾ƿ��� ����
		context->IASetVertexBuffers(0, 1, meshes->m_vertexBuffer.GetAddressOf(), &stride, &offset); // ���ؽ� ���۸� ����
		context->IASetIndexBuffer(meshes->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);	// �ε��� ���۸� ����
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);				// �⺻���� ������Ƽ�� ������ ���� : �ﰢ��
		context->VSSetConstantBuffers(0, 1, meshes->m_vertexConstantBuffer.GetAddressOf()); // ���ؽ� ���̴����� ����� ��� ���۸� ����
		context->PSSetConstantBuffers(0, 1, meshes->m_pixelConstantBuffer.GetAddressOf());	// �ȼ� ���̴����� ����� ��� ���۸� ����
		// ��ü �������� �� ť��ʵ� ���� ���
		context->DrawIndexed(meshes->m_indexCount, 0, 0);									// �ε����� ���ؽ��� ����Ͽ� ������ (�������� �ε��� �� ��)
	}

}


void MeshGroup::CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld)
{
	//��ġ���� ����
	objectPixelConstantData.useTexture = true;
	objectPixelConstantData.eyeWorld = camEyeWorld;
	objectVertexConstantData.view = camViewRow.Transpose();
	objectVertexConstantData.projection = camProjRow.Transpose();

	UpdateConstantBuffers(device, context);
}

void MeshGroup::LightUpdate(float m_diffuse, float m_specualr, bool m_useTexture, float m_norScale, int m_lightType, NestGraphics::Light m_lightFromGUI)
{
	//������ ���� (��� �������� �ִ°� �ƴϴ�.) ä����� : �ȼ� ���� �����Ϳ� ��ǻ��� ����ŧ�� ���� �����Ѵ�.
	objectPixelConstantData.material.diffuse = Vector3(m_diffuse);
	objectPixelConstantData.material.specular = Vector3(m_specualr);
	objectPixelConstantData.useTexture = m_useTexture;

	// ���� �� ���� ����ϱ� ����, ������� �ʴ� ������ ���� ������ 0���� �����ؼ� �׸�
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		// �ٸ� ���� ����
		if (i != m_lightType)
		{
			objectPixelConstantData.lights[i].strength *= 0.0f;
		}
		else
		{
			objectPixelConstantData.lights[i] = m_lightFromGUI;
		}
	}
}

std::string MeshGroup::WstringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}