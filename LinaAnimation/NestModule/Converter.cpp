#pragma once
#include <iostream>      //����� �ƴҰ�� ����
#include "Converter.h"

Converter::Converter()
{
	std::cout << "������ ����\n";
	m_pImporter = std::make_shared<Assimp::Importer>();
	std::cout << "Importer ����\n";

	std::cout << "ModelData ����\n";

}

Converter::~Converter()
{

}

/// Asset������ �а�, �޸𸮿� �����Ѵ�.
/// ������ FBX�������� ���δ�. �����̴�. - DJ
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
		std::string fileStr(fileWStr.begin(), fileWStr.end());   //***wstring�� string���� ��ȯ
		m_pScene = m_pImporter->ReadFile
		(
			fileStr.c_str()
			//, aiProcessPreset_TargetRealtime_MaxQuality|   // �ǽð� �������� ���� ����ȭ ����
			, aiProcess_Triangulate | // �ﰢ������ ��ȯ
			aiProcess_GenNormals |	// �븻 ����/
			aiProcess_GenUVCoords |		// UV ����
			aiProcess_CalcTangentSpace |  // ź��Ʈ ����
			aiProcess_LimitBoneWeights |	// ���� ������ �޴� ������ �ִ� ������ 4���� ����
			aiProcess_ConvertToLeftHanded        // �޼� ��ǥ�踦 ����ϵ��� ���� ��ǥ�踦 ��ȯ
			
												 // | aiProcess_CalcTangentSpace            // ������ ������ ���� ������ ���   
			// | aiProcess_OptimizeMeshes            // �޽ø� ����ȭ�Ͽ� �ߺ��� ������ �����ϰ� �ε����� ����ȭ
			// | aiProcess_SplitLargeMeshes            // ũ�Ⱑ ū �޽ø� �����Ͽ� ������ ���� ���
			// | aiProcess_JoinIdenticalVertices      // �ߺ��� ������ ����
		);

		// TODO : ReadFile�� �и��Ұ�, ReadFile�� void��ȯ�Ѵ�.
		//        NestGraphics::ModelData* GetModel(
		if (m_pScene != nullptr)
		{
			m_pModelData = new NestGraphics::ModelData();
			ReadModelDataTwo(m_pScene->mRootNode, 0, -1); //��Ʈ ���� idnetity �� ��� -1���� �ϸ� �ȵǴ� ������ ��Ʈ ��忡 �����ϸ� ������. 
			//m_UnMapModelData.insert({ _key, m_pModelData });
			ReadSkinData();
			ConnectMesh();
			return m_pModelData;
		}
		else
		{
			std::cout << " ���� - m_pScene�� nullptr�� ��ȯ�Ͽ���\n";
			return nullptr;
		}
	}
}



/// <summary>
/// �ӽ÷� ������ N���� MeshData�� �������ִ� ModelData�� ����� �Լ�
/// 2024.01.16 
/// </summary>
/// <param name="node"></param>
void Converter::ReadModelData(aiNode* node)
{

	// ���� ����� MeshData�� �����Ѵ�.
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = m_pScene->mMeshes[node->mMeshes[i]];
		m_pModelData->meshes.push_back(ReadMeshData(mesh));
	}

	// �ڽ� ��带 Ž���Ѵ�.
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ReadModelData(node->mChildren[i]);
	}
}

/// <summary>
/// ������ ����� ���� Model�� �д� �Լ�
/// </summary>
/// <param name="node"></param>
/// <param name="index"></param>
/// <param name="parentNode"></param>
void Converter::ReadModelDataTwo(aiNode* node, __int32 index, __int32 parentIndex)
{
	std::shared_ptr<NestGraphics::Bone> bone = std::make_shared<NestGraphics::Bone>();
	//�θ��ε����� -1�ϰŴ�. �׸��� �ڽ����� ���� ������ 1�� �����Ѵ�.
	bone->index = index;
	bone->name = node->mName.C_Str();
	bone->parentIndex = parentIndex;

	Matrix transform(node->mTransformation[0]);
	bone->localTransform = transform.Transpose(); //assimp�� DX�� ��������ǰ谡 �ٸ��� ��-���� �ٸ��Ƿ� ��ġ����� ���Ѵ�. 

	m_bones.push_back(bone);

	ReadMeshDataTwo(node, index);

	//����� �ڽ� ������ Ÿ�°Ŷ� �Ȱ�����, �� ������ ���� �Բ� �޴°ŷ� ����
	for (__int32 i = 0; i < node->mNumChildren; i++)
	{
		ReadModelDataTwo(node->mChildren[i], static_cast<unsigned>(m_bones.size()), index);
	}


}

/// <summary>
/// �� ���� �޽� �������� �д� �Լ�. 
/// </summary>
/// <param name="node"></param>
/// <param name="bone"></param>
void Converter::ReadMeshDataTwo(aiNode* node, __int32 bone)
{
	//�޽����� 1 �̸��̶��, �翬�� �޽��� ���ٴ°Ŵ�.
	if (node->mNumMeshes < 1)
	{
		std::cout << "�޽����� 0���� ���� �о����ϴ�.. - �迹����" << std::endl;
		return;
	}

	//���Ⱑ ��¥ �Ľ��ϴ� �κ� �帧���� ���鼭 ���� �����.
	std::shared_ptr<NestGraphics::MeshData> mesh = std::make_shared<NestGraphics::MeshData>();
	mesh->name = node->mName.C_Str();
	mesh->boneIndex = bone;

	for (__int32 i = 0; i < node->mNumMeshes; ++i)
	{
		//�� �ε��� ��ȣ�� ������ ���� �޽� ��ȣ�� �ȴ�..�װɷ� ����
		// �ִ� �޽��� �����Ѵ�. 
		//������ �Ѵٰ� �ϴµ� ����� ���ظ���. TO DO: �ڵ��ƺ���
		__int32 index = node->mMeshes[i];
		const aiMesh* srcMesh = m_pScene->mMeshes[index];
		//���ݱ��� ���� ��ȣ ����̿�����  
		const __int32 startVec = mesh->vertices.size();

		//���ǿ��� ���⿡ ���׸��� �Ľ̵� �־���. ������ �츮�� �ʿ������
		//���ؽ� ��ȣ�� �ϳ��� �������ϸ鼭 ���ؽ��� uv��ǥ�� �븻 ��ǥ�� �־���.
		//���͵� �� �־���Ѵ�...�ƹ�ư ���ؽ� ���� ä��� �κ�
		for (__int32 v = 0; v < srcMesh->mNumVertices; ++v)
		{
			// Vertex
			NestGraphics::Vertex vertex;
			::memcpy(&vertex.position, &srcMesh->mVertices[v], sizeof(Vector3));

			// UV
			if (srcMesh->HasTextureCoords(0))
			{
				// &srcMesh->mTextureCoords[0][v] ù ��° ��ǥ ��Ʈ�� v��° ���ؽ��� ���� �ִ�
				// uv ����ü �ּҸ� ��Ÿ����. 
				::memcpy(&vertex.texcoord, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));
			}

			// Normal
			if (srcMesh->HasNormals())
			{
				::memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Vector3));
			}


			mesh->vertices.push_back(vertex);
		}

		//���Ⱑ �ε��� ���۸� ä��� �κ��� �ȴ�. �̰��� �⼮�� �ڵ带 �貼��.
		for (unsigned int f = 0; f < srcMesh->mNumFaces; ++f)
		{
			//aiface�� �ε����� ����..��� �Ѵ�
			aiFace& face = srcMesh->mFaces[f];
			for (__int32 k = 0; k < face.mNumIndices; k++)
			{
				mesh->indices.push_back(face.mIndices[k] + startVec);
			}

		}
		m_meshData.push_back(mesh); //�̰� �ۿ� ������ �ȵȴ�

	}

}
/// <summary>
/// �׷��Ƚ� ����ü �� �����ϴ� �κ�
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

	///�ִϸ��̼� ����?
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
/// ���� ����ġ�� ���ϴ� �Լ�
/// �������� � ���� ������ �����ǰ�?
/// </summary>
void Converter::ReadSkinData()
{
	for (unsigned __int32 i = 0; i < m_pScene->mNumMeshes; i++)
	{
		aiMesh* srcMesh = m_pScene->mMeshes[i];

		//���� ���ٸ�..?�� ���� �˻��ϴ°�. 
		if (!srcMesh->HasBones())
			continue;

		std::shared_ptr<NestGraphics::MeshData> mesh = std::make_shared<NestGraphics::MeshData>();
		mesh = m_meshData[i];

		// �� ��ȣ�� ����ġ�� �ӽ� ������ ����
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
				// �ش� ���� ������ �޴� ���� ��ȣ
				unsigned __int32 index = srcMeshBone->mWeights[w].mVertexId;

				float weight = srcMeshBone->mWeights[w].mWeight;

				tempData[index].AddWeights(boneIndex, weight);
			}
		}


		//�ᱹ �̰� �� �־��ش�.
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
 //���� �̸����� ���� �ε����� ã�� �Լ�
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

	// �ִϸ��̼� �����͸� ���������� ������ ��, �����Ӵ� 1���� �����͸� ������ �ȴ�.
	// �� �ִϸ��̼��� 1�� ������ �������� �� ��, 1�ʸ� ���ؼ� �Ҽ��� ������ �����ش�.
	// ex : mDuration ���� 3.5 ��� 3.5�� ������ �ִϸ��̼��� ��Ÿ���� �� -> 1�ʸ� ���ؼ� 4.5�ʷ� ������� 0.5�ʸ� ������ 4�ʸ� �����Ų��.
	animation->frameCount = static_cast<unsigned __int32>(srcAnimation->mDuration) + 1;

	std::map<std::string, std::shared_ptr<NestGraphics::asAnimationNode>> cacheAnimNodes;

	// �ִϸ��̼��� ����ִ� bone ä��
	for (unsigned __int32 i = 0; i < srcAnimation->mNumChannels; ++i)
	{
		aiNodeAnim* srcNode = srcAnimation->mChannels[i];

		// �ִϸ��̼� ��� ������ �Ľ� (bone �ϳ��� �� �ʸ��� ��� �ִ� transform ���� ����ִ� node)
		std::shared_ptr<NestGraphics::asAnimationNode> node = ParseAnimationNode(animation, srcNode);

		// ���� ã�� ��� �߿� ���� �� �ð����� �ִϸ��̼� �ð� ���� (��� bone �߿��� ���� �� time ������ �ִϸ��̼� ���̸� ����) 
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

	// �̹� cache �� ������� �˻�
	std::shared_ptr<NestGraphics::asAnimationNode> findNode = cache[srcNode->mName.C_Str()];

	// �ִϸ��̼��� �� frame ��ŭ �����͸� �����Ѵ�.
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

	// �ִϸ��̼� Ű������ ä��� 
	///TO DO :  �����԰� ����ü�� �޶� ������� �Ϸ��� �̰��� ������Ѵ�.
	animation->keyframes.push_back(keyframe);
	animation->mapKeyframes[keyframe->boneName] = keyframe;

	// ����� ȣ��
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

	//// �ִϸ��̼��� �� �ϳ��� ó���ϴ� ����
	//m_Animations.resize(1);
	//m_Animations[0].duration = static_cast<float>(srcAnimation->mDuration / srcAnimation->mTicksPerSecond);

	//for (size_t iChannel = 0; iChannel < srcAnimation->mNumChannels; iChannel++) {
	//	const aiNodeAnim* pAiNodeAnim = srcAnimation->mChannels[iChannel];
	//	if (!pAiNodeAnim) continue; // ��ȿ�� aiNodeAnim���� Ȯ��

	//	NestGraphics::asKeyframeContainer keyframeContainer;

	//	// �ִ� Ű������ ���� ������� keyframeContainer �ʱ�ȭ
	//	unsigned __int32 maxKeyframes = max(max(pAiNodeAnim->mNumPositionKeys, pAiNodeAnim->mNumScalingKeys), pAiNodeAnim->mNumRotationKeys);
	//	//unsigned __int32 keyCount = max(max(srcNode->mNumPositionKeys, srcNode->mNumScalingKeys), srcNode->mNumRotationKeys);

	//	keyframeContainer.transforms.resize(maxKeyframes);

	//	// ��ġ, ȸ��, ������ Ű������ ó��
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
	//	// �� ä�ο� ���� Ű������ �����̳ʸ� m_Animations�� �߰�
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
//			// �޽��� ������� �����ִ��� ����ã������
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
/// �ϴ� �ӽ÷� ������ Vertex, Index�� ������ �ִ� MeshData�� ��ȯ�ϴ� �Լ�
/// 2024.01.16 
/// </summary>
/// <param name="mesh"></param>
/// 
/// <returns></returns>
NestGraphics::MeshData Converter::ReadMeshData(aiMesh* mesh)
{
	NestGraphics::MeshData meshData;

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
		//   vertex.tangent.x = mesh->mTangents[i].x;
		//   vertex.tangent.y = mesh->mTangents[i].y;
		//   vertex.tangent.z = mesh->mTangents[i].z;

		//   vertex.bitangent.x = mesh->mBitangents[i].x;
		//   vertex.bitangent.y = mesh->mBitangents[i].y;
		//   vertex.bitangent.z = mesh->mBitangents[i].z;
		//}

		meshData.vertices.push_back(vertex);
	}

	// index�� meshData�� ����
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
//���� ������..
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

///���ϴ� �ڵ�--------------------------
		//AnimationKeys[i].Time = (float)(pos.mTime / tickPerSecond);
		//AnimationKeys[i].Position = { pos.mValue.x,pos.mValue.y,pos.mValue.z };
		//AnimationKeys[i].Rotation = { rot.mValue.x,rot.mValue.y,rot.mValue.z, rot.mValue.w };
		//AnimationKeys[i].Scaling = { scl.mValue.x,scl.mValue.y,scl.mValue.z };
		/*
		bool found = false;

		// ������� ó���� Ű�������� ��
		unsigned __int32 t = static_cast<unsigned __int32>(node->keyframe.size());

		// �ִϸ��̼� �����ʹ� ���������� ���� �� ������, �ð� ���� �ߺ��ǰų� ������ ��찡 �߻��� �� �ִ�.
		// ���Ἲ�� ��Ȯ���� �����ϱ� ���� ���ǹ� �߰�
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