#pragma once
#include <string>
#include "Vertex.h"

using std::vector;

namespace NestGraphics
{
	// 메쉬 데이터 : 기본적으로 버텍스의 위치와 인덱스를 저장하고 있다.
	struct MeshData 
	{
	std::vector<NestGraphics::Vertex> vertices;
	std::vector<uint16_t> indices;
	std::string materialName;		// mesh의 material의 이름
	std::string textureFilename; // 텍스처가 없으면 적용하지 않기 위한 플래그
	};
}


