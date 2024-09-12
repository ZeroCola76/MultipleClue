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

	virtual void Initialize(ComPtr<ID3D11Device>& device) override;
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) override;
	virtual void Render(ComPtr<ID3D11DeviceContext>& context) override;
	virtual void Update(Vector3 vector, Matrix view, Matrix proj) override;
	virtual std::vector<Nest::MeshData> GetMeshType() override;
};

