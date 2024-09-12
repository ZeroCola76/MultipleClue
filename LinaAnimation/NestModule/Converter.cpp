#pragma once
#include <iostream>      //디버그 아닐경우 떼기
#include "Converter.h"

Converter::Converter()
{
	std::cout << "컨버터 생성\n";
	m_pImporter = std::make_shared<Assimp::Importer>();
	std::cout << "Importer 생성\n";

	std::cout << "ModelData 생성\n";

}

Converter::~Converter()
{

}

/// Asset파일을 읽고, 메모리에 저장한다.
/// 지금은 FBX한정으로 쓰인다. 걱정이다. - DJ
NestGraphics::ModelData* Converter::ReadFile(std::wstring _key, std::wstring file)
{
	std::unordered_map<std::wstring, NestGraphics::ModelData*>::const_iterator it = m_UnMapModelData.find(_key);
	if (it != m_UnMapModelData.end())
	{
		return it->second;
	}
	else
	{
		std::wstring fileWStr = file;
		std::string fileStr(fileWStr.begin(), fileWStr.end());   //***wstring을 string으로 변환
		m_pScene = m_pImporter->ReadFile
		(
			fileStr.c_str()
			//, aiProcessPreset_TargetRealtime_MaxQuality|   // 실시간 렌더링을 위한 최적화 수행
			, aiProcess_Triangulate | // 삼각형으로 변환
			aiProcess_GenNormals |	// 노말 생성/
			aiProcess_GenUVCoords |		// UV 생성
			aiProcess_CalcTangentSpace |  // 탄젠트 생성
			aiProcess_LimitBoneWeights |	// 본의 영향을 받는 정점의 최대 개수를 4개로 제한
			aiProcess_ConvertToLeftHanded        // 왼손 좌표계를 사용하도록 모델의 좌표계를 변환
			
												 // | aiProcess_CalcTangentSpace            // 정점의 접선과 접선 공간을 계산   
			// | aiProcess_OptimizeMeshes            // 메시를 최적화하여 중복된 정점을 제거하고 인덱스를 최적화
			// | aiProcess_SplitLargeMeshes            // 크기가 큰 메시를 분할하여 렌더링 성능 향상
			// | aiProcess_JoinIdenticalVertices      // 중복된 정점을 병합
		);

		// TODO : ReadFile과 분리할것, ReadFile은 void반환한다.
		//        NestGraphics::ModelData* GetModel(
		if (m_pScene != nullptr)
		{
			m_pModelData = new NestGraphics::ModelData();
			ReadModelDataTwo(m_pScene->mRootNode, 0, -1); //루트 노드는 idnetity 다 어쩄든 -1으로 하면 안되는 이유는 루트 노드에 접근하면 막힌다. 
			//m_UnMapModelData.insert({ _key, m_pModelData });
			ReadSkinData();
			ConnectMesh();
			return m_pModelData;
		}
		else
		{
			std::cout << " 오류 - m_pScene이 nullptr을 반환하였음\n";
			return nullptr;
		}
	}
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
/// 리나가 만드는 중인 Model를 읽는 함수
/// </summary>
/// <param name="node"></param>
/// <param name="index"></param>
/// <param name="parentNode"></param>
void Converter::ReadModelDataTwo(aiNode* node, __int32 index, __int32 parentIndex)
{
	std::shared_ptr<NestGraphics::Bone> bone = std::make_shared<NestGraphics::Bone>();
	//부모인덱스는 -1일거다. 그리고 자식으로 가면 갈수록 1씩 증가한다.
	bone->index = index;
	bone->name = node->mName.C_Str();
	bone->parentIndex = parentIndex;

	Matrix transform(node->mTransformation[0]);
	bone->localTransform = transform.Transpose(); //assimp와 DX의 행렬저장쳬계가 다르다 행-열이 다르므로 전치행렬을 구한다. 

	m_bones.push_back(bone);

	ReadMeshDataTwo(node, index);

	//여기는 자식 계층을 타는거랑 똑같은데, 본 연결을 위해 함께 받는거로 수정
	for (__int32 i = 0; i < node->mNumChildren; i++)
	{
		ReadModelDataTwo(node->mChildren[i], static_cast<unsigned>(m_bones.size()), index);
	}


}

/// <summary>
/// 모델 안의 메쉬 여러개를 읽는 함수. 
/// </summary>
/// <param name="node"></param>
/// <param name="bone"></param>
void Converter::ReadMeshDataTwo(aiNode* node, __int32 bone)
{
	//메쉬수가 1 미만이라면, 당연히 메쉬가 없다는거다.
	if (node->mNumMeshes < 1)
	{
		std::cout << "메쉬수가 0개인 모델을 읽었습니다.. - 김예리나" << std::endl;
		return;
	}

	//여기가 진짜 파싱하는 부분 흐름별로 가면서 본을 끼운다.
	std::shared_ptr<NestGraphics::MeshData> mesh = std::make_shared<NestGraphics::MeshData>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (__int32 i = 0; i < node->mNumMeshes; ++i)
	{
		//이 인덱스 번호가 실제로 신의 메쉬 번호가 된다..그걸로 씬에
		// 있는 메쉬에 접근한다. 
		//추출을 한다고 하는데 제대로 이해못함. TO DO: 뒤돌아보기
		__int32 index = node->mMeshes[i];
		const aiMesh* srcMesh = m_pScene->mMeshes[index];
		//지금까지 정점 번호 몇번이였는지  
		const __int32 startVec = mesh->vertices.size();

		//강의에는 여기에 머테리얼 파싱도 있었다. 하지만 우리는 필요없을듯
		//버텍스 번호를 하나씩 스퀘어하면서 버텍스랑 uv좌표랑 노말 좌표에 넣었다.
		//딴것도 더 넣어야한다...아무튼 버텍스 버퍼 채우는 부분
		for (__int32 v = 0; v < srcMesh->mNumVertices; ++v)
		{
			// Vertex
			NestGraphics::Vertex vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vector3));

			// UV
			if (srcMesh->HasTextureCoords(0))
			{
				// &srcMesh->mTextureCoords[0][v] 첫 번째 좌표 세트에 v번째 버텍스가 갖고 있는
				// uv 구조체 주소를 나타낸다. 
				::memcpy(&vertex.texcoord, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			}

			// Normal
			if (srcMesh->HasNormals())
			{
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vector3));
			}


			mesh->vertices.push_back(vertex);
		}

		//여기가 인덱스 버퍼를 채우는 부분이 된다. 이것은 기석님 코드를 배꼈다.
		for (unsigned int f = 0; f < srcMesh->mNumFaces; ++f)
		{
			//aiface는 인덱스의 갯수..라고 한다
			aiFace& face = srcMesh->mFaces[f];
			for (__int32 k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.push_back(face.mIndices[k] + startVec);
			}

		}
		m_meshData.push_back(mesh); //이거 밖에 있으면 안된다

	}

}
/// <summary>
/// 그래픽스 구조체 와 연결하는 부분
/// </summary>
/// <param name="node"></param>
void Converter::ConnectMesh()
{
	for (const auto& meshPtr : m_meshData)
	{
		if (meshPtr)
		{
			m_pModelData->meshes.push_back(*meshPtr);
		}
	}

	for (const auto& bonePtr : m_bones)
	{
		if (bonePtr)
		{
			m_pModelData->bones.push_back(*bonePtr);
		}
	}

	///애니메이션 연결?
	if (m_pScene->HasAnimations()) {
		for (unsigned int i = 0; i < m_pScene->mNumAnimations; i++) {
			 aiAnimation* animation = m_pScene->mAnimations[0];

			assert(animation->mNumChannels > 1);
			std::shared_ptr<NestGraphics::asAnimation> animationData = ReadAnimationData(animation);
			m_pModelData->animations.push_back(*animationData);
		}
	}

	//ReadBone(m_pScene, m_pScene->mRootNode);
	//m_pModelData->bones = m_bones;
}

/// <summary>
/// 본의 가중치를 구하는 함수
/// 정점마다 어떤 뼈에 영향을 받을건가?
/// </summary>
void Converter::ReadSkinData()
{
	for (unsigned __int32 i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiMesh* srcMesh = m_pScene->mMeshes[i];

		//본이 없다면..?왜 굳이 검사하는가. 
		if (!srcMesh->HasBones())
			continue;

		std::shared_ptr<NestGraphics::MeshData> mesh = std::make_shared<NestGraphics::MeshData>();
		mesh = m_meshData[i];

		// 본 번호와 가중치를 임시 저장할 벡터
		std::vector<NestGraphics::asBoneWeights> tempData;
		tempData.resize(mesh->vertices.size());

		for (unsigned __int32  f = 0; f < srcMesh->mNumBones; f++)
		{
			aiBone* srcMeshBone = srcMesh->mBones[f];

			unsigned __int32 boneIndex = GetBoneIndex(srcMeshBone->mName.C_Str());
			//this->m_bones[boneIndex]->localTransform = Matrix(srcMeshBone->mOffsetMatrix[0]).Transpose();
			this->m_bones[boneIndex]->offsetTrm = Matrix(&srcMeshBone->mOffsetMatrix.a1).Transpose();
			for (unsigned __int32 w = 0; w < srcMeshBone->mNumWeights; w++)
			{
				// 해당 본에 영향을 받는 정점 번호
				unsigned __int32 index = srcMeshBone->mWeights[w].mVertexId;

				float weight = srcMeshBone->mWeights[w].mWeight;

				tempData[index].AddWeights(boneIndex, weight);
			}
		}


		//결국 이걸 다 넣어준다.
		for (unsigned __int32  v = 0; v < tempData.size(); v++)
		{
			tempData[v].Normalize();
			NestGraphics::asBlendWeight blendWeight = tempData[v].GetBlendWeights();
			mesh->vertices[v].blendIndices = (blendWeight.indices);

			mesh->vertices[v].blendWeights = blendWeight.weights;
		}
	}
}

 //<summary>
 //본의 이름으로 본의 인덱스를 찾는 함수
 //</summary>
 //<param name="_name"></param>
 //<returns></returns>
unsigned __int32 Converter::GetBoneIndex(const std::string& _name)
{
	for (std::shared_ptr<NestGraphics::Bone>& bone : this->m_bones)
	{
		if (bone->name == _name)
			return bone->index;
	}
	//return 0;
	std::cout << "GetBoneIndex faild to find" << _name << std::endl;
}

std::shared_ptr<NestGraphics::asAnimation> Converter::ReadAnimationData(aiAnimation* srcAnimation)
{
	std::shared_ptr<NestGraphics::asAnimation> animation = std::make_shared<NestGraphics::asAnimation>();
	animation->name = srcAnimation->mName.C_Str();
	animation->frameRate = static_cast<float>(srcAnimation->mTicksPerSecond);

	// 애니메이션 데이터를 프레임으로 분할할 때, 프레임당 1개의 데이터를 가지게 된다.
	// 이 애니메이션을 1초 단위로 나누고자 할 때, 1초를 더해서 소수점 단위를 없애준다.
	// ex : mDuration 값이 3.5 라면 3.5초 동안의 애니메이션을 나타내는 것 -> 1초를 더해서 4.5초로 만든다음 0.5초를 날려서 4초를 저장시킨다.
	animation->frameCount = static_cast<unsigned __int32>(srcAnimation->mDuration) + 1;

	std::map<std::string, std::shared_ptr<NestGraphics::asAnimationNode>> cacheAnimNodes;

	// 애니메이션이 들고있는 bone 채널
	for (unsigned __int32 i = 0; i < srcAnimation->mNumChannels; ++i)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// 애니메이션 노드 데이터 파싱 (bone 하나의 매 초마다 들고 있는 transform 값이 들어있는 node)
		std::shared_ptr<NestGraphics::asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		// 현재 찾은 노드 중에 제일 긴 시간으로 애니메이션 시간 갱신 (모든 bone 중에서 가장 긴 time 값으로 애니메이션 길이를 설정) 
		animation->duration = max(animation->duration, node->keyframe.back().time);

		cacheAnimNodes[srcNode->mNodeName.C_Str()] = node;
	}

	ReadKeyframeData(animation, m_pScene->mRootNode, cacheAnimNodes);

	return animation;
}

std::shared_ptr<NestGraphics::asAnimationNode> Converter::ParseAnimationNode(std::shared_ptr<NestGraphics::asAnimation> animation, aiNodeAnim* srcNode)
{
	std::shared_ptr<NestGraphics::asAnimationNode> node = std::make_shared<NestGraphics::asAnimationNode>();
	node->name = srcNode->mNodeName.C_Str();

	unsigned __int32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);

// 	assert(srcNode != nullptr);
// 	assert(srcNode->mNumPositionKeys == srcNode->mNumRotationKeys);
// 	assert(srcNode->mNumRotationKeys == srcNode->mNumScalingKeys);

	for (unsigned __int32 k = 0; k < keyCount; ++k)
	{
		NestGraphics::asKeyframeData frameData;

		bool found = false;

		unsigned __int32 t = static_cast<unsigned __int32>(node->keyframe.size());

		// Position  
		//if (::fabsf(static_cast<float>(srcNode->mPositionKeys[k].mTime) - static_cast<float>(t)) <= 0.001f)
		if (static_cast<float>(srcNode->mPositionKeys[k].mTime) - static_cast<float>(t) <= 0.001f)
		{
			//std::cout << static_cast<float>(srcNode->mPositionKeys[k].mTime) - static_cast<float>(t) << std::endl;
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = static_cast<float>(key.mTime);
			::memcpy_s(&frameData.translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Rotation
		if (::fabsf(static_cast<float>(srcNode->mRotationKeys[k].mTime) - static_cast<float>(t)) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = static_cast<float>(key.mTime);

			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}

		// Scale
		if (::fabsf(static_cast<float>(srcNode->mScalingKeys[k].mTime) - static_cast<float>(t)) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = static_cast<float>(key.mTime);
			::memcpy_s(&frameData.scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
			//::memcpy_s(&frameData.scale, sizeof(Vector3(1,1,1)), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
			node->keyframe.push_back(frameData);
	}

	if (node->keyframe.size() < animation->frameCount)
	{
		unsigned __int32 count = animation->frameCount - static_cast<unsigned __int32>(node->keyframe.size());
		NestGraphics::asKeyframeData keyFrame = node->keyframe.back();

		for (unsigned __int32 n = 0; n < count; ++n)
			node->keyframe.push_back(keyFrame);
	}

	return node;
}

void Converter::ReadKeyframeData(std::shared_ptr<NestGraphics::asAnimation> animation, aiNode* srcNode, std::map<std::string, std::shared_ptr<NestGraphics::asAnimationNode>>& cache)
{

	std::shared_ptr<NestGraphics::asKeyframeContainer> keyframe = std::make_shared<NestGraphics::asKeyframeContainer>();
	keyframe->boneName = srcNode->mName.C_Str();

	// 이미 cache 된 노드인지 검색
	std::shared_ptr<NestGraphics::asAnimationNode> findNode = cache[srcNode->mName.C_Str()];

	// 애니메이션의 총 frame 만큼 데이터를 복사한다.
	for (unsigned __int32 i = 0; i < animation->frameCount; i++)
	{
		NestGraphics::asKeyframeData frameData;

		if (findNode == nullptr)
		{
			Matrix transform(srcNode->mTransformation[0]);
			transform = transform.Transpose();
			frameData.time = (float)i;
			transform.Decompose(frameData.scale, frameData.rotation, frameData.translation);
		}
		else
		{
			frameData = findNode->keyframe[i];
		}

		keyframe->transforms.push_back(frameData);
	}

	// 애니메이션 키프레임 채우기 
	///TO DO :  교수님과 구조체가 달라서 원래대로 하려면 이곳을 살려야한다.
	animation->keyframes.push_back(keyframe);
	animation->mapKeyframes[keyframe->boneName] = keyframe;

	// 재귀적 호출
	for (unsigned __int32 i = 0; i < srcNode->mNumChildren; i++)
		ReadKeyframeData(animation, srcNode->mChildren[i], cache);
}

std::shared_ptr<NestGraphics::asAnimation> Converter::AnimatedRead(const aiAnimation* srcAnimation)
{
	for (size_t iChannel = 0; iChannel < srcAnimation->mNumChannels; iChannel++)
	{
		aiNodeAnim* nodeAnimation = srcAnimation->mChannels[iChannel];
		assert(nodeAnimation != nullptr);
		assert(nodeAnimation->mNumPositionKeys == nodeAnimation->mNumRotationKeys);
		assert(nodeAnimation->mNumRotationKeys == nodeAnimation->mNumScalingKeys);
	}

	auto animation = std::make_shared<NestGraphics::asAnimation>();
	//animation->name = srcAnimation->mName.C_Str();
	//animation->frameRate = static_cast<float>(srcAnimation->mTicksPerSecond);
	//animation->frameCount = static_cast<unsigned int>(srcAnimation->mDuration) + 1;

	//// 애니메이션을 단 하나만 처리하는 예제
	//m_Animations.resize(1);
	//m_Animations[0].duration = static_cast<float>(srcAnimation->mDuration / srcAnimation->mTicksPerSecond);

	//for (size_t iChannel = 0; iChannel < srcAnimation->mNumChannels; iChannel++) {
	//	const aiNodeAnim* pAiNodeAnim = srcAnimation->mChannels[iChannel];
	//	if (!pAiNodeAnim) continue; // 유효한 aiNodeAnim인지 확인

	//	NestGraphics::asKeyframeContainer keyframeContainer;

	//	// 최대 키프레임 수를 기반으로 keyframeContainer 초기화
	//	unsigned __int32 maxKeyframes = max(max(pAiNodeAnim->mNumPositionKeys, pAiNodeAnim->mNumScalingKeys), pAiNodeAnim->mNumRotationKeys);
	//	//unsigned __int32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);

	//	keyframeContainer.transforms.resize(maxKeyframes);

	//	// 위치, 회전, 스케일 키프레임 처리
	//	for (size_t i = 0; i < maxKeyframes; i++) {
	//		if (i < pAiNodeAnim->mNumPositionKeys) {
	//			const aiVectorKey& posKey = pAiNodeAnim->mPositionKeys[i];
	//			keyframeContainer.transforms[i].translation = { posKey.mValue.x, posKey.mValue.y, posKey.mValue.z };
	//			keyframeContainer.transforms[i].time = static_cast<float>(posKey.mTime / srcAnimation->mTicksPerSecond);
	//		}
	//		if (i < pAiNodeAnim->mNumRotationKeys) {
	//			const aiQuatKey& rotKey = pAiNodeAnim->mRotationKeys[i];
	//			keyframeContainer.transforms[i].rotation = { rotKey.mValue.x, rotKey.mValue.y, rotKey.mValue.z, rotKey.mValue.w };
	//			keyframeContainer.transforms[i].time = static_cast<float>(rotKey.mTime / srcAnimation->mTicksPerSecond);
	//		}
	//		if (i < pAiNodeAnim->mNumScalingKeys) {
	//			const aiVectorKey& sclKey = pAiNodeAnim->mScalingKeys[i];
	//			keyframeContainer.transforms[i].scale = { sclKey.mValue.x, sclKey.mValue.y, sclKey.mValue.z };
	//			keyframeContainer.transforms[i].time = static_cast<float>(sclKey.mTime / srcAnimation->mTicksPerSecond);
	//		}
	//	}
	//	// 각 채널에 대한 키프레임 컨테이너를 m_Animations에 추가
	//	m_Animations[0].keyframes.push_back(keyframeContainer);
	//}
	//animation = std::make_shared<NestGraphics::asAnimation>(m_Animations[0]);
	return animation;
}

//NestGraphics::Bone* Converter::ReadBone(const aiScene* pScene, const aiNode* pNode)
//{
//	NestGraphics::Bone& bone = m_bones.emplace_back();
//	NestGraphics::Skeleton skeleton;
//	bone.name = pNode->mName.C_Str();
//	bone.localTransform = Matrix(&pNode->mTransformation.a1).Transpose();
//	bone.NumChildren = pNode->mNumChildren;
//
//	m_bones.reserve(0);
//	int BoneIndex = (int)(m_bones.size() - 1);
//	skeleton.BoneMappingTable[bone.name] = BoneIndex;
//	UINT numMesh = pNode->mNumMeshes;
//	if (numMesh > 0)
//	{
//		bone.MeshNames.resize(numMesh);
//		for (UINT i = 0; i < numMesh; ++i)
//		{
//			UINT meshIndex = pNode->mMeshes[i];
//			aiMesh* pMesh = pScene->mMeshes[meshIndex];
//			std::string meshName = pMesh->mName.C_Str();
//			bone.MeshNames[i] = meshName;
//			// 메쉬가 어느본에 속해있는지 빨리찾기위해
//			skeleton.MeshMappingTable[meshName] = BoneIndex;
//		}
//	}
//
//	UINT numChild = pNode->mNumChildren;
//	for (UINT i = 0; i < numChild; ++i)
//	{
//		NestGraphics::Bone* child = ReadBone(m_pScene, pNode->mChildren[i]);
//		child->parentIndex = BoneIndex;
//	}
//
//	return &m_bones[BoneIndex];
//
//
//}

/// <summary>
/// 일단 임시로 만들어둔 Vertex, Index를 가지고 있는 MeshData를 반환하는 함수
/// 2024.01.16 
/// </summary>
/// <param name="mesh"></param>
/// 
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
		//   vertex.tangent.x = mesh->mTangents[i].x;
		//   vertex.tangent.y = mesh->mTangents[i].y;
		//   vertex.tangent.z = mesh->mTangents[i].z;

		//   vertex.bitangent.x = mesh->mBitangents[i].x;
		//   vertex.bitangent.y = mesh->mBitangents[i].y;
		//   vertex.bitangent.z = mesh->mBitangents[i].z;
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
	meshData.name = mesh->mName.C_Str();


	return meshData;
}

/*
//아직 생각중..
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

///동일님 코드--------------------------
		//AnimationKeys[i].Time = (float)(pos.mTime / tickPerSecond);
		//AnimationKeys[i].Position = { pos.mValue.x,pos.mValue.y,pos.mValue.z };
		//AnimationKeys[i].Rotation = { rot.mValue.x,rot.mValue.y,rot.mValue.z, rot.mValue.w };
		//AnimationKeys[i].Scaling = { scl.mValue.x,scl.mValue.y,scl.mValue.z };
		/*
		bool found = false;

		// 현재까지 처리된 키프레임의 수
		unsigned __int32 t = static_cast<unsigned __int32>(node->keyframe.size());

		// 애니메이션 데이터는 연속적이지 않을 수 있으며, 시간 값이 중복되거나 누락된 경우가 발생할 수 있다.
		// 무결성과 정확성을 보장하기 위해 조건문 추가
		//
		// Position
		if (::fabsf(static_cast<float>(srcNode->mPositionKeys[k].mTime) - static_cast<float>(t)) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mPositionKeys[k];
			frameData.time = static_cast<float>(key.mTime);
			::memcpy_s(&frameData.translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		// Rotation
		if (::fabsf(static_cast<float>(srcNode->mRotationKeys[k].mTime) - static_cast<float>(t)) <= 0.0001f)
		{
			aiQuatKey key = srcNode->mRotationKeys[k];
			frameData.time = static_cast<float>(key.mTime);

			frameData.rotation.x = key.mValue.x;
			frameData.rotation.y = key.mValue.y;
			frameData.rotation.z = key.mValue.z;
			frameData.rotation.w = key.mValue.w;

			found = true;
		}

		// Scale
		if (::fabsf(static_cast<float>(srcNode->mScalingKeys[k].mTime) - static_cast<float>(t)) <= 0.0001f)
		{
			aiVectorKey key = srcNode->mScalingKeys[k];
			frameData.time = static_cast<float>(key.mTime);
			::memcpy_s(&frameData.scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));

			found = true;
		}

		if (found == true)
			node->keyframe.push_back(frameData);
		*/