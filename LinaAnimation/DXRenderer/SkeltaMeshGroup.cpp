#include "SkeltaMeshGroup.h"
#include <initializer_list>
#include <stb_image.h>
#include "BoneInstance.h"

SkeltaMeshGroup::SkeltaMeshGroup()
{
	//d3dUtilities = new D3dUtilities();
}

SkeltaMeshGroup::~SkeltaMeshGroup()
{
	//delete d3dUtilities;
}

void SkeltaMeshGroup::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix _trsMatrix, std::initializer_list<std::wstring> textureFileKeys)
{

	std::vector<NestGraphics::MeshData>* m_modelMeshData = &modelMeshDataes->meshes;
	std::vector<NestGraphics::Bone>* m_modelBoneData = &modelMeshDataes->bones;
	std::vector<NestGraphics::asAnimation>* m_modelAniData = &modelMeshDataes->animations;

	m_pModelData = modelMeshDataes;

	//std::vector<NestGraphics::MeshData> a = modelMeshDataes->meshes;

	std::initializer_list<std::wstring>::iterator textureFileKeysIterator = textureFileKeys.begin();
	for (size_t i = 0; i < m_modelMeshData->size() && i < textureFileKeys.size(); i++, textureFileKeysIterator++)
	{
		(*m_modelMeshData)[i].textureFilename = WstringToString(*textureFileKeysIterator);
	}

	// ������ ���� �並 ����
	objectVertexConstantData.world = Matrix();			// ��ü�� ���ؽ� ��� ������ ���� ���� 4X4��Ŀ� �����Ѵ�.
	objectVertexConstantData.view = Matrix();			// ��ü�� ���ؽ� ��� ������ ī�޶� �� ���� 4X4��Ŀ� �����Ѵ�.
	objectVertexConstantData.projection = Matrix();		// ��ü�� ���ؽ� ��� ������ ���� �� ���� 4X4��Ŀ� �����Ѵ�.

	d3dUtilities->CreateConstantBuffer(device, objectVertexConstantData, m_pVertexConstantBuffer); // ������Ʈ ���� ������ �����͸� �ʱ� �Ҵ� ���ۿ� ���� �� �����͸� �������� �ʱ� �Ҵ� ���۸� ����!
	d3dUtilities->CreateConstantBuffer(device, objectPixelConstantData, m_pPixelConstantBuffer); // ������Ʈ �ȼ� ������ �����͸� �ʱ� �Ҵ� ���ۿ� ���� �� �����͸� �������� �ʱ� �Ҵ� ���۸� ����!

	///
	for (const auto& aniData : (*m_modelAniData))
	{
		std::shared_ptr<NestGraphics::asAnimation> newAni = std::make_shared<NestGraphics::asAnimation>(aniData);
		m_Animations.push_back(newAni);
	}

	CreateBoneInstance(*m_modelBoneData);
	/*
	for (const auto& boneData : (*m_modelBoneData))
	{
		std::shared_ptr<NestGraphics::Bone> newBone = std::make_shared<NestGraphics::Bone>(boneData);
		m_Bones.push_back(newBone);
	}
	*/
	///

	///for���� ���鼭 ���ӿ������� ���� �����͸� ������Ʈ ��Ų��.
	for (const auto& meshData : (*m_modelMeshData))
	{
		std::shared_ptr<NestGraphics::Mesh> newMesh = std::make_shared<NestGraphics::Mesh>();	// Mesh ����ü�� �������� �Ҵ��Ѵ�.  
		d3dUtilities->CreateVertexBuffer(device, meshData.vertices, newMesh->m_vertexBuffer);	// ���ؽ� ���� �����͸� �޾ƿ��� �����͸� �������� �����Ѵ�.
		newMesh->m_indexCount = UINT(meshData.indices.size());									// �ε��� ������ �����Ѵ�. 
		d3dUtilities->CreateIndexBuffer(device, meshData.indices, newMesh->m_indexBuffer);		// �ε��� ���۸� �����ϰ� �־��� �����͸� �������� �����Ѵ�.


		if (!meshData.textureFilename.empty())																					 //�����Ͱ� ������ ���
		{
			std::cout << meshData.textureFilename << std::endl;
			d3dUtilities->CreateTexture(device, meshData.textureFilename, newMesh->texture, newMesh->textureResourceView);		 //�ý��ĸ� �����Ѵ�.
			//TestCreateTexture(device, meshData.textureFilename, newMesh->texture, newMesh->textureResourceView);
		}

		newMesh->m_vertexConstantBuffer = m_pVertexConstantBuffer;																 //newMesh���� vertexConstantBuffer�� ����
		newMesh->m_pixelConstantBuffer = m_pPixelConstantBuffer;																 //newMesh���� pixelConstantBuffer�� ����

		newMesh->boneIndex = meshData.boneIndex;

		this->m_pMeshes.push_back(newMesh);																						 //���� �ȿ� �ִ´�.
	}

	// �ԷµǴ� �����Ͱ��� � �������� ����	///���̴� ����
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLEND_INDICES", 0 ,DXGI_FORMAT_R32G32B32A32_SINT, 0, 32,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"BLEND_WEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	// ���ؽ��� hlsl�κ��� ������ �����ͼ� �Է� �����Ϳ� �ִ´�. // �ڼ��� ������ ���� �Լ� ���ø��� �����
	d3dUtilities->CreateVertexShaderAndInputLayout(device, L"../NestResources/hlsl/VertexShader.hlsl", inputElements, m_pVertexShader, m_pInputLayout);
	// CreateVertexShaderAndInputLayout()�� ���� ����
	d3dUtilities->CreatePixelShader(device, L"../NestResources/hlsl/PixelShader.hlsl", m_pPixelShader);

	// Sampler �����
	D3D11_SAMPLER_DESC sampDesc;						// ���÷� ���¸� ����
	ZeroMemory(&sampDesc, sizeof(sampDesc));			// ���÷��� �ʱ�ȭ �Ѵ�.
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// ���÷��� ���͸� ���� : ���, ���� �� �� ���� ���ø��� ���� ������ ���
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// 0���� 1 ���� �ۿ� �ִ� u �ؽ�ó ��ǥ�� Ȯ���ϴ� �� ��� : ��� (u,v) ���� ���տ��� �ؽ�ó�� Ÿ��ȭ
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// 0���� 1 ���� �ۿ� �ִ� v �ؽ�ó ��ǥ�� Ȯ���ϴ� �� ��� : "
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// 0���� 1 ���� �ۿ� �ִ� w �ؽ�ó ��ǥ�� Ȯ���ϴ� �� ����� �޼��� : "
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	// ���ø��� �����͸� ���� ���ø��� �����Ϳ� ���ϴ� �Լ� : �� �Ұ�
	sampDesc.MinLOD = 0;								// �׼����� ������ mipmap ������ �Ʒ��� ��(float)
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;				// �׼����� ������ mipmap ������ ���� ��
	device->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()); // �ؽ�ó�� ���� ���ø� ������ ĸ��ȭ�ϴ� ���÷� ���� ��ü�� �����. 
	// sampDesc : ���÷� ���� ���� ���� ������, m_pSamplerState : ���÷� ���� ��ü

	/// ���������� : SRV ���� -> ��� ���� ���� -> ��ǲ ����� ���� -> VS ���̴� -> PS ���̴�

	/// ���� �� �κ��� ������Ʈ ���۳��� ������ �� �Ͼ��? �׷����� ���� �� ����.

	//��������� ����°� �ν� �����ִ� �κ�
	//�޽��� ��ġ //���� ����� ���� ���� ���͸� ��ȯ �� �������� ������ ����

	Matrix invTRS = _trsMatrix;					// ������� ��� ������ ������ �����.
	invTRS.Translation(Vector3(0.0f));			// �̵� ��� �κи� 0, 0, 0���� �ٲ��ش�.
	invTRS = invTRS.Invert().Transpose();		//�����, ��ġ����� ���� �־��ش�.
	objectVertexConstantData.world = _trsMatrix.Transpose();	// �޾ƿ� ����� ��ġ ����� ���� ��ǥ�� �־��ش�.
	objectVertexConstantData.invTranspose = invTRS.Transpose();
	objectPixelConstantData.useTexture = false;
	objectPixelConstantData.material.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	objectPixelConstantData.material.specular = Vector3(0.0f);
	UpdateConstantBuffers(device, context);

	NestGraphics::VertexConstantBuffer a;
	std::cout << "123123123123123123123123123123123123123123123\n";
	std::cout << sizeof(a) << std::endl;
}

// ��� ���۸� ������Ʈ �ϴ� �Լ�(����̽�, ������ ����� �����ϴ� ����̽� ���ؽ�Ʈ)
void SkeltaMeshGroup::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	d3dUtilities->UpdateBuffer(device, context, objectVertexConstantData, m_pVertexConstantBuffer); // ���ؽ� ��� ���� ������ ���� ���� ���۸� ������Ʈ �Ѵ�.
	d3dUtilities->UpdateBuffer(device, context, objectPixelConstantData, m_pPixelConstantBuffer);	// ����� �ȼ� ���� ������ ������Ʈ �ȴ�.

}

/// <summary>
/// �޽��� �����ϴ� �Լ�
/// </summary>
/// <param name="context">�������� �����ϴ� ���ؽ�Ʈ</param>
void SkeltaMeshGroup::Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) /// ���⿡ int�� SRV ������ �Ű������� �޾ƿ;� �ϰ� �� ��..
{
	// ���ؽ�/�ε��� ���� ����
	UINT stride = sizeof(NestGraphics::Vertex); //���ؽ��� �� ��� ������ ������ ��Ÿ���� �����Դϴ�. (���ؽ� ���ۿ��� ���� ���ؽ��� �̵��� �� ���)
	UINT offset = 0;							//���ؽ� ������ ���� ��ġ������ ������

	// ���ؽ� ���̴��� �����Ѵ�
	context->VSSetShader(m_pVertexShader.Get(), 0, 0);				// ���ؽ� ���̴� ����
	context->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());	// �ȼ� ���÷� ����
	context->PSSetShader(m_pPixelShader.Get(), 0, 0);				// �ȼ� ���̴� ����

	const unsigned __int32 boneCount = m_pModelData->bones.size();
	std::vector<NestGraphics::Bone>* m_modelBoneData = &m_pModelData->bones;

	// ���ؽ�/�ε��� ���� ����
	for (const auto& meshes : m_pMeshes)
	{
		//objectVertexConstantData.world = m_BonesInsatnce[meshes->boneIndex].m_worldTrm.Transpose();

		for (size_t i = 0; i < m_BonesInsatnce.size(); i++)
		{
			Matrix& temp1 = m_BonesInsatnce[i].offsetTrm;   // �����ؿ��� �޽��� �󸶳� �������ִ����� �ǹ��ϴ� ������ Ʈ������
			Matrix& temp2 = m_MatrixPalette.transforms[i];  // ���� ������ ����Ʈ������.			
			objectVertexConstantData.BoneTransforms[i] = (temp1 * temp2).Transpose();
		}
		//objectVertexConstantData.m_TweenDesc = m_TweenDesc;
		//objectVertexConstantData.m_SRV = meshes->textureResourceView;
		UpdateConstantBuffers(device, context);

		


		ID3D11ShaderResourceView* resViews[1] =												// �ȼ� ���̴����� ����� ���̴� ���ҽ� �並 ����(�ؽ�ó ������ŭ �ִ´�.)
		{ meshes->textureResourceView.Get() };
		context->PSSetShaderResources(0, 1, resViews);										// �ȼ� ���̴��� ���̴� ���ҽ� �並 ���ε�
		context->IASetInputLayout(m_pInputLayout.Get());									//�Է� ���̾ƿ��� ����
		context->IASetVertexBuffers(0, 1, meshes->m_vertexBuffer.GetAddressOf(), &stride, &offset); // ���ؽ� ���۸� ����
		context->IASetIndexBuffer(meshes->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);	// �ε��� ���۸� ����
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);				// �⺻���� ������Ƽ�� ������ ���� : �ﰢ��
		context->VSSetConstantBuffers(0, 1, meshes->m_vertexConstantBuffer.GetAddressOf()); // ���ؽ� ���̴����� ����� ��� ���۸� ����
		context->PSSetConstantBuffers(0, 1, meshes->m_pixelConstantBuffer.GetAddressOf());	// �ȼ� ���̴����� ����� ��� ���۸� ����
		// ��ü �������� �� ť��ʵ� ���� ���
		context->DrawIndexed(meshes->m_indexCount, 0, 0);									// �ε����� ���ؽ��� ����Ͽ� ������ (�������� �ε��� �� ��)
	}

}


void SkeltaMeshGroup::CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld)
{
	//��ġ���� ����
	objectPixelConstantData.useTexture = true;
	objectPixelConstantData.eyeWorld = camEyeWorld;
	objectVertexConstantData.view = camViewRow.Transpose();
	objectVertexConstantData.projection = camProjRow.Transpose();

	shared_ptr<NestGraphics::asAnimation> currentAnim = m_Animations[0];


	if (!currentAnim) return; 

	// �ִϸ��̼� ������
	unsigned int frameCount = currentAnim->frameCount;
	float frameRate = currentAnim->frameRate;
	float duration = currentAnim->duration;

	float timePerFrame = duration / frameCount;

	static float elapsedTime = 0.f; 
	elapsedTime += m_dTime;

	if (elapsedTime >= duration) {
		elapsedTime = fmod(elapsedTime, duration); // �ִϸ��̼��� �ݺ��ǵ��� ����
	}

	///TO DO : �迹����
	// ���� ������ ���
	unsigned int currentFrame = static_cast<unsigned int>(elapsedTime / timePerFrame * 40)  % frameCount; //��Ʈ : 40����� ����, Ű������ Ȯ���Ұ� 

	unsigned int nextFrame = (currentFrame + 1) % frameCount;

	//������ �غ�
	float ratio = (elapsedTime - (currentFrame * timePerFrame)) / timePerFrame;

	//for (auto& keyframePair : m_Animations[0]->keyframes) {
	//	auto keyframe = keyframePair.second;

	//	scale = m_Animations[0]->keyframes[0]->transforms[currentFrame].scale;
	//	rotation = m_Animations[0]->keyframes[0]->transforms[currentFrame].rotation;
	//	position = m_Animations[0]->keyframes[0]->transforms[currentFrame].translation;

	//}

	//index++;
	//index = index % frameCount;
	//Vector3 vScale;
	//Quaternion qRotation;
	//Vector3 vTrans;

	vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	shared_ptr<NestGraphics::asAnimation> animation = m_Animations[index];

	for (size_t i = 0; i < m_BonesInsatnce.size(); i++)
	{
		Matrix local = Matrix::CreateScale(m_BonesInsatnce[i].m_KeyframeContainer->transforms[currentFrame].scale) *
			Matrix::CreateFromQuaternion(m_BonesInsatnce[i].m_KeyframeContainer->transforms[currentFrame].rotation) *
			Matrix::CreateTranslation(m_BonesInsatnce[i].m_KeyframeContainer->transforms[currentFrame].translation);


		if (m_BonesInsatnce[i].m_pBoneParent == nullptr)
		{
			m_BonesInsatnce[i].m_worldTrm = local;
		}
		else
		{


			/*		std::cout << m_BonesInsatnce[i].m_KeyframeContainer->transforms[currentFrame].scale.x << std::endl;
					std::cout << m_BonesInsatnce[i].m_KeyframeContainer->transforms[currentFrame].scale.y << std::endl;
					std::cout << m_BonesInsatnce[i].m_KeyframeContainer->transforms[currentFrame].scale.z << std::endl;*/

			m_BonesInsatnce[i].m_worldTrm = local * m_BonesInsatnce[i].m_pBoneParent->m_worldTrm;
			//m_BonesInsatnce[i].m_worldTrm = m_BonesInsatnce[i].m_localTrm * m_BonesInsatnce[i].m_pBoneParent->m_worldTrm;
		}


		m_MatrixPalette.transforms[i] = m_BonesInsatnce[i].m_worldTrm;
	}

	//UpdateTweenData(m_dTime);

	//UpdateConstantBuffers(device, context);
}

void SkeltaMeshGroup::LightUpdate(float m_diffuse, float m_specualr, bool m_useTexture, float m_norScale, int m_lightType, NestGraphics::Light m_lightFromGUI)
{
	//������ ���� (��� �������� �ִ°� �ƴϴ�.) ä����� : �ȼ� ���� �����Ϳ� ��ǻ��� ����ŧ�� ���� �����Ѵ�.
	objectPixelConstantData.material.diffuse = Vector3(m_diffuse);
	objectPixelConstantData.material.specular = Vector3(m_specualr);
	objectPixelConstantData.useTexture = m_useTexture;

	// ���� �� ���� ����ϱ� ����, ������� �ʴ� ������ ���� ������ 0���� �����ؼ� �׸�
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		// �ٸ� ���� ����
		if (i != m_lightType)
		{
			objectPixelConstantData.lights[i].strength *= 0.0f;
		}
		else
		{
			objectPixelConstantData.lights[i] = m_lightFromGUI;
		}
	}
}

std::string SkeltaMeshGroup::WstringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}

void SkeltaMeshGroup::UpdateAnimationContainer(int animationIndex)
{

}

void SkeltaMeshGroup::CreateBoneInstance(std::vector<NestGraphics::Bone>& modelBone)
{
	// �̹�  modelData�� ���������� ����ִ�.
	// �׸��� �ؾߵɰ��� ��Ʈ���ν��Ͻ��� Ʈ���� �����Ѵ�.

	m_BonesInsatnce.reserve(modelBone.size());

	// First, create all bone instances without setting parents.
	for (const auto& BoneInfo : modelBone)
	{
		auto& BoneInstance = m_BonesInsatnce.emplace_back();
		BoneInstance.m_localTrm = BoneInfo.localTransform;
		BoneInstance.m_boneName = BoneInfo.name;
		// Initially, don't set parents. This will be done in a separate loop.
	}

	// Now, set parent-child relationships.
	for (size_t i = 0; i < modelBone.size(); ++i)
	{
		int ParentIndex = modelBone[i].parentIndex;
		if (ParentIndex != -1) // Check if the bone has a parent
		{
			// Set the parent of the current bone instance.
			m_BonesInsatnce[i].m_pBoneParent = &m_BonesInsatnce[ParentIndex];
		}
		else
		{
			// If ParentIndex is -1, it means this bone has no parent.
			m_BonesInsatnce[i].m_pBoneParent = nullptr;
		}

		//�𵨵���Ÿ �ȿ� �ִ� Ű������ ������ �� ��ü�� ���� �־��־���.
		// �𵨵����� �ȿ��ִ� Ű�����������̳� ���ҽ��� ��������ش�.
		m_BonesInsatnce[i].m_KeyframeContainer = m_pModelData->animations[0].keyframes[i];
		m_BonesInsatnce[i].BoneIndex = modelBone[i].index;
		m_BonesInsatnce[i].offsetTrm = modelBone[i].offsetTrm;
	}

}



void SkeltaMeshGroup::UpdateTweenData(float m_dTime)
{
	std::vector<NestGraphics::asAnimation>* m_modelAniData = &m_pModelData->animations;
	NestGraphics::TweenDesc& desc = m_TweenDesc;
	desc.curr.sumTime += m_dTime;

	// ���� �ִϸ��̼� ���!
	{
		shared_ptr<NestGraphics::asAnimation> currentAnim = m_Animations[desc.curr.animIndex];
		if (currentAnim)
		{
			// �ִϸ��̼��� 1������ �ҿ� �ð��� ���Ѵ�.
			float timePerFrame = 1.f / (currentAnim->frameRate * desc.curr.speed);

			// �ִϸ��̼� 1�������� �����ٸ�
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0;

				// 1�� �ø��⸸ �ϸ� �� ������ ���� �ʰ��� �� ������ ������ ������ �־��ش�.
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
			}

			// ������ ���� ���� �� (0 ~ 1)
			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// ���� �ִϸ��̼�
	if (desc.next.animIndex >= 0)
	{
		desc.tweenSumTime += m_dTime;
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		if (desc.tweenRatio >= 1.f)
		{
			// �ִϸ��̼� ��ü ����
			desc.curr = desc.next;
			desc.ClearNextAnim();
		}
		else
		{
			// ��ü�ϴ� �κ� �ٵ� ���߿� �պ����ҵ�.
			shared_ptr<NestGraphics::asAnimation> nextAnim = m_Animations[desc.next.animIndex];
			desc.next.sumTime += m_dTime;

			float timePerFrame = 1.f / (nextAnim->frameRate * desc.next.speed);
			if (desc.next.ratio >= 1.f)
			{
				desc.next.sumTime = 0;
				desc.next.currFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
				desc.next.nextFrame = (desc.next.currFrame + 1) % nextAnim->frameCount;
			}

			desc.next.ratio = (desc.next.sumTime / timePerFrame);
		}
	}

}

void SkeltaMeshGroup::CreateAnimationTransform(unsigned __int32 index)
{
	vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	shared_ptr<NestGraphics::asAnimation> animation = m_Animations[index];


	for (unsigned __int32  f = 0; f < animation->frameCount; ++f)
	{
		// ������ bone �� ���� (3��)
		for (unsigned __int32  b = 0; b < m_pModelData->bones.size(); ++b)
		{
			shared_ptr<NestGraphics::Bone> bone = m_Bones[b];

			Matrix matAnimation;

			shared_ptr<NestGraphics::asKeyframeContainer> frame = animation->mapKeyframes[bone->name];
			if (frame != nullptr)
			{
				if (frame->transforms.size() == 0)
					continue;

				NestGraphics::asKeyframeData& data = frame->transforms[f];

				Matrix scale = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				Matrix rotation = Matrix::CreateFromQuaternion(data.rotation);
				Matrix translation = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);

				// �̷��� ���� SRT�� �θ� ���� ��ȯ ���
				matAnimation = scale * rotation * translation;
			}

			else
			{
				matAnimation = Matrix::Identity;
			}

			// ���� �߿��� ����.

			// Model
			// �ֻ��� ��Ʈ ��ȯ���
			Matrix toRootMatrix = bone->localTransform;

			// ������� ������ �ִ� �θ� ���� ��ȯ ��� 
			Matrix invGlobal = toRootMatrix.Invert();

			// �θ���index
			__int32 parentIndex = bone->parentIndex;

			// Animation
			// �ֻ��� ��Ʈ ��ȯ����� ������ش�.
			Matrix matParent = Matrix::Identity;
			if (parentIndex >= 0)
				matParent = tempAnimBoneTransforms[parentIndex];

			tempAnimBoneTransforms[b] = matAnimation * matParent;

			// T �������� �������̴� Model���� 
			// �ֻ��� ��Ʈ ���� ��ȯ ����� ������ ��� �־��µ� 
			// ����ķ� �θ� ���� ��ȯ��ķ� �ٲ۴�.

			// Animation ������ �θ� ���� ��ȯ����� ��� �ְ�,
			// �̸� �ֻ��� ��Ʈ ���� ��ȯ ��ķ� ������ش�.

			// �̷��� ���� Model ���� Relative ��ȯ ��İ� 
			// Animation ���� Root ��ȯ ����� ���������ν�
			// Animation transform���� ��ǥ �̵��� �ȴ�.
			m_AnimTransforms[index].transforms[f][b] = invGlobal * tempAnimBoneTransforms[b];
		}
	}
}

void SkeltaMeshGroup::TestCreateTexture(ComPtr<ID3D11Device>& device, const std::string filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{
	m_AnimTransforms.resize(m_Animations.size());

	for (unsigned __int32 i = 0; i < m_Animations.size(); ++i)
	{
		//CreateAnimationTransform(i);
	}

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));

	// bone�� ����ִ� transform�� 64����Ʈ������ texture format����
	// ���� ū ���� 16����Ʈ �̹Ƿ� * 4�� ������ش�.
	// ���Ŀ� * 4 ��ŭ�� 1ĭ���� ��� shader�� ������ ��.
	desc.Width = MAX_MODEL_TRANSFORMS * 4;
	desc.Height = MAX_MODEL_KEYFRAMES;
	desc.ArraySize = m_Animations.size();
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16����Ʈ
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	// �޸� �Ҵ� ���� 
	// 3���� �迭 ũ�� ��ŭ �޸𸮸� �Ҵ��Ѵ�.
	const unsigned __int32  dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix); // ����
	const unsigned __int32  pageSize = dataSize * MAX_MODEL_KEYFRAMES;		   // ���� * ����
	void* mallocPtr = ::malloc(pageSize * m_Animations.size()); // ���� * ���� * �ִϸ��̼� �� 

	// ����ȭ�� �����͸� �����Ѵ�.
	for (unsigned __int32 c = 0; c < m_Animations.size(); ++c)
	{
		// �ִϸ��̼��� �������� �� �׸�ŭ offset�� �д�.
		unsigned __int32 startOffset = c * pageSize;

		BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

		// frame���� �ִ� ��� bone�� SRT������ �о�־��ش�. 
		for (unsigned __int32 f = 0; f < MAX_MODEL_KEYFRAMES; ++f)
		{
			void* ptr = pageStartPtr + dataSize * f;
			::memcpy(ptr, m_AnimTransforms[c].transforms[f].data(), dataSize);
		}
	}

	// ���ҽ� �����
	vector<D3D11_SUBRESOURCE_DATA> subResources(m_Animations.size());

	// �ִϸ��̼��� 3���� �ִٰ� �����Ѵٸ�,
	// 1�� �ִϸ��̼� ���� 2�� �ִϸ��̼� ���������� ��� �����͵��� �迭[0]��
	// 2�� �ִϸ��̼� ���� 3�� �ִϸ��̼� ���������� ��� �����͵��� �迭[1]��
	// 3�� �ִϸ��̼� ���� ������������ ��� �����͵��� �迭[3]�� �����Ų��.
	for (unsigned __int32 c = 0; c < m_Animations.size(); ++c)
	{
		void* ptr = (BYTE*)mallocPtr + c * pageSize;
		subResources[c].pSysMem = ptr;
		subResources[c].SysMemPitch = dataSize;
		subResources[c].SysMemSlicePitch = pageSize;
	}

	device->CreateTexture2D(&desc, subResources.data(), texture.GetAddressOf());

	::free(mallocPtr);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.ArraySize = m_Animations.size();

	device->CreateShaderResourceView(texture.Get(), &srvDesc, textureResourceView.GetAddressOf());
}