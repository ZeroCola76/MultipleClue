#pragma once
#include <iostream>		//디버그 아닐경우 떼기
#include "Converter.h"

Converter::Converter()
{
	std::cout << "컨버터 생성\n";
	m_pImporter = std::make_shared<Assimp::Importer>();
	std::cout << "Importer 생성\n";

}

Converter::~Converter()
{
	//for (int i = 0; i < m_pModelData.size(); i++)
	//{
	//	delete m_pModelData[i];
	//}
}

/// <summary>
/// Asset파일을 읽고, 메모리에 저장한다.
/// TODO_Doyo : 파일경로는 후에 고정시키고 파일명만 인자로 받도록 수정할것
/// </summary>
/// <param name="file"> : 파일경로+파일명</param>
void Converter::ReadFile(std::wstring file)
{
	NestGraphics::ModelData* pModelData = new NestGraphics::ModelData();
	std::cout << "ModelData 생성\n";

	std::wstring fileWStr = /*L"../Resource/Assets/" +*/ file;
	std::string fileStr(fileWStr.begin(), fileWStr.end());
	m_pScene = m_pImporter->ReadFile
	(
		fileStr.c_str()
		, aiProcess_ConvertToLeftHanded				// 왼손 좌표계를 사용하도록 모델의 좌표계를 변환
		| aiProcessPreset_TargetRealtime_MaxQuality	// 실시간 렌더링을 위한 최적화 수행
		| aiProcess_Triangulate						// 모든 면을 삼각형으로 변환
		| aiProcess_GenUVCoords						// 텍스쳐 매핑을 위한 UV좌표 생성
		| aiProcess_GenNormals						// 정점의 법선을 생성
		// | aiProcess_CalcTangentSpace				// 정점의 접선과 접선 공간을 계산	
		| aiProcess_OptimizeMeshes					// 메시를 최적화하여 중복된 정점을 제거하고 인덱스를 최적화
		// | aiProcess_SplitLargeMeshes				// 크기가 큰 메시를 분할하여 렌더링 성능 향상
		| aiProcess_JoinIdenticalVertices			// 중복된 정점을 병합
	);

	// TODO : assert 대신할 에러코드 추가하기
	assert(m_pScene && "ReadFile() Failed.");

	ReadModelData(m_pScene->mRootNode);
	//ReadModelData(m_pScene->mRootNode, pModelData);
	//m_pModelData.push_back(pModelData);
}

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

/// <summary>
/// 일단 임시로 만들어둔 Vertex, Index를 가지고 있는 MeshData를 반환하는 함수
/// 2024.01.25
/// Material도 가지고 있도록 변경
/// </summary>
/// <param name="mesh">현재 Mesh</param>
/// <returns></returns>
NestGraphics::MeshData Converter::ReadMeshData(aiMesh* mesh)
{
	NestGraphics::MeshData meshData;

	// vertex를 meshData에 저장
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		NestGraphics::Vertex vertex;

		// position값 저장
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		// texcoord값 저장
		if (mesh->HasTextureCoords(0))
		{
			vertex.texcoord.x = mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = mesh->mTextureCoords[0][i].y;
		}

		// normal값 저장
		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		// tangent와 bitangent값 저장
		//if (mesh->HasTangentsAndBitangents())
		//{
		//	vertex.tangent.x = mesh->mTangents[i].x;
		//	vertex.tangent.y = mesh->mTangents[i].y;
		//	vertex.tangent.z = mesh->mTangents[i].z;

		//	vertex.bitangent.x = mesh->mBitangents[i].x;
		//	vertex.bitangent.y = mesh->mBitangents[i].y;
		//	vertex.bitangent.z = mesh->mBitangents[i].z;
		//}

		meshData.vertices.push_back(vertex);
	}

	// index를 meshData에 저장
	{
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				meshData.indices.push_back(face.mIndices[j]);
			}
		}
	}

	// mesh의 material의 이름을 매칭
	// ReadMaterialData(m_pScene->mRootNode, meshData);

	return meshData;
}

/// <summary>
/// 임시로 만들어둔 N개의 MeshData를 가지고있는 ModelData를 만드는 함수
/// 2024.01.16 
/// </summary>
/// <param name="node"></param>
void Converter::ReadModelData(aiNode* node)
{
	// 현재 노드의 MeshData를 저장한다.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = m_pScene->mMeshes[node->mMeshes[i]];
		m_pModelData->meshes.push_back(ReadMeshData(mesh));
	}

	// 자식 노드를 탐색한다.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ReadModelData(node->mChildren[i]);
	}
}


/// <summary>
/// 임시로 만들어둔 N개의 MeshData를 가지고있는 ModelData를 만드는 함수
/// 2024.01.16 
/// </summary>
/// <param name="node">현재 Node</param>
/// <param name="model">현재 Model</param>
void Converter::ReadModelData(aiNode* node, NestGraphics::ModelData* model)
{
	// 현재 노드의 MeshData를 저장한다.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = m_pScene->mMeshes[node->mMeshes[i]];
		model->meshes.push_back(ReadMeshData(mesh));
	}

	// 자식 노드를 탐색한다.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ReadModelData(node->mChildren[i], model);
	}
}

/// <summary>
/// MeshData에 Material구조체를 넣는 함수
/// </summary>
/// <param name="node">현재 Node</param>
/// <param name="mesh">현재 Mesh</param>
/*
void Converter::ReadMaterialData(aiNode* node, NestGraphics::MeshData& mesh)
{
	for (unsigned int i = 0; i < m_pScene->mNumMaterials; i++)
	{
		aiMaterial* material = m_pScene->mMaterials[i];

		NestGraphics::Material materialData;

		aiColor3D color;

		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		materialData.ambient = Vector3(color.r, color.g, color.b);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		materialData.diffuse = Vector3(color.r, color.g, color.b);

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		materialData.specular = Vector3(color.r, color.g, color.b);

		material->Get(AI_MATKEY_SHININESS, materialData.shininess);

		mesh.material = materialData;
	}
}
*/

void Converter::ReadAnimationData()
{

}
