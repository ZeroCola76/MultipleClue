#pragma once
#include <directxtk/SimpleMath.h>
#include <vector>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

namespace NestGraphics
{
	// ���ؽ��� ����ü
	struct Vertex {
		Vector3 position;
		Vector3 normal;
		Vector2 texcoord;
		//Vector3 tangent;	// ���� ����
		//Vector3 bitangent;	// 
		// Vector3 color;
	};
}

