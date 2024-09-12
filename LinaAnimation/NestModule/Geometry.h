#pragma once
#include <string>
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include "directxtk/SimpleMath.h"
#include <memory>
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using Microsoft::WRL::ComPtr;
using namespace DirectX::SimpleMath;

namespace NestGraphics
{
	//�迹������ �ۼ��� Bone Ŭ����
	///���⼭���� ���������� ����ü
	class Bone
	{
	public:
		Bone() {};
		~Bone() {};

	public:
		std::string name; //�̸�
		__int32 index = -1; //���� �ε���?
		__int32 parentIndex = -1; //�θ��� �ε���
		Matrix localTransform; //������ �θ���� ������� ���
		Matrix offsetTrm;
	};

	// Animation
	struct asBlendWeight
	{
		// ���� ����� 4��
		// index : indices
		// ��Ͽ��� �� ��°����
		// boneIndex : ���� ���� ����Ű����
		// weight : ����ġ (���� �޴� ����)
		void Set(unsigned __int32 index, unsigned __int32 boneIndex, float weight)
		{
			float i = static_cast<float>(boneIndex);
			float w = weight;

			switch (index)
			{
				case 0: indices.x = i; weights.x = w; break;
				case 1: indices.y = i; weights.y = w; break;
				case 2: indices.z = i; weights.z = w; break;
				case 3: indices.w = i; weights.w = w; break;
			}
		}

		Vector4 indices = Vector4(0.f, 0.f, 0.f, 0.f);
		Vector4 weights = Vector4(0.f, 0.f, 0.f, 0.f);
	};

	// �������� -> (������ȣ, ����ġ) �� ����ִ´�.
// �̰����� �۾��� ������ asBlendWeight struct�� �Ѱ��ش�.
	struct asBoneWeights
	{
		// ����ġ�� ���� ������� boneWeights�� insert�ȴ�. 
		// ex : 3 7 1 5 -> 7 5 3 1
		//
		// ������ ������ �ִ� bone�� ���� ��, �ش� ������
		// ���� ����� �ִ� bone�� ����ġ�� ���� ���� ������� ���������ν�
		// ������ �������� �� �ڿ������� ���̰� �� �� �ִ�.
		//
		// ���� ¥���� skinning �ڵ�� ��� ����ġ�� ���� transform�� �����ְ� �ֱ⿡ �� �ٸ� ȿ���� ���� ��
		// �̶�� ���������� normalize �ϴ� �κп��� ������ �ִ� bone�� ���� 4 �̻��� ��� ����� 4��
		// �����������ν� �ǹ� �ִٰ� ������
		void AddWeights(unsigned __int32 boneIndex, float weight)
		{
			// �̻��� ��
			if (weight <= 0.f)
				return;

			auto findIt = std::find_if(boneWeights.begin(), boneWeights.end(),
				[weight](const Pair& p) {return weight > p.second; });

			boneWeights.insert(findIt, Pair(boneIndex, weight));
		}

		asBlendWeight GetBlendWeights()
		{
			asBlendWeight blendWeights;

			for (unsigned __int32 i = 0; i < boneWeights.size(); ++i)
			{
				if (i >= 4)
					break;

				blendWeights.Set(i, boneWeights[i].first, boneWeights[i].second);
			}

			return blendWeights;
		}

		// �������� ����ġ�� �����ؼ� 1�� ����� ���� �Լ�
		// ex (1, 0.3) (2, 0.2) -> (1, 0.6) (2, 0.4)
		void Normalize()
		{
			// ������������ ȿ������ �ϵ������ ���ට����
			// �Ϲ������� ����ġ�� ���� �ִ� 4���� ���ѵȴ�.
			if (boneWeights.size() >= 4)
				boneWeights.resize(4);

			float totalWeight = 0.f;
			for (const auto& item : boneWeights)
				totalWeight += item.second;

			float scale = 1.f / totalWeight;
			for (auto& item : boneWeights)
				item.second *= scale;
		}

		using Pair = std::pair<unsigned __int32, float>;
		std::vector<Pair> boneWeights;
	};

	// Keyframe �� ����ִ� �������� bone transform ������
// Animation -> Keyframe -> KeyframeData�� �̷���� ������ 
// 3�� �迭�� �����ϸ� �ȴ�.
// Animation �� �迭�� ������ ���̰� Animation[0] �ȿ��� frame ������ �������� ����ְ�
// frame[0] ������ ������ �ӿ��� �������� bone transform �����Ͱ� ������� ��
	struct asKeyframeData
	{
		float time = 0.f;
		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;
	};

	// �ִϸ��̼��� ����ִ� Keyframe ����
	struct asKeyframeContainer
	{
		std::string boneName;
		std::vector<asKeyframeData> transforms;
	};

	// �ִϸ��̼� (idle, run, slash �� ���� �ϳ��� �ִϸ��̼��̴�.)
	struct asAnimation 
	{
		std::string name;
		unsigned int  frameCount = 0; // �� ������ ����
		float frameRate = 0.f; // �ʴ� ȭ�鿡 ��µǴ� ������ ���� (fps)
		float duration = 0.f;  // �ִϸ��̼��� �� �ð� (����) 
		//std::vector<std::unique_ptr<asKeyframe>> keyframes;
		std::vector<std::shared_ptr<asKeyframeContainer>>keyframes; //�����Ϳ��� �Ľ̿� ���� ������. �ܼ� ���ʹ� ������ �����̰� shared_ptr�� �츮�� ���� �ϴ� ����ε� �ϴ�
		std::unordered_map<std::string, std::shared_ptr<asKeyframeContainer>> mapKeyframes; //19���� ���µ� �������� ���⸦ ���ͷ� �޴´�. 
	}; 

	// Cache
	struct asAnimationNode
	{
		//aiString name;
		std::string name;
		std::vector<asKeyframeData> keyframe;
	};

	//�� �������� �ִϸ��̼� ����
	struct KeyframeDesc
	{
		__int32 animIndex = 0;
		unsigned __int32 currFrame = 0;
		unsigned __int32 nextFrame = 0;
		float ratio = 0.f;
		float sumTime = 0.f;
		float speed = 1.f;
		Vector2 padding;
	};

	//����
	struct TweenDesc
	{
		TweenDesc()
		{
			curr.animIndex = 0;
			next.animIndex = -1;
		}

		void ClearNextAnim()
		{
			next.animIndex = -1;
			next.currFrame = 0;
			next.nextFrame = 0;
			next.sumTime = 0;
			tweenSumTime = 0;
			tweenRatio = 0;
		}

		float tweenDuration = 1.0f;
		float tweenRatio = 0.f;
		float tweenSumTime = 0.f;
		float padding = 0.f;
		KeyframeDesc curr;
		KeyframeDesc next;
	};

	///

		///���ؽ��� ����ü
	class Vertex
	{
	public:
		Vertex() {}
		~Vertex() {}
	public:
		Vector3 position;							//������ 
		Vector3 normal;								//�븻 ����
		Vector2 texcoord;							//������ ��ǥ
		Vector4 blendIndices;			//�ε��� ������ ��ȣ �̰� ������4�� �ص� ��
		Vector4 blendWeights = {};					//���� �޴� ����ġ. �󸶳� ����Ǵ°�
		//Vector3 color;
	};

	// �޽� ������ : �⺻������ ���ؽ��� 
	// ��ġ�� �ε����� �����ϰ� �ִ�.
	class MeshData
	{
	public:
		MeshData() {}
		~MeshData() {}
	public:
		std::string name; //�޽��� �̸�
		__int32 boneIndex = 0; //���� �ε���
		std::vector<Vertex> vertices;				//Vertex����ü�� ����
		std::vector<uint32_t> indices;				//index������ �����ϴ� indices
		std::string materialName;					//mesh�� material�� �̸�
		std::string textureFilename;
		// ���߿� AABB���� �������ؾ��Ҽ�������.
	};
	
	class ModelData
	{
	public:
		ModelData() {}
		~ModelData() {}
	public:
		std::vector<MeshData> meshes;	//�𵨿� ���� �޽õ��� �迭
		std::vector<Bone> bones; // 
		std::vector<asAnimation> animations;	//�𵨿� ���� �޽õ��� �迭

		//std::vector<std::shared_ptr<NestGraphics::MeshData>> meshes;	//�𵨿� ���� �޽õ��� �迭
		//std::vector<std::shared_ptr<NestGraphics::Bone>> bones; // �𵨿� ���� ������ �迭//�ؽ����� �̸�
		//�ִ��� �迭

	};

	class Mesh
	{
	public:
		Mesh() {}
		~Mesh() {}
	public:
		ComPtr<ID3D11Buffer> m_vertexBuffer;		 // GPU�� vertex������ ������ ����, ���߿� �޸𸮴��� üũ�غ���.
		ComPtr<ID3D11Buffer> m_indexBuffer;			 // GPU�� vertex�� ���� ������ ����
		ComPtr<ID3D11Buffer> m_vertexConstantBuffer; // vertexShader�� �����ϴ� ���������
		ComPtr<ID3D11Buffer> m_pixelConstantBuffer;	 // pixelShader�� �����ϴ� ���������

		ComPtr<ID3D11Texture2D> texture;							//�ؽ���
		ComPtr<ID3D11ShaderResourceView> textureResourceView;		//���̴� ���ҽ� �� (���ҽ� �����͸� �д´�. >> ���� ���̴°� X)
		__int32 boneIndex = 0;						 //���� �ε���
		UINT m_indexCount;							 //�޽��� �� �ε��� ����
	};

		/// ����
	class Material
	{
	public:
		Material() {}
		~Material() {}
	public:
		Vector3 ambient = Vector3(0.1f);  // 12byte
		float shininess = 1.0f;           // 4byte	/����

		Vector3 diffuse = Vector3(0.5f);  // 12byte	/����
		float dummy1;                     // 4byte	/���� ������

		Vector3 specular = Vector3(0.5f); // 12byte
		float dummy2;                     // 4byte	/���� ������
	};
}