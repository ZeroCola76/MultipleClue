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
	//���ؽ��� ����ü
	struct Vertex
	{
		Vector3 position;							//������ 
		Vector3 normal;								//�븻 ����
		Vector2 texcoord;							//������ ��ǥ
		//Vector3 color;
	};

	// �޽� ������ : �⺻������ ���ؽ��� ��ġ�� �ε����� �����ϰ� �ִ�.
	struct MeshData 
	{
		std::vector<Vertex> vertices;				//Vertex����ü�� ����
		std::vector<uint16_t> indices;				//index������ �����ϴ� indices
		std::string materialName;					//mesh�� material�� �̸�
		std::string textureFilename;				//�ؽ����� �̸�
	};

	//�޽��� ����
	struct Mesh 
	{												 
		ComPtr<ID3D11Buffer> m_vertexBuffer;		 // GPU�� vertex������ ������ ����, ���߿� �޸𸮴��� üũ�غ���.
		ComPtr<ID3D11Buffer> m_indexBuffer;			 // GPU�� vertex�� ���� ������ ����
		ComPtr<ID3D11Buffer> m_vertexConstantBuffer; // vertexShader�� �����ϴ� ���������
		ComPtr<ID3D11Buffer> m_pixelConstantBuffer;	 // pixelShader�� �����ϴ� ���������

		// �׷��Ƚ����� ���� ������̶�� �� 
		ComPtr<ID3D11Texture2D> texture;			 //�ؽ���
		ComPtr<ID3D11ShaderResourceView> textureResourceView;		//���̴� ���ҽ� �� (���ҽ� �����͸� �д´�. >> ���� ���̴°� X)

		UINT m_indexCount;		//���° �޽����� Ȯ���ϱ� ���� �ε���
	};


	// ����
	struct Material 
	{
		Vector3 ambient = Vector3(0.1f);  // 12byte
		float shininess = 1.0f;           // 4byte	/����

		Vector3 diffuse = Vector3(0.5f);  // 12byte	/����
		float dummy1;                     // 4byte	/���� ������

		Vector3 specular = Vector3(0.5f); // 12byte
		float dummy2;                     // 4byte	/���� ������
	};

	struct ModelData
	{
		std::vector<MeshData> meshes;   // �𵨿� ���� �޽õ��� �迭
	};
}