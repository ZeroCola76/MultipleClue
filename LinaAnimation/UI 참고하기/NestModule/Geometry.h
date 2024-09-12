#pragma once
#include <string>
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "directxtk/SimpleMath.h"
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using Microsoft::WRL::ComPtr;

namespace NestGraphics
{
	//버텍스의 구조체
	struct Vertex
	{
		Vector3 position;							//포지션 
		Vector3 normal;								//노말 벡터
		Vector2 texcoord;							//덱스쳐 좌표
		//Vector3 color;
	};

	// 메쉬 데이터 : 기본적으로 버텍스의 위치와 인덱스를 저장하고 있다.
	struct MeshData 
	{
		std::vector<Vertex> vertices;				//Vertex구조체의 벡터
		std::vector<uint16_t> indices;				//index순서를 저장하는 indices
		std::string materialName;					//mesh의 material의 이름
		std::string textureFilename;				//텍스쳐의 이름
	};

	//메쉬의 정보
	struct Mesh 
	{												 
		ComPtr<ID3D11Buffer> m_vertexBuffer;		 // GPU에 vertex정보의 전달을 위해, 나중에 메모리누수 체크해보자.
		ComPtr<ID3D11Buffer> m_indexBuffer;			 // GPU에 vertex의 연결 정보를 전달
		ComPtr<ID3D11Buffer> m_vertexConstantBuffer; // vertexShader에 전달하는 상수데이터
		ComPtr<ID3D11Buffer> m_pixelConstantBuffer;	 // pixelShader에 전달하는 상수데이터

		// 그래픽스에서 아직 고민중이라고 함 
		ComPtr<ID3D11Texture2D> texture;			 //텍스쳐
		ComPtr<ID3D11ShaderResourceView> textureResourceView;		//셰이더 리소스 뷰 (리소스 데이터를 읽는다. >> 아직 쓰이는곳 X)

		UINT m_indexCount;		//몇번째 메쉬인지 확인하기 위한 인덱스
	};


	// 재질
	struct Material 
	{
		Vector3 ambient = Vector3(0.1f);  // 12byte
		float shininess = 1.0f;           // 4byte	/광택

		Vector3 diffuse = Vector3(0.5f);  // 12byte	/방향
		float dummy1;                     // 4byte	/더미 데이터

		Vector3 specular = Vector3(0.5f); // 12byte
		float dummy2;                     // 4byte	/더미 데이터
	};

	struct ModelData
	{
		std::vector<MeshData> meshes;   // 모델에 속한 메시들의 배열
	};
}