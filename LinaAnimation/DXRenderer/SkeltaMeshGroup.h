#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include <array>
#include "MeshGroup.h"
#include "BoneInstance.h"
#include <limits>

/// <summary>
/// 오브젝트를 그리는 클래스
/// 2024.1.25. 김예리나 구조 건들기전에 나한테 와서 근거를 대고 바꿔라. DJ.
/// </summary> 
class MeshGroup;
class BoneInstance;
struct BoneDesc
{
	Matrix transforms[MAX_MODEL_TRANSFORMS];
};

struct AnimTransform
{
	// Bone SRT 배열
	using TransformArrayType = std::array<Matrix, MAX_MODEL_TRANSFORMS>;

	// Keyframe 만큼 Bone SRT 배열을 들고 있는다. (2차원 배열)
	std::array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;
};


class SkeltaMeshGroup : public MeshGroup
{
public:
	SkeltaMeshGroup();
	virtual ~SkeltaMeshGroup();

protected:
	float m_modelScale;
	NestGraphics::TweenDesc m_TweenDesc;
	vector<shared_ptr<NestGraphics::asAnimation>> m_Animations;
	//모델의 본
	vector<shared_ptr<NestGraphics::Bone>> m_Bones;
	vector<BoneInstance> m_BonesInsatnce;
	///행렬연산
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	float currentTime = 0.0f;
	int index;
	BoneDesc m_MatrixPalette;
	vector<AnimTransform> m_AnimTransforms;

	//모델의 본 행렬정보들
	//vector<shared_ptr<BoneDesc>> m_BoneDesc;
	//BoneInstance m_rootBone;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys);	// assimp로  정보를 가져오는 메쉬 초기화
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);	// 상수버퍼 업데이트
	virtual void Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);												// 렌더
	virtual void CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld);																					// 업데이트
	virtual void LightUpdate(float m_diffuse, float m_specualr, bool m_useTexture, float m_scale, int m_lightType, NestGraphics::Light m_lightFromGUI);
	// ExampleApp::Update()에서 접근
	NestGraphics::VertexConstantBuffer objectVertexConstantData;		   // 오브젝트 관련의 정보를 담는 버텍스 상수버퍼
	NestGraphics::PixelShaderConstantBuffer objectPixelConstantData;	   // 오브젝트 관련의 정보를 담는 픽셀 상수버퍼

	// ExampleApp:Initialize()에서 접근
	ComPtr<ID3D11ShaderResourceView> m_pObjectDiffuseResView;			   ///큐브매핑에 사용될것 같음, 아직 미구현.
	ComPtr<ID3D11ShaderResourceView> m_pObjectSpecularResView;			   ///큐브매핑에 사용될것 같음, 아직 미구현.

public:
	std::vector<NestGraphics::MeshData>* getModelMeshData() const { return m_ModelMeshData; };
	void setModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) { m_ModelMeshData = _ModelMeshData; }
	std::string WstringToString(const std::wstring& wstr);
	void UpdateAnimationContainer(int animationIndex);
private:
	///김예리나가 추가한 함수
	void CreateBoneInstance(std::vector<NestGraphics::Bone>& modelBone);

	void UpdateTweenData(float m_dTime);
	void CreateAnimationTransform(unsigned __int32 index);
	void TestCreateTexture(ComPtr<ID3D11Device>& device, const std::string filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
};