#pragma once
#include "directxtk/SimpleMath.h"
#include "Geometry.h"
#include "Light.h"

#define MAX_LIGHTS 3
#define MAX_MODEL_TRANSFORMS 250 //���� ����� ����
#define MAX_MODEL_KEYFRAMES  500

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

namespace NestGraphics
{

		///���ؽ� ��� ����
	class VertexConstantBuffer
	{
	public:
		VertexConstantBuffer() {};
		~VertexConstantBuffer() {};
	public:
		Matrix world;			//������ǥ
		Matrix invTranspose;	//������ǥ
		Matrix view;			//����Ʈ��ǥ
		Matrix projection;		//����������ǥ
		Matrix trsMatrix;		//TRS���
		Matrix BoneTransforms[MAX_MODEL_TRANSFORMS]; //�迹������ �߰���
		//NestGraphics::TweenDesc m_TweenDesc; //���� ������ ��� �߿����� ������
		//ComPtr<ID3D11ShaderResourceView> m_SRV; //�̰� �ٽ�
		unsigned __int32 padding[12]; // �е�����


	public:
		// operator= �����ε�
		VertexConstantBuffer& operator=(const VertexConstantBuffer& other)
		{
			if (this != &other) // �ڱ� �ڽſ��� �Ҵ��ϴ� ��츦 ����
			{
				world = other.world;
				invTranspose = other.invTranspose;
				view = other.view;
				projection = other.projection;
				for (int i = 0; i < MAX_MODEL_TRANSFORMS; i++)
				{
					BoneTransforms[i] = other.BoneTransforms[i];
				}
			}
			return *this;
		}
	};

		///�ȼ� �������
	class PixelShaderConstantBuffer
	{
	public:
		PixelShaderConstantBuffer() {}
		~PixelShaderConstantBuffer() {}
	public:
		Vector3 eyeWorld;         // 12 //������ȯ�� ���
		bool useTexture = 1;      // 1byte  //�ؽ�ó�� ����Ұ��ΰ� ����
		Material material;        // 48 //���׸���
		Light lights[MAX_LIGHTS]; // 48 * MAX_LIGHTS //��

	public:
		// operator= �����ε�
		PixelShaderConstantBuffer& operator=(const PixelShaderConstantBuffer& other)
		{
			if (this != &other) // �ڱ� �ڽſ��� �Ҵ��ϴ� ��츦 ����
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
	
	///�븻 ��� ����
	class NormalShaderConstantBuffer
	{
	public:
		NormalShaderConstantBuffer() {}
		~NormalShaderConstantBuffer() {}

	public:
		/*Matrix world;
		Matrix invTranspose;
		Matrix view;
		Matrix projection;*/ //�Ⱦ�
		float scale = 0.1f;		//ũ�� 
		float dummy[3];			//���̵�����

		// operator= �����ε�
		NormalShaderConstantBuffer& operator=(const NormalShaderConstantBuffer& other)
		{
			if (this != &other) // �ڱ� �ڽſ��� �Ҵ��ϴ� ��츦 ����
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