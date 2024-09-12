#pragma once
/// <summary>
/// Asset을 메모리에 Import하고, Model, Material, Animation 등의 정보를 
/// 바이너리 형태의 커스텀 파일 포맷으로 만드는 클래스입니다.
/// 만들어진 파일? 데이터?는 리소스 매니저등에서 관리하도록 할것?
/// 
/// 2024.01.16 임시로 vertex, index 정보만을 뽑아낼 수 있도록 만듬 
/// **예시 )
/// Converter* converter = new Converter();		// Converter객체를 생성
/// ReadFile(../Resources/Asset/Test.fbx);		// 파일의 경로와 파일명을 인자로 입력
/// m_pModelData = GetModelData();				// MeshData 배열과 Material배열을 가지고있는 ModelData를 반환하는 함수
///  
/// </summary>
#include "framework.h"
class Converter
{
public:
	Converter();
	~Converter();
private:
	//Assimp 라이브러리의 'Import' 클래스를 가리키는 스마트 포인터, 이를 사용해서 3D모텔파일을 Import하고 처리한다.
	std::shared_ptr<Assimp::Importer> m_pImporter;	

	//Assimp 라이브러리에서 Import 된 3D 모델의 정보를 가리키는 포인터.
	const aiScene* m_pScene;

	std::vector<NestGraphics::ModelData*> m_pModelDataVec;
	//std::vector<NestGraphics::Material> materials; 다끝나고 주석해제하고 오류잡아

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

