#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include <array>
#include "MeshGroup.h"
#include "BoneInstance.h"
#include <limits>

/// <summary>
/// ������Ʈ�� �׸��� Ŭ����
/// 2024.1.25. �迹���� ���� �ǵ������ ������ �ͼ� �ٰŸ� ��� �ٲ��. DJ.
/// </summary> 
class MeshGroup;
class BoneInstance;
struct BoneDesc
{
	Matrix transforms[MAX_MODEL_TRANSFORMS];
};

struct AnimTransform
{
	// Bone SRT �迭
	using TransformArrayType = std::array<Matrix, MAX_MODEL_TRANSFORMS>;

	// Keyframe ��ŭ Bone SRT �迭�� ��� �ִ´�. (2���� �迭)
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
	//���� ��
	vector<shared_ptr<NestGraphics::Bone>> m_Bones;
	vector<BoneInstance> m_BonesInsatnce;
	///��Ŀ���
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;
	float currentTime = 0.0f;
	int index;
	BoneDesc m_MatrixPalette;
	vector<AnimTransform> m_AnimTransforms;

	//���� �� ���������
	//vector<shared_ptr<BoneDesc>> m_BoneDesc;
	//BoneInstance m_rootBone;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys);	// assimp��  ������ �������� �޽� �ʱ�ȭ
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);	// ������� ������Ʈ
	virtual void Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);												// ����
	virtual void CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld);																					// ������Ʈ
	virtual void LightUpdate(float m_diffuse, float m_specualr, bool m_useTexture, float m_scale, int m_lightType, NestGraphics::Light m_lightFromGUI);
	// ExampleApp::Update()���� ����
	NestGraphics::VertexConstantBuffer objectVertexConstantData;		   // ������Ʈ ������ ������ ��� ���ؽ� �������
	NestGraphics::PixelShaderConstantBuffer objectPixelConstantData;	   // ������Ʈ ������ ������ ��� �ȼ� �������

	// ExampleApp:Initialize()���� ����
	ComPtr<ID3D11ShaderResourceView> m_pObjectDiffuseResView;			   ///ť����ο� ���ɰ� ����, ���� �̱���.
	ComPtr<ID3D11ShaderResourceView> m_pObjectSpecularResView;			   ///ť����ο� ���ɰ� ����, ���� �̱���.

public:
	std::vector<NestGraphics::MeshData>* getModelMeshData() const { return m_ModelMeshData; };
	void setModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) { m_ModelMeshData = _ModelMeshData; }
	std::string WstringToString(const std::wstring& wstr);
	void UpdateAnimationContainer(int animationIndex);
private:
	///�迹������ �߰��� �Լ�
	void CreateBoneInstance(std::vector<NestGraphics::Bone>& modelBone);

	void UpdateTweenData(float m_dTime);
	void CreateAnimationTransform(unsigned __int32 index);
	void TestCreateTexture(ComPtr<ID3D11Device>& device, const std::string filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
};