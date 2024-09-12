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
	//김예리나가 작성한 Bone 클래스
	///여기서부터 리나리나의 구조체
	class Bone
	{
	public:
		Bone() {};
		~Bone() {};

	public:
		std::string name; //이름
		__int32 index = -1; //현재 인덱스?
		__int32 parentIndex = -1; //부모의 인덱스
		Matrix localTransform; //최초의 부모기준 상대적인 행렬
		Matrix offsetTrm;
	};

	// Animation
	struct asBlendWeight
	{
		// 뼈의 목록은 4개
		// index : indices
		// 목록에서 몇 번째인지
		// boneIndex : 무슨 뼈를 가리키는지
		// weight : 가중치 (영향 받는 비율)
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

	// 정점마다 -> (관절번호, 가중치) 를 들고있는다.
// 이곳에서 작업을 끝내면 asBlendWeight struct로 넘겨준다.
	struct asBoneWeights
	{
		// 가중치가 높은 순서대로 boneWeights에 insert된다. 
		// ex : 3 7 1 5 -> 7 5 3 1
		//
		// 정점에 영향을 주는 bone이 많을 때, 해당 정점에
		// 가장 영향력 있는 bone을 가중치가 가장 높은 순서대로 적용함으로써
		// 정점의 움직임을 더 자연스럽게 보이게 할 수 있다.
		//
		// 현재 짜여진 skinning 코드는 모든 가중치에 대한 transform을 더해주고 있기에 별 다른 효과는 없을 듯
		// 이라고 생각했지만 normalize 하는 부분에서 영향을 주는 bone의 수가 4 이상일 경우 사이즈를 4로
		// 재조정함으로써 의미 있다고 생각됨
		void AddWeights(unsigned __int32 boneIndex, float weight)
		{
			// 이상한 값
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

		// 최종적인 가중치를 보정해서 1로 만들기 위한 함수
		// ex (1, 0.3) (2, 0.2) -> (1, 0.6) (2, 0.4)
		void Normalize()
		{
			// 파이프라인의 효율성과 하드웨어의 제약때문에
			// 일반적으로 가중치는 보통 최대 4개로 제한된다.
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

	// Keyframe 이 들고있는 실질적인 bone transform 데이터
// Animation -> Keyframe -> KeyframeData로 이루어져 있으며 
// 3중 배열로 생각하면 된다.
// Animation 은 배열로 관리될 것이고 Animation[0] 안에는 frame 단위의 정보들이 들어있고
// frame[0] 단위의 정보들 속에는 실질적인 bone transform 데이터가 들어있을 것
	struct asKeyframeData
	{
		float time = 0.f;
		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;
	};

	// 애니메이션이 들고있는 Keyframe 정보
	struct asKeyframeContainer
	{
		std::string boneName;
		std::vector<asKeyframeData> transforms;
	};

	// 애니메이션 (idle, run, slash 등 각각 하나의 애니메이션이다.)
	struct asAnimation 
	{
		std::string name;
		unsigned int  frameCount = 0; // 총 프레임 개수
		float frameRate = 0.f; // 초당 화면에 출력되는 프레임 개수 (fps)
		float duration = 0.f;  // 애니메이션의 총 시간 (길이) 
		//std::vector<std::unique_ptr<asKeyframe>> keyframes;
		std::vector<std::shared_ptr<asKeyframeContainer>>keyframes; //컨버터에서 파싱에 쓰고 버린다. 단순 벡터는 교수님 버전이고 shared_ptr은 우리가 원래 하던 방식인데 일단
		std::unordered_map<std::string, std::shared_ptr<asKeyframeContainer>> mapKeyframes; //19개가 들어가는데 교수님은 여기를 벡터로 받는다. 
	}; 

	// Cache
	struct asAnimationNode
	{
		//aiString name;
		std::string name;
		std::vector<asKeyframeData> keyframe;
	};

	//한 프레임의 애니메이션 정보
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

	//보간
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

		///버텍스의 구조체
	class Vertex
	{
	public:
		Vertex() {}
		~Vertex() {}
	public:
		Vector3 position;							//포지션 
		Vector3 normal;								//노말 벡터
		Vector2 texcoord;							//덱스쳐 좌표
		Vector4 blendIndices;			//인덱스 뼈대의 번호 이거 걍벡터4로 해도 됨
		Vector4 blendWeights = {};					//영향 받는 가중치. 얼마나 적용되는가
		//Vector3 color;
	};

	// 메쉬 데이터 : 기본적으로 버텍스의 
	// 위치와 인덱스를 저장하고 있다.
	class MeshData
	{
	public:
		MeshData() {}
		~MeshData() {}
	public:
		std::string name; //메쉬의 이름
		__int32 boneIndex = 0; //본의 인덱스
		std::vector<Vertex> vertices;				//Vertex구조체의 벡터
		std::vector<uint32_t> indices;				//index순서를 저장하는 indices
		std::string materialName;					//mesh의 material의 이름
		std::string textureFilename;
		// 나중에 AABB정보 값복사해야할수도있음.
	};
	
	class ModelData
	{
	public:
		ModelData() {}
		~ModelData() {}
	public:
		std::vector<MeshData> meshes;	//모델에 속한 메시들의 배열
		std::vector<Bone> bones; // 
		std::vector<asAnimation> animations;	//모델에 속한 메시들의 배열

		//std::vector<std::shared_ptr<NestGraphics::MeshData>> meshes;	//모델에 속한 메시들의 배열
		//std::vector<std::shared_ptr<NestGraphics::Bone>> bones; // 모델에 속한 본들의 배열//텍스쳐의 이름
		//애니의 배열

	};

	class Mesh
	{
	public:
		Mesh() {}
		~Mesh() {}
	public:
		ComPtr<ID3D11Buffer> m_vertexBuffer;		 // GPU에 vertex정보의 전달을 위해, 나중에 메모리누수 체크해보자.
		ComPtr<ID3D11Buffer> m_indexBuffer;			 // GPU에 vertex의 연결 정보를 전달
		ComPtr<ID3D11Buffer> m_vertexConstantBuffer; // vertexShader에 전달하는 상수데이터
		ComPtr<ID3D11Buffer> m_pixelConstantBuffer;	 // pixelShader에 전달하는 상수데이터

		ComPtr<ID3D11Texture2D> texture;							//텍스쳐
		ComPtr<ID3D11ShaderResourceView> textureResourceView;		//셰이더 리소스 뷰 (리소스 데이터를 읽는다. >> 아직 쓰이는곳 X)
		__int32 boneIndex = 0;						 //본의 인덱스
		UINT m_indexCount;							 //메쉬의 총 인덱스 개수
	};

		/// 재질
	class Material
	{
	public:
		Material() {}
		~Material() {}
	public:
		Vector3 ambient = Vector3(0.1f);  // 12byte
		float shininess = 1.0f;           // 4byte	/광택

		Vector3 diffuse = Vector3(0.5f);  // 12byte	/방향
		float dummy1;                     // 4byte	/더미 데이터

		Vector3 specular = Vector3(0.5f); // 12byte
		float dummy2;                     // 4byte	/더미 데이터
	};
}