#pragma once
/// <summary>
/// ���ҽ� �Ŵ������� �����Ҽ� �ֵ����Ѵ�.
/// 
/// �ϴ� ���ư��� ���� �ڵ�)
/// Converter* converter = new Converter();      // Converter��ü�� ����
/// ReadFile(../Resources/Asset/Test.fbx);      // ������ ��ο� ���ϸ��� ���ڷ� �Է�
/// m_pModelData = GetModelData();            // MeshData �迭�� Material�迭�� �������ִ� ModelData�� ��ȯ�ϴ� �Լ�
///  
/// </summary>
#include <memory>
#include <string>
#include <DirectXMath.h>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/StringUtils.h"
#include "directxtk/SimpleMath.h"
#include "Geometry.h"
#include <map>

using namespace DirectX::SimpleMath;

class Converter
{
public:
	Converter();
	~Converter();
private:
	//Assimp ���̺귯���� 'Import' Ŭ������ ����Ű�� ����Ʈ ������, �̸� ����ؼ� 3D���������� Import�ϰ� ó���Ѵ�.
	std::shared_ptr<Assimp::Importer> m_pImporter;

	//Assimp ���̺귯������ Import �� 3D ���� ������ ����Ű�� ������.
	const aiScene* m_pScene;

	//Ű���� �׿� ���� ModelData�� ���� ������ �ִ´�.
	std::unordered_map<std::wstring, NestGraphics::ModelData*> m_UnMapModelData;

	//std::vector<MeshData> meshes�� ������ Ŭ����
	NestGraphics::ModelData* m_pModelData;

	//�̰� �ϴ� ���߿� Material�� ��� �Ұ����� �����ϰ� ¥��.
	//std::vector<NestGraphics::Material> materials;

	   /// <summary>
 /// ���⼭���� �迹������ �߰��� �������Դϴ�. �ִϸ��̼��� ���Ͽ�!
 /// 2024.2.1�� 
 /// </summary>
	//�޽��� ��
	std::vector< std::shared_ptr<NestGraphics::Bone>> m_bones;
	//�ϳ��� �޽��� �ִ�..�������� �޽�?
	std::vector<std::shared_ptr<NestGraphics::MeshData>> m_meshData;
	std::vector<NestGraphics::asAnimation> m_Animations;
	//���ǿ��� ���׸��� �־��µ� ����� �ȳ����Ŵ�. ���� ���ǵ� �ȉ��..
	//std::vector<std::shared_ptr<asMaterial>> m_Materials;

public:
	NestGraphics::ModelData* ReadFile(std::wstring key, std::wstring file);

	// �̰� �츱�� ���� ������ -DJ
	// void ExportData(std::wstring file);

	//���� �ʿ��Ѱ�..? �ƴѰ� ������..?	�ʿ��մϴ�
	//NestGraphics::ModelData* GetModelData() { return m_pModelData; }
	//NestGraphics::ModelData* GetModelData(std::wstring key);

private:
	NestGraphics::MeshData ReadMeshData(aiMesh* mesh);
	void ReadModelData(aiNode* node);

	//�迹������ �ۼ��� �Լ�
	void ReadModelDataTwo(aiNode* node, __int32 index, __int32 parentNode);
	void ReadMeshDataTwo(aiNode* node, __int32 bone);
	void ConnectMesh();
	void ReadSkinData();
	unsigned __int32 GetBoneIndex(const std::string& _name);
	std::shared_ptr<NestGraphics::asAnimation> ReadAnimationData(aiAnimation* srcAnimation);
	std::shared_ptr<NestGraphics::asAnimationNode> ParseAnimationNode(std::shared_ptr<NestGraphics::asAnimation> animation, aiNodeAnim* srcNode);
	void ReadKeyframeData(std::shared_ptr<NestGraphics::asAnimation> animation, aiNode* srcNode, std::map<std::string, std::shared_ptr<NestGraphics::asAnimationNode>>& cache);
	std::shared_ptr<NestGraphics::asAnimation>  AnimatedRead(const aiAnimation* srcAnimation);
	//Matrix ComputeWorldTransform(const aiNode* node, const Matrix& parentWorldMatrix = Matrix::Identity);
	//NestGraphics::Bone* ReadBone(const aiScene* pScene, const aiNode* pNode);

	//���� �̱���..
	void ReadMaterialData(aiNode* node) {}
	void ReadAnimationData() {}
	void ExportMeshData(std::wstring file) {}
	void ExportModelData(std::wstring file) {}
	void ExportMaterialData(std::wstring file) {}
	void ExportAnimationData(std::wstring file) {}

};
