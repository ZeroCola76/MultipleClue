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

	void Initialize(ComPtr<ID3D11Device>& device);
	void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void Update(Vector3 vector, Matrix view, Matrix proj);
	std::vector<NestGraphics::MeshData> GetMeshType();

	//������ �д� �׷�..�׷� ������ �ʿ��ѵ� ���ҽ��Ŵ����� ���߾˰Ͱ���..->�̺κ��� ������ �� �ϴ°� ��ģ ���̴�.
	//void GetTexturePath(const std::string filename, const std::string filename2,
	//	const wstring filename3, const wstring filename4); //�ϴ� �׳� �״�� ��������! ������ �ͺ��� ����.

};

