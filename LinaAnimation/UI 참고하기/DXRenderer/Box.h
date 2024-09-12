#pragma once
#include "MeshGroup.h"

/// <summary>
/// 우리가 아는 박스. 사각형
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

	//파일을 읽는 그런..그런 역할이 필요한데 리소스매니저를 봐야알것같다..->이부분을 일일히 다 하는건 미친 짓이다.
	//void GetTexturePath(const std::string filename, const std::string filename2,
	//	const wstring filename3, const wstring filename4); //일단 그냥 그대로 가져오자! 터지는 것보단 낫다.

};

