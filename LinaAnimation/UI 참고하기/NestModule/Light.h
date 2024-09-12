#pragma once
#include "directxtk/SimpleMath.h"
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

namespace NestGraphics
{
	// 조명
	struct Light 
	{
		Vector3 strength = Vector3(1.0f);              // 12BYTE 빛의 세기 - 빛은 1, 빛을 발광하지 않는 물체는 0
		float fallOffStart = 0.0f;                     // 4 //빛의 밝기가 어두워지기 시작하는 곳.	//point Light
		Vector3 direction = Vector3(0.0f, 0.0f, 1.0f); // 12 //방향
		float fallOffEnd = 10.0f;                      // 4 //빛이 끝나는 곳						//point Light
		Vector3 position = Vector3(0.0f, 0.0f, -2.0f); // 12 //빛의 위치
		float spotPower = 1.0f;                        // 4 //?

	};
}