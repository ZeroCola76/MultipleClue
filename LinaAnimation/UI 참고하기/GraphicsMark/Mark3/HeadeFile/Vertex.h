#pragma once
#include <directxtk/SimpleMath.h>
#include <vector>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

namespace NestGraphics
{
	// 버텍스의 구조체
	struct Vertex {
		Vector3 position;
		Vector3 normal;
		Vector2 texcoord;
		//Vector3 tangent;	// 접선 벡터
		//Vector3 bitangent;	// 
		// Vector3 color;
	};
}

