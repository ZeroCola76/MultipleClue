#pragma once
#include "MeshGroup.h"

/// <summary>
/// 2D 사각형. 
/// </summary>
class Quad : public MeshGroup
{
public:
	Quad(ComPtr<ID3D11Device>& device);
	~Quad();
	//파일을 읽는 그런..그런 역할이 필요한데 리소스매니저를 봐야알것같다..
	//void GetTexturePath();
};

