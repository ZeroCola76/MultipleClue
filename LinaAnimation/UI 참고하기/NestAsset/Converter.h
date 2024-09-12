#pragma once
#include "framework.h"
//#include "FileData.h"
//using namespace FileData;
/// <summary>
/// Asset�� �޸𸮿� Import�ϰ�, Model, Material, Animation ���� ������ 
/// ���̳ʸ� ������ Ŀ���� ���� �������� ����� Ŭ�����Դϴ�.
/// ������� ����? ������?�� ���ҽ� �Ŵ������ �����ϵ��� �Ұ�?
/// 
/// 2024.01.16 
/// �ӽ÷� vertex, index �������� �̾Ƴ� �� �ֵ��� ���� 
/// **���� )
/// Converter* converter = new Converter();		// Converter��ü�� ����
/// ReadFile(../Resources/Asset/Test.fbx);		// ������ ��ο� ���ϸ��� ���ڷ� �Է�
/// m_pModelData = GetModelData();				// MeshData �迭�� Material�迭�� �������ִ� ModelData�� ��ȯ�ϴ� �Լ�
/// 
/// 2024.01.25
/// CHANGE_Doyo : N���� �� �����͸� ������ �ֵ��� ����, MeshData����ü�� ���� Material�� ����ֵ��� ����
/// Converter* converter = new Converter();		// Converter��ü�� ����
/// ReadFile(../Resources/Asset/Test.fbx);		// ������ ��ο� ���ϸ��� ���ڷ� �Է�
/// m_pModelData = GetModelData(index);			// MeshData �迭�� �������ִ� index��°�� ModelData�� ��ȯ�ϴ� �Լ�
/// 
/// </summary>
class Converter
{
public:
	Converter();
	~Converter();

	void ReadFile(std::wstring file);
	void ExportData(std::wstring file);

	//NestGraphics::ModelData* GetModelData(int index) { return m_pModelData[index]; }
	NestGraphics::ModelData* GetModelData() { return m_pModelData; }

private:
	void ExportMeshData(std::wstring file);
	void ExportModelData(std::wstring file);
	void ExportMaterialData(std::wstring file);
	void ExportAnimationData(std::wstring file);

	NestGraphics::MeshData ReadMeshData(aiMesh* mesh);
	void ReadModelData(aiNode* node);
	void ReadModelData(aiNode* node, NestGraphics::ModelData* model);
	void ReadMaterialData(aiNode* node, NestGraphics::MeshData& pMesh) {};
	void ReadAnimationData();

private:
	std::shared_ptr<Assimp::Importer> m_pImporter;
	const aiScene* m_pScene;

	//std::vector<NestGraphics::ModelData*> m_pModelData;
	NestGraphics::ModelData* m_pModelData;
};

