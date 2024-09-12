#pragma once
#include "directxtk/SimpleMath.h"
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

namespace NestGraphics
{
	// ����
	struct Light 
	{
		Vector3 strength = Vector3(1.0f);              // 12BYTE ���� ���� - ���� 1, ���� �߱����� �ʴ� ��ü�� 0
		float fallOffStart = 0.0f;                     // 4 //���� ��Ⱑ ��ο����� �����ϴ� ��.	//point Light
		Vector3 direction = Vector3(0.0f, 0.0f, 1.0f); // 12 //����
		float fallOffEnd = 10.0f;                      // 4 //���� ������ ��						//point Light
		Vector3 position = Vector3(0.0f, 0.0f, -2.0f); // 12 //���� ��ġ
		float spotPower = 1.0f;                        // 4 //?

	};
}