#pragma once
#include "framework.h"
//#include "FileData.h"
//using namespace FileData;
/// <summary>
/// Asset을 메모리에 Import하고, Model, Material, Animation 등의 정보를 
/// 바이너리 형태의 커스텀 파일 포맷으로 만드는 클래스입니다.
/// 만들어진 파일? 데이터?는 리소스 매니저등에서 관리하도록 할것?
/// 
/// 2024.01.16 
/// 임시로 vertex, index 정보만을 뽑아낼 수 있도록 만듬 
/// **예시 )
/// Converter* converter = new Converter();		// Converter객체를 생성
/// ReadFile(../Resources/Asset/Test.fbx);		// 파일의 경로와 파일명을 인자로 입력
/// m_pModelData = GetModelData();				// MeshData 배열과 Material배열을 가지고있는 ModelData를 반환하는 함수
/// 
/// 2024.01.25
/// CHANGE_Doyo : N개의 모델 데이터를 가지고 있도록 변경, MeshData구조체에 직접 Material을 집어넣도록 변경
/// Converter* converter = new Converter();		// Converter객체를 생성
/// ReadFile(../Resources/Asset/Test.fbx);		// 파일의 경로와 파일명을 인자로 입력
/// m_pModelData = GetModelData(index);			// MeshData 배열을 가지고있는 index번째의 ModelData를 반환하는 함수
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

