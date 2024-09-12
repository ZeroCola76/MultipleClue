#include "Sphere.h"

Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void Sphere::Initialize(ComPtr<ID3D11Device>& device)
{
	MeshGroup::Initialize(device);

	///TO DO : �迹���� �̵� �츱�� 2024.1.24
	//d3dUtilities->CreateCubemapTexture(device, diffuseFilename, m_diffuseResView);
	//meshGroupSphere->m_pObjectDiffuseResView = cubeMapping->m_diffuseResView;
	//meshGroupSphere->m_pObjectSpecularResView = cubeMapping->m_specularResView;

	Matrix modelMat = Matrix::CreateTranslation({ -0.2f, 0.1f, 0.6f });
	Matrix invTransposeRow = modelMat;
	invTransposeRow.Translation(Vector3(0.0f));
	invTransposeRow = invTransposeRow.Invert().Transpose();
	objectVertexConstantData.world = modelMat.Transpose();
	objectVertexConstantData.invTranspose = invTransposeRow.Transpose();
	objectPixelConstantData.useTexture = false;
	objectPixelConstantData.material.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	objectPixelConstantData.material.specular = Vector3(0.0f);
}

void Sphere::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	MeshGroup::UpdateConstantBuffers(device, context);
}

void Sphere::Render(ComPtr<ID3D11DeviceContext>& context)
{
	MeshGroup::Render(context);
}

void Sphere::Update(Vector3 vector, Matrix view, Matrix proj)
{
	objectPixelConstantData.useTexture = true;
	objectPixelConstantData.eyeWorld = vector;
	objectVertexConstantData.view = view.Transpose();
	objectVertexConstantData.projection = proj.Transpose();
}

std::vector<NestGraphics::MeshData> Sphere::GetMeshType()
{
	NestGraphics::MeshData sphere = GameObject::MakeSphere(0.1f, 20, 20);
	std::vector<NestGraphics::MeshData> meshes = { sphere };
	return meshes;
}
