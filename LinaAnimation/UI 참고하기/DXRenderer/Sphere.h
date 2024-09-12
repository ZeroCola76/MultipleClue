#pragma once
#include "MeshGroup.h"
/// <summary>
/// ±¸
/// </summary>
class Sphere : public MeshGroup
{
public:
	Sphere();
	~Sphere();

	void Initialize(ComPtr<ID3D11Device>& device);
	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void Update(Vector3 vector, Matrix view, Matrix proj);
	std::vector<NestGraphics::MeshData> GetMeshType();
};

