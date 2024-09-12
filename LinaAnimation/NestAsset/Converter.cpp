#pragma once
#include <iostream>		//����� �ƴҰ�� ����
#include "Converter.h"

Converter::Converter()
{
	std::cout << "������ ����\n";
	m_pImporter = std::make_shared<Assimp::Importer>();
	std::cout << "Importer ����\n";
// 	m_pModelData = new NestGraphics::ModelData();
// 	std::cout << "ModelData ����\n";

}

Converter::~Converter()
{

}

/// <summary>
/// Asset������ �а�, �޸𸮿� �����Ѵ�.
/// </summary>
/// <param name="file"> : ���ϰ��+���ϸ�</param>
void Converter::ReadFile(std:: wstring Key, std::wstring file)
{
	std::wstring fileWStr = file;
	std::string fileStr(fileWStr.begin(), fileWStr.end());	//***wstring�� string���� ��ȯ
	m_pScene = m_pImporter->ReadFile
	(
		fileStr.c_str()								///�а��� �ϴ� ������ ���
		, aiProcess_ConvertToLeftHanded				// �޼� ��ǥ�踦 ����ϵ��� ���� ��ǥ�踦 ��ȯ
		| aiProcessPreset_TargetRealtime_MaxQuality	// �ǽð� �������� ���� ����ȭ ����
		| aiProcess_Triangulate						// ��� ���� �ﰢ������ ��ȯ
		| aiProcess_GenUVCoords						// �ؽ��� ������ ���� UV��ǥ ����
		| aiProcess_GenNormals						// ������ ������ ����
		| aiProcess_CalcTangentSpace				// ������ ������ ���� ������ ���	
		// | aiProcess_OptimizeMeshes				// �޽ø� ����ȭ�Ͽ� �ߺ��� ������ �����ϰ� �ε����� ����ȭ
		// | aiProcess_SplitLargeMeshes				// ũ�Ⱑ ū �޽ø� �����Ͽ� ������ ���� ���
		// | aiProcess_JoinIdenticalVertices		// �ߺ��� ������ ����
	);

	// TODO : assert ����� �����ڵ� �߰��ϱ�
	if (m_pScene == nullptr)
	{
		std::cout << " ���� - m_pScene�� nullptr�� ��ȯ�Ͽ���\n";
	}

	ReadModelData(m_pScene->mRootNode);
	//ReadMaterialData(m_pScene->mRootNode);
}



/// <summary>
/// �ӽ÷� ������ N���� MeshData�� �������ִ� ModelData�� ����� �Լ�
/// 2024.01.16 
/// </summary>
/// <param name="node"></param>
void Converter::ReadModelData(aiNode* node)
{
	// ���� ����� MeshData�� �����Ѵ�.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)				/** The number of meshes of this node. */
	{
		aiMesh* mesh = m_pScene->mMeshes[node->mMeshes[i]];
		m_pModelDataVec.meshes.push_back(ReadMeshData(mesh));
		///Modeldata�ȿ��� MeshData�� ���Ͱ� �����Ƿ�.. MeshData�� �־���Ѵ�.
	}

	// �ڽ� ��带 Ž���Ѵ�.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ReadModelData(node->mChildren[i]);
	}
}

/// <summary>
/// �ϴ� �ӽ÷� ������ Vertex, Index�� ������ �ִ� MeshData�� ��ȯ�ϴ� �Լ�
/// 2024.01.16 
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
NestGraphics::MeshData Converter::ReadMeshData(aiMesh* mesh)
{
	NestGraphics::MeshData* meshData = new NestGraphics::MeshData();

	// vertex�� meshData�� ����
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		NestGraphics::Vertex vertex;

		// position�� ����
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		// texcoord�� ����
		if (mesh->HasTextureCoords(0))
		{
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}

		// normal�� ����
		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		// tangent�� bitangent�� ����
		//if (mesh->HasTangentsAndBitangents())
		//{
		//	vertex.tangent.x = mesh->mTangents[i].x;
		//	vertex.tangent.y = mesh->mTangents[i].y;
		//	vertex.tangent.z = mesh->mTangents[i].z;

		//	vertex.bitangent.x = mesh->mBitangents[i].x;
		//	vertex.bitangent.y = mesh->mBitangents[i].y;
		//	vertex.bitangent.z = mesh->mBitangents[i].z;
		//}

		meshData->vertices.push_back(vertex);
	}

	// index�� meshData�� ����
	{
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				meshData->indices.push_back(face.mIndices[j]);
			}
		}
	}

	// mesh�� material�� �̸��� ��Ī
	aiMaterial* material = m_pScene->mMaterials[mesh->mMaterialIndex];
	meshData->materialName = material->GetName().C_Str();

	return *meshData;
}

/*
void Converter::ReadMaterialData(aiNode* node)
{
	for (unsigned int i = 0; i < m_pScene->mNumMaterials; i++)
	{
		aiMaterial* material = m_pScene->mMaterials[i];

		Nest::Material materialData;
		materialData.name = material->GetName().C_Str();

		aiColor3D color;

		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		materialData.ambient = Vector3(color.r, color.g, color.b);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		materialData.diffuse = Vector3(color.r, color.g, color.b);

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		materialData.specular = Vector3(color.r, color.g, color.b);

		material->Get(AI_MATKEY_SHININESS, materialData.shininess);

		m_pModelData->materials.push_back(materialData);

	}
}
*/

void Converter::ExportData(std::wstring file)
{

}

void Converter::ExportMeshData(std::wstring file)
{

}

void Converter::ExportModelData(std::wstring file)
{

}

void Converter::ExportMaterialData(std::wstring file)
{

}

void Converter::ExportAnimationData(std::wstring file)
{

}

void Converter::ReadAnimationData()
{

}
