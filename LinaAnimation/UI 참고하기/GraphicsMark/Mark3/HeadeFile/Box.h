#pragma once
#include "MeshGroup.h"

/// <summary>
/// �츮�� �ƴ� �ڽ�. �簢��
/// </summary>
class Box : public MeshGroup
{
public:
	Box();
	~Box();

	virtual void Initialize(ComPtr<ID3D11Device>& device) override;
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) override;
	virtual void Render(ComPtr<ID3D11DeviceContext>& context) override;
	virtual void Update(Vector3 vector, Matrix view, Matrix proj) override;
	virtual std::vector<Nest::MeshData> GetMeshType() override;

	//������ �д� �׷�..�׷� ������ �ʿ��ѵ� ���ҽ��Ŵ����� ���߾˰Ͱ���..->�̺κ��� ������ �� �ϴ°� ��ģ ���̴�.
	//void GetTexturePath(const std::string filename, const std::string filename2,
	//	const wstring filename3, const wstring filename4); //�ϴ� �׳� �״�� ��������! ������ �ͺ��� ����.

};

