//#pragma once
//#include "framework.h"
//
///// <summary>
///// �� ���Ͽ��� ���� �� �ִ� �������� ����ü��
///// �׷��Ƚ��� Ŭ���̾�Ʈ�� ����ü�� ��ġ���Ѿ��Ѵ�.
///// Ŭ���� ��ȣ���� �����ϱ����� ������ �����������ϴ�.
///// </summary>
//namespace FileData
//{
//	struct Vertex
//	{
//		Vector3 position;	// ���� ��ǥ
//		Vector3 normal;		// ���� ����
//		Vector2 texCoord;	// �ؽ��� ��ǥ
//		//Vector3 tangent;	// ���� ����
//		//Vector3 bitangent;	// 
//		// ...
//	};
//
//	struct Material
//	{
//		std::string name;
//		Vector3 ambient;	// �ֺ��� ����
//		Vector3 diffuse;	// Ȯ�걤 ����
//		Vector3 specular;	// �ݻ籤 ����
//		float shininess;	// ǥ�� ��¦�� ����
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
//		std::vector<FileData::Vertex> vertices;	// ���� �迭
//		std::vector<uint16_t> indices;	// ���� �ε��� �迭
//		std::string materialName;		// mesh�� material�� �̸�
//		std::string textureFilename; //�ؽ�ó�� ������ �������� �ʱ� ���� �÷���
//	};
//
//	struct ModelData
//	{
//		std::vector<FileData::MeshData> meshes;	// �𵨿� ���� �޽õ��� �迭
//		std::vector<FileData::Material> materials;	// ���� ����
//	};
//
//	struct Keyframe
//	{
//		unsigned int frame;
//		float time;	//�ð�
//		Vector3 translation;	// �̵� ��ȯ
//		Vector4 rotation;	// ȸ�� ��ȯ (Quaternion)
//		Vector3 scale;	// ũ�� ��ȯ
//	};
//
//	struct AnimationChannel
//	{
//		std::vector<FileData::Keyframe> keyframes; // Ű������ �迭
//	};
//
//	struct AnimationData
//	{
//		float duration;	// �ִϸ��̼� �� �ð�
//		std::unordered_map<std::wstring, AnimationChannel> channels; // �ִϸ��̼� ä�� ��
//	};
//
//	struct Node
//	{
//		std::wstring name;	// ��� �̸�
//		Matrix transform;	// ��ȯ ���
//		std::vector<Node> children;	// �ڽ� ��� �迭
//	};
//
//	struct SceneData
//	{
//		Node rootNode;	// ��Ʈ ���
//		std::vector<FileData::ModelData> models;	// �� ���� �迭
//		std::vector<FileData::AnimationData> animations;	// �ִϸ��̼� ����
//	};
//}
