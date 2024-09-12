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

	// 버퍼의 현재 뷰를 설정
	objectVertexConstantData.world = Matrix();			// 물체의 버텍스 상수 버퍼의 월드 값을 4X4행렬에 저장한다.
	objectVertexConstantData.view = Matrix();			// 물체의 버텍스 상수 버퍼의 카메라 뷰 값을 4X4행렬에 저장한다.
	objectVertexConstantData.projection = Matrix();		// 물체의 버텍스 상수 버퍼의 투영 뷰 값을 4X4행렬에 저장한다.

	d3dUtilities->CreateConstantBuffer(device, objectVertexConstantData, m_pVertexConstantBuffer); // 오브젝트 정점 버퍼의 데이터를 초기 할당 버퍼에 전달 후 데이터를 바탕으로 초기 할당 버퍼를 생성!
	d3dUtilities->CreateConstantBuffer(device, objectPixelConstantData, m_pPixelConstantBuffer); // 오브젝트 픽셸 버퍼의 데이터를 초기 할당 버퍼에 전달 후 데이터를 바탕으로 초기 할당 버퍼를 생성!

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

	///for문을 돌면서 게임엔진에서 받은 데이터를 업데이트 시킨다.
	for (const auto& meshData : (*m_modelMeshData))
	{
		std::shared_ptr<NestGraphics::Mesh> newMesh = std::make_shared<NestGraphics::Mesh>();	// Mesh 구조체를 동적으로 할당한다.  
		d3dUtilities->CreateVertexBuffer(device, meshData.vertices, newMesh->m_vertexBuffer);	// 버텍스 버퍼 데이터를 받아오고 데이터를 바탕으로 생성한다.
		newMesh->m_indexCount = UINT(meshData.indices.size());									// 인덱스 개수를 설정한다. 
		d3dUtilities->CreateIndexBuffer(device, meshData.indices, newMesh->m_indexBuffer);		// 인덱스 버퍼를 생성하고 넣어준 데이터를 바탕으로 생성한다.


		if (!meshData.textureFilename.empty())																					 //데이터가 존재할 경우
		{
			std::cout << meshData.textureFilename << std::endl;
			d3dUtilities->CreateTexture(device, meshData.textureFilename, newMesh->texture, newMesh->textureResourceView);		 //택스쳐를 생성한다.
			//TestCreateTexture(device, meshData.textureFilename, newMesh->texture, newMesh->textureResourceView);
		}

		newMesh->m_vertexConstantBuffer = m_pVertexConstantBuffer;																 //newMesh안의 vertexConstantBuffer에 대입
		newMesh->m_pixelConstantBuffer = m_pPixelConstantBuffer;																 //newMesh안의 pixelConstantBuffer에 대입

		newMesh->boneIndex = meshData.boneIndex;

		this->m_pMeshes.push_back(newMesh);																						 //벡터 안에 넣는다.
	}

	// 입력되는 데이터값이 어떤 형태인지 저장	///세이더 설정
	vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLEND_INDICES", 0 ,DXGI_FORMAT_R32G32B32A32_SINT, 0, 32,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"BLEND_WEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA,0},
	};

	// 버텍스의 hlsl로부터 정보를 가져와서 입력 데이터에 넣는다. // 자세한 설명은 관련 함수 템플릿에 적어둠
	d3dUtilities->CreateVertexShaderAndInputLayout(device, L"../NestResources/hlsl/VertexShader.hlsl", inputElements, m_pVertexShader, m_pInputLayout);
	// CreateVertexShaderAndInputLayout()와 구조 동일
	d3dUtilities->CreatePixelShader(device, L"../NestResources/hlsl/PixelShader.hlsl", m_pPixelShader);

	// Sampler 만들기
	D3D11_SAMPLER_DESC sampDesc;						// 샘플러 상태를 설명
	ZeroMemory(&sampDesc, sizeof(sampDesc));			// 샘플러를 초기화 한다.
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	// 샘플러의 필터를 설정 : 축소, 배율 및 밉 수준 샘플링에 선형 보간을 사용
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		// 0에서 1 범위 밖에 있는 u 텍스처 좌표를 확인하는 데 사용 : 모든 (u,v) 정수 접합에서 텍스처를 타일화
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;		// 0에서 1 범위 밖에 있는 v 텍스처 좌표를 확인하는 데 사용 : "
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;		// 0에서 1 범위 밖에 있는 w 텍스처 좌표를 확인하는 데 사용할 메서드 : "
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;	// 샘플링된 데이터를 기존 샘플링된 데이터와 비교하는 함수 : 비교 불가
	sampDesc.MinLOD = 0;								// 액세스를 고정할 mipmap 범위의 아래쪽 끝(float)
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;				// 액세스를 고정할 mipmap 범위의 위쪽 끝
	device->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()); // 텍스처에 대한 샘플링 정보를 캡슐화하는 샘플러 상태 개체를 만든다. 
	// sampDesc : 샘플러 상태 설명에 대한 포인터, m_pSamplerState : 샘플러 상태 개체

	/// 파이프라인 : SRV 설정 -> 상수 버퍼 생성 -> 인풋 어셈블러 생성 -> VS 쉐이더 -> PS 쉐이더

	/// 과연 이 부분이 컨스턴트 버퍼끼리 연동이 안 일어날까? 그럴리가 없을 거 같아.

	//여기까지가 구라는걸 인식 시켜주는 부분
	//메쉬의 위치 //빛의 계산을 위해 법선 벡터를 변환 후 스케일의 영향을 보정

	Matrix invTRS = _trsMatrix;					// 역행렬을 잠시 저장할 변수를 만든다.
	invTRS.Translation(Vector3(0.0f));			// 이동 행렬 부분만 0, 0, 0으로 바꿔준다.
	invTRS = invTRS.Invert().Transpose();		//역행렬, 전치행렬을 만들어서 넣어준다.
	objectVertexConstantData.world = _trsMatrix.Transpose();	// 받아온 행렬의 전치 행렬을 월드 좌표에 넣어준다.
	objectVertexConstantData.invTranspose = invTRS.Transpose();
	objectPixelConstantData.useTexture = false;
	objectPixelConstantData.material.diffuse = Vector3(0.5f, 0.5f, 0.5f);
	objectPixelConstantData.material.specular = Vector3(0.0f);
	UpdateConstantBuffers(device, context);

	NestGraphics::VertexConstantBuffer a;
	std::cout << "123123123123123123123123123123123123123123123\n";
	std::cout << sizeof(a) << std::endl;
}

// 상수 버퍼를 업데이트 하는 함수(디바이스, 렌더링 명령을 생성하는 디바이스 컨텍스트)
void SkeltaMeshGroup::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	d3dUtilities->UpdateBuffer(device, context, objectVertexConstantData, m_pVertexConstantBuffer); // 버텍스 상수 버퍼 데이터 값을 토대로 버퍼를 업데이트 한다.
	d3dUtilities->UpdateBuffer(device, context, objectPixelConstantData, m_pPixelConstantBuffer);	// 노멀의 픽셸 값은 언제나 업데이트 된다.

}

/// <summary>
/// 메쉬를 랜더하는 함수
/// </summary>
/// <param name="context">랜더링을 제어하는 컨텍스트</param>
void SkeltaMeshGroup::Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) /// 여기에 int로 SRV 개수를 매개변수로 받아와야 하게 될 것..
{
	// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(NestGraphics::Vertex); //버텍스의 각 요소 사이의 간격을 나타내는 변수입니다. (버텍스 버퍼에서 다음 버텍스로 이동할 때 사용)
	UINT offset = 0;							//버텍스 버퍼의 시작 위치에서의 오프셋

	// 버텍스 쉐이더를 설정한다
	context->VSSetShader(m_pVertexShader.Get(), 0, 0);				// 버텍스 쉐이더 설정
	context->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());	// 픽셀 샘플러 설정
	context->PSSetShader(m_pPixelShader.Get(), 0, 0);				// 픽셀 쉐이더 설정

	const unsigned __int32 boneCount = m_pModelData->bones.size();
	std::vector<NestGraphics::Bone>* m_modelBoneData = &m_pModelData->bones;

	// 버텍스/인덱스 버퍼 설정
	for (const auto& meshes : m_pMeshes)
	{
		//objectVertexConstantData.world = m_BonesInsatnce[meshes->boneIndex].m_worldTrm.Transpose();

		for (size_t i = 0; i < m_BonesInsatnce.size(); i++)
		{
			Matrix& temp1 = m_BonesInsatnce[i].offsetTrm;   // 본기준에서 메쉬가 얼마나 떨어져있는지는 의미하는 오프셋 트랜스폼
			Matrix& temp2 = m_MatrixPalette.transforms[i];  // 최종 본들의 월드트랜스폼.			
			objectVertexConstantData.BoneTransforms[i] = (temp1 * temp2).Transpose();
		}
		//objectVertexConstantData.m_TweenDesc = m_TweenDesc;
		//objectVertexConstantData.m_SRV = meshes->textureResourceView;
		UpdateConstantBuffers(device, context);

		


		ID3D11ShaderResourceView* resViews[1] =												// 픽셀 쉐이더에서 사용할 셰이더 리소스 뷰를 설정(텍스처 개수만큼 넣는다.)
		{ meshes->textureResourceView.Get() };
		context->PSSetShaderResources(0, 1, resViews);										// 픽셀 쉐이더에 셰이더 리소스 뷰를 바인딩
		context->IASetInputLayout(m_pInputLayout.Get());									//입력 레이아웃을 설정
		context->IASetVertexBuffers(0, 1, meshes->m_vertexBuffer.GetAddressOf(), &stride, &offset); // 버텍스 버퍼를 설정
		context->IASetIndexBuffer(meshes->m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);	// 인덱스 버퍼를 설정
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);				// 기본적인 프리미티브 유형을 설정 : 삼각형
		context->VSSetConstantBuffers(0, 1, meshes->m_vertexConstantBuffer.GetAddressOf()); // 버텍스 쉐이더에서 사용할 상수 버퍼를 설정
		context->PSSetConstantBuffers(0, 1, meshes->m_pixelConstantBuffer.GetAddressOf());	// 픽셀 쉐이더에서 사용할 상수 버퍼를 설정
		// 물체 렌더링할 때 큐브맵도 같이 사용
		context->DrawIndexed(meshes->m_indexCount, 0, 0);									// 인덱스된 버텍스를 사용하여 렌더링 (렌더링된 인덱수 총 수)
	}

}


void SkeltaMeshGroup::CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld)
{
	//위치관련 업뎃
	objectPixelConstantData.useTexture = true;
	objectPixelConstantData.eyeWorld = camEyeWorld;
	objectVertexConstantData.view = camViewRow.Transpose();
	objectVertexConstantData.projection = camProjRow.Transpose();

	shared_ptr<NestGraphics::asAnimation> currentAnim = m_Animations[0];


	if (!currentAnim) return; 

	// 애니메이션 데이터
	unsigned int frameCount = currentAnim->frameCount;
	float frameRate = currentAnim->frameRate;
	float duration = currentAnim->duration;

	float timePerFrame = duration / frameCount;

	static float elapsedTime = 0.f; 
	elapsedTime += m_dTime;

	if (elapsedTime >= duration) {
		elapsedTime = fmod(elapsedTime, duration); // 애니메이션이 반복되도록 설정
	}

	///TO DO : 김예리나
	// 현재 프레임 계산
	unsigned int currentFrame = static_cast<unsigned int>(elapsedTime / timePerFrame * 40)  % frameCount; //아트 : 40배속이 적당, 키프레임 확인할것 

	unsigned int nextFrame = (currentFrame + 1) % frameCount;

	//보간을 준비
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
	//빛관련 업뎃 (사실 빛정보만 있는건 아니다.) 채윤언니 : 픽셸 버퍼 데이터에 디퓨즈와 스펙큘러 값을 저장한다.
	objectPixelConstantData.material.diffuse = Vector3(m_diffuse);
	objectPixelConstantData.material.specular = Vector3(m_specualr);
	objectPixelConstantData.useTexture = m_useTexture;

	// 여러 개 조명 사용하기 위해, 사용하지 않는 조명의 값을 강제로 0으로 조정해서 그림
	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		// 다른 조명 끄기
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
	// 이미  modelData에 본정보들이 들어있다.
	// 그리고 해야될것은 루트본인스턴스에 트리를 생성한다.

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

		//모델데이타 안에 있던 키프레임 정보를 본 객체에 값을 넣어주었다.
		// 모델데이터 안에있는 키프레임컨테이너 리소스를 연결시켜준다.
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

	// 현재 애니메이션 재생!
	{
		shared_ptr<NestGraphics::asAnimation> currentAnim = m_Animations[desc.curr.animIndex];
		if (currentAnim)
		{
			// 애니메이션의 1프레임 소요 시간을 구한다.
			float timePerFrame = 1.f / (currentAnim->frameRate * desc.curr.speed);

			// 애니메이션 1프레임이 끝났다면
			if (desc.curr.sumTime >= timePerFrame)
			{
				desc.curr.sumTime = 0;

				// 1을 늘리기만 하면 총 프레임 수를 초과할 수 있으니 나머지 연산을 넣어준다.
				desc.curr.currFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
				desc.curr.nextFrame = (desc.curr.currFrame + 1) % currentAnim->frameCount;
			}

			// 보간을 위한 비율 값 (0 ~ 1)
			desc.curr.ratio = (desc.curr.sumTime / timePerFrame);
		}
	}

	// 다음 애니메이션
	if (desc.next.animIndex >= 0)
	{
		desc.tweenSumTime += m_dTime;
		desc.tweenRatio = desc.tweenSumTime / desc.tweenDuration;

		if (desc.tweenRatio >= 1.f)
		{
			// 애니메이션 교체 성공
			desc.curr = desc.next;
			desc.ClearNextAnim();
		}
		else
		{
			// 교체하는 부분 근데 나중에 손봐야할듯.
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
		// 각각의 bone 에 접근 (3차)
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

				// 이렇게 구한 SRT는 부모 기준 변환 행렬
				matAnimation = scale * rotation * translation;
			}

			else
			{
				matAnimation = Matrix::Identity;
			}

			// 가장 중요한 구간.

			// Model
			// 최상위 루트 변환행렬
			Matrix toRootMatrix = bone->localTransform;

			// 역행렬은 기존에 있던 부모 기준 변환 행렬 
			Matrix invGlobal = toRootMatrix.Invert();

			// 부모의index
			__int32 parentIndex = bone->parentIndex;

			// Animation
			// 최상위 루트 변환행렬을 만들어준다.
			Matrix matParent = Matrix::Identity;
			if (parentIndex >= 0)
				matParent = tempAnimBoneTransforms[parentIndex];

			tempAnimBoneTransforms[b] = matAnimation * matParent;

			// T 자형으로 고정적이던 Model에서 
			// 최상위 루트 기준 변환 행렬을 모조리 들고 있었는데 
			// 역행렬로 부모 기준 변환행렬로 바꾼다.

			// Animation 에서는 부모 기준 변환행렬을 들고 있고,
			// 이를 최상위 루트 기준 변환 행렬로 만들어준다.

			// 이렇게 구한 Model 기준 Relative 변환 행렬과 
			// Animation 기준 Root 변환 행렬을 곱해줌으로써
			// Animation transform으로 좌표 이동이 된다.
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

	// bone이 들고있는 transform은 64바이트이지만 texture format에서
	// 가장 큰 값은 16바이트 이므로 * 4로 만들어준다.
	// 추후에 * 4 만큼을 1칸으로 묶어서 shader에 보내줄 것.
	desc.Width = MAX_MODEL_TRANSFORMS * 4;
	desc.Height = MAX_MODEL_KEYFRAMES;
	desc.ArraySize = m_Animations.size();
	desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16바이트
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;

	// 메모리 할당 구간 
	// 3차원 배열 크기 만큼 메모리를 할당한다.
	const unsigned __int32  dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix); // 가로
	const unsigned __int32  pageSize = dataSize * MAX_MODEL_KEYFRAMES;		   // 가로 * 세로
	void* mallocPtr = ::malloc(pageSize * m_Animations.size()); // 가로 * 세로 * 애니메이션 수 

	// 파편화된 데이터를 조립한다.
	for (unsigned __int32 c = 0; c < m_Animations.size(); ++c)
	{
		// 애니메이션이 여러개일 때 그만큼 offset을 둔다.
		unsigned __int32 startOffset = c * pageSize;

		BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

		// frame마다 있는 모든 bone의 SRT값들을 밀어넣어준다. 
		for (unsigned __int32 f = 0; f < MAX_MODEL_KEYFRAMES; ++f)
		{
			void* ptr = pageStartPtr + dataSize * f;
			::memcpy(ptr, m_AnimTransforms[c].transforms[f].data(), dataSize);
		}
	}

	// 리소스 만들기
	vector<D3D11_SUBRESOURCE_DATA> subResources(m_Animations.size());

	// 애니메이션이 3개가 있다고 가정한다면,
	// 1번 애니메이션 부터 2번 애니메이션 직전까지의 모든 데이터들을 배열[0]에
	// 2번 애니메이션 부터 3번 애니메이션 직전까지의 모든 데이터들을 배열[1]에
	// 3번 애니메이션 부터 마지막까지의 모든 데이터들을 배열[3]에 저장시킨다.
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