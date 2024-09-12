#pragma once
/// <summary>
/// 리소스 매니저에서 관리할수 있도록한다.
/// 
/// 일단 돌아가는 예시 코드)
/// Converter* converter = new Converter();      // Converter객체를 생성
/// ReadFile(../Resources/Asset/Test.fbx);      // 파일의 경로와 파일명을 인자로 입력
/// m_pModelData = GetModelData();            // MeshData 배열과 Material배열을 가지고있는 ModelData를 반환하는 함수
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
	//Assimp 라이브러리의 'Import' 클래스를 가리키는 스마트 포인터, 이를 사용해서 3D모텔파일을 Import하고 처리한다.
	std::shared_ptr<Assimp::Importer> m_pImporter;

	//Assimp 라이브러리에서 Import 된 3D 모델의 정보를 가리키는 포인터.
	const aiScene* m_pScene;

	//키값에 그에 따른 ModelData의 값을 가지고 있는다.
	std::unordered_map<std::wstring, NestGraphics::ModelData*> m_UnMapModelData;

	//std::vector<MeshData> meshes을 가지는 클래스
	NestGraphics::ModelData* m_pModelData;

	//이건 일단 나중에 Material을 어떻게 할것인지 생각하고 짜자.
	//std::vector<NestGraphics::Material> materials;

	   /// <summary>
 /// 여기서부터 김예리나가 추가한 변수들입니다. 애니메이션을 위하여!
 /// 2024.2.1일 
 /// </summary>
	//메쉬의 본
	std::vector< std::shared_ptr<NestGraphics::Bone>> m_bones;
	//하나의 메쉬에 있는..여러개의 메쉬?
	std::vector<std::shared_ptr<NestGraphics::MeshData>> m_meshData;
	std::vector<NestGraphics::asAnimation> m_Animations;
	//강의에는 메테리얼도 있었는데 여기는 안넣을거다. 아직 상의도 안됬고..
	//std::vector<std::shared_ptr<asMaterial>> m_Materials;

public:
	NestGraphics::ModelData* ReadFile(std::wstring key, std::wstring file);

	// 이걸 살릴까 말까 생각중 -DJ
	// void ExportData(std::wstring file);

	//굳이 필요한가..? 아닌거 같은데..?	필요합니다
	//NestGraphics::ModelData* GetModelData() { return m_pModelData; }
	//NestGraphics::ModelData* GetModelData(std::wstring key);

private:
	NestGraphics::MeshData ReadMeshData(aiMesh* mesh);
	void ReadModelData(aiNode* node);

	//김예리나가 작성한 함수
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

	//아직 미구현..
	void ReadMaterialData(aiNode* node) {}
	void ReadAnimationData() {}
	void ExportMeshData(std::wstring file) {}
	void ExportModelData(std::wstring file) {}
	void ExportMaterialData(std::wstring file) {}
	void ExportAnimationData(std::wstring file) {}

};
