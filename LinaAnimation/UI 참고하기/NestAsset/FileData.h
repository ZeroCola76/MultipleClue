//#pragma once
//#include "framework.h"
//
///// <summary>
///// 모델 파일에서 얻을 수 있는 정보들의 구조체들
///// 그래픽스나 클라이언트의 구조체와 일치시켜야한다.
///// 클래스 모호성을 방지하기위해 범위를 한정지었습니다.
///// </summary>
//namespace FileData
//{
//	struct Vertex
//	{
//		Vector3 position;	// 정점 좌표
//		Vector3 normal;		// 법선 벡터
//		Vector2 texCoord;	// 텍스쳐 좌표
//		//Vector3 tangent;	// 접선 벡터
//		//Vector3 bitangent;	// 
//		// ...
//	};
//
//	struct Material
//	{
//		std::string name;
//		Vector3 ambient;	// 주변광 색상
//		Vector3 diffuse;	// 확산광 색상
//		Vector3 specular;	// 반사광 색상
//		float shininess;	// 표면 반짝임 정도
//	};
//
//	struct Texture
//	{
//		std::string type;
//		std::string path;
//	};
//
//	struct MeshData
//	{
//		std::vector<FileData::Vertex> vertices;	// 정점 배열
//		std::vector<uint16_t> indices;	// 정점 인덱스 배열
//		std::string materialName;		// mesh의 material의 이름
//		std::string textureFilename; //텍스처가 없으면 적용하지 않기 위한 플래그
//	};
//
//	struct ModelData
//	{
//		std::vector<FileData::MeshData> meshes;	// 모델에 속한 메시들의 배열
//		std::vector<FileData::Material> materials;	// 재질 정보
//	};
//
//	struct Keyframe
//	{
//		unsigned int frame;
//		float time;	//시간
//		Vector3 translation;	// 이동 변환
//		Vector4 rotation;	// 회전 변환 (Quaternion)
//		Vector3 scale;	// 크기 변환
//	};
//
//	struct AnimationChannel
//	{
//		std::vector<FileData::Keyframe> keyframes; // 키프레임 배열
//	};
//
//	struct AnimationData
//	{
//		float duration;	// 애니메이션 총 시간
//		std::unordered_map<std::wstring, AnimationChannel> channels; // 애니메이션 채널 맵
//	};
//
//	struct Node
//	{
//		std::wstring name;	// 노드 이름
//		Matrix transform;	// 변환 행렬
//		std::vector<Node> children;	// 자식 노드 배열
//	};
//
//	struct SceneData
//	{
//		Node rootNode;	// 루트 노드
//		std::vector<FileData::ModelData> models;	// 모델 정보 배열
//		std::vector<FileData::AnimationData> animations;	// 애니메이션 정보
//	};
//}
