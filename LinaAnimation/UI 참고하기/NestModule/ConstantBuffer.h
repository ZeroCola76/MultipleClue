#pragma once
#include "directxtk/SimpleMath.h"
#include "Geometry.h"
#include "Light.h"

#define MAX_LIGHTS 3
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

namespace NestGraphics
{
	//버텍스 상수 버퍼
	struct VertexConstantBuffer
	{
		Matrix world; //로컬좌표
		Matrix invTranspose; //반전좌표
		Matrix view; //뷰포트좌표
		Matrix projection; //포르젝션좌표

		// operator= 오버로딩
		VertexConstantBuffer& operator=(const VertexConstantBuffer& other)
		{
			if (this != &other) // 자기 자신에게 할당하는 경우를 방지
			{
				world = other.world;
				invTranspose = other.invTranspose;
				view = other.view;
				projection = other.projection;
			}
			return *this;
		}
	};

	//픽셀 상수버퍼
	struct PixelShaderConstantBuffer
	{
		Vector3 eyeWorld;         // 12 //시점변환에 사용
		bool useTexture = 1;      // 1byte  //텍스처를 사용할것인가 여부
		Material material;        // 48 //메테리얼
		Light lights[MAX_LIGHTS]; // 48 * MAX_LIGHTS //빛

		// operator= 오버로딩
		PixelShaderConstantBuffer& operator=(const PixelShaderConstantBuffer& other)
		{
			if (this != &other) // 자기 자신에게 할당하는 경우를 방지
			{
				eyeWorld = other.eyeWorld;
				useTexture = other.useTexture;
				material = other.material;
				for (int i = 0; i < MAX_LIGHTS; ++i) {
					lights[i] = other.lights[i];
				}
			}
			return *this;
		}
	};

	//노말 상수 버퍼
	struct NormalShaderConstantBuffer
	{
		/*Matrix world;
		Matrix invTranspose;
		Matrix view;
		Matrix projection;*/ //안씀

		float scale = 0.1f; //크기 
		float dummy[3]; //더미데이터

		// operator= 오버로딩
		NormalShaderConstantBuffer& operator=(const NormalShaderConstantBuffer& other)
		{
			if (this != &other) // 자기 자신에게 할당하는 경우를 방지
			{
				scale = other.scale;
				for (int i = 0; i < 3; ++i) {
					dummy[i] = other.dummy[i];
				}
			}
			return *this;
		}
	};
}