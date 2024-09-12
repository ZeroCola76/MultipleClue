#include "Box.h"
#include "DXDefine.h"

Box::Box()
{

}

Box::~Box()
{
}

void Box::Initialize(ComPtr<ID3D11Device>& device)
{
	MeshGroup::Initialize(device);

	///TO DO : 김예리나 이따 살릴것 2024.1.24
	//d3dUtilities->CreateCubemapTexture(device, diffuseFilename, m_diffuseResView);
	//meshGroupSphere->m_pObjectDiffuseResView = cubeMapping->m_diffuseResView;
	//meshGroupSphere->m_pObjectSpecularResView = cubeMapping->m_specularResView;

	Matrix modelMat = Matrix::CreateTranslation({ 0.2f, 0.1f, 0.6f });
	Matrix invTransposeRow = modelMat;
	invTransposeRow.Translation(Vector3(0.0f));
	invTransposeRow = invTransposeRow.Invert().Transpose();
	objectVertexConstantData.world = modelMat.Transpose();
	objectVertexConstantData.invTranspose = invTransposeRow.Transpose();
	objectPixelConstantData.useTexture = false;
	objectPixelConstantData.material.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	objectPixelConstantData.material.specular = Vector3(0.0f);
}

void Box::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	MeshGroup::UpdateConstantBuffers(device, context);
}

void Box::Render(ComPtr<ID3D11DeviceContext>& context)
{
	MeshGroup::Render(context);
}

void Box::Update(Vector3 vector, Matrix view, Matrix proj)
{
	objectPixelConstantData.useTexture = true;
	objectPixelConstantData.eyeWorld = vector;
	objectVertexConstantData.view = view.Transpose();
	objectVertexConstantData.projection = proj.Transpose();
}

std::vector<NestGraphics::MeshData> Box::GetMeshType()
{
	NestGraphics::MeshData box = GameObject::MakeBox(0.1f);
	std::vector<NestGraphics::MeshData> meshes = { box };
	return meshes;
}

