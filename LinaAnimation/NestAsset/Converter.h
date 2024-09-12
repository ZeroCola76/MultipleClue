#pragma once
/// <summary>
/// Asset�� �޸𸮿� Import�ϰ�, Model, Material, Animation ���� ������ 
/// ���̳ʸ� ������ Ŀ���� ���� �������� ����� Ŭ�����Դϴ�.
/// ������� ����? ������?�� ���ҽ� �Ŵ������ �����ϵ��� �Ұ�?
/// 
/// 2024.01.16 �ӽ÷� vertex, index �������� �̾Ƴ� �� �ֵ��� ���� 
/// **���� )
/// Converter* converter = new Converter();		// Converter��ü�� ����
/// ReadFile(../Resources/Asset/Test.fbx);		// ������ ��ο� ���ϸ��� ���ڷ� �Է�
/// m_pModelData = GetModelData();				// MeshData �迭�� Material�迭�� �������ִ� ModelData�� ��ȯ�ϴ� �Լ�
///  
/// </summary>
#include "framework.h"
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

	std::vector<NestGraphics::ModelData*> m_pModelDataVec;
	//std::vector<NestGraphics::Material> materials; �ٳ����� �ּ������ϰ� �������

	std::unordered_map <std::string, NestGraphics::ModelData> m_ModelDataUnMap;

public:
	void ReadFile(std::wstring file);
	void ExportData(std::wstring file);

	NestGraphics::ModelData* GetModelData(int index) { return m_pModelDataVec[index]; }

private:
	NestGraphics::MeshData ReadMeshData(aiMesh* mesh);
	void ReadModelData(aiNode* node);
	void ReadMaterialData(aiNode* node) {}
	void ReadAnimationData();

	void ExportMeshData(std::wstring file);
	void ExportModelData(std::wstring file);
	void ExportMaterialData(std::wstring file);
	void ExportAnimationData(std::wstring file);

};

