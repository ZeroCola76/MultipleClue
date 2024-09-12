#pragma once

#include "Geometry.h"
#include "ConstantBuffer.h"
#include "D3dUtilities.h"
#include <array>

/// <summary>
/// 오브젝트를 그리는 클래스
/// 2024.1.25. 김예리나 구조 건들기전에 나한테 와서 근거를 대고 바꿔라. DJ.
/// </summary> 

class MeshGroup
{
public:
	MeshGroup();
	virtual ~MeshGroup();

protected:
	// 하나의 3D 모델이 내부적으로는 여러개의 메쉬로 구성
	std::vector<std::shared_ptr<NestGraphics::Mesh>> m_pMeshes; // 오브젝트 메쉬의 구조체를 가리키는 포인터
	// 매쉬 데이터를 받아오기 위한 포인터
	std::vector<NestGraphics::MeshData>* m_ModelMeshData;
	std::wstring m_modelFilePath;
	float m_modelScale;
	NestGraphics::ModelData* m_pModelData;	// shared-ptr로 변경해야함


	// 오브젝트 쉐이더를 저장하는 인터페이스
	ComPtr<ID3D11VertexShader> m_pVertexShader;					// 정점 셰이더
	ComPtr<ID3D11PixelShader> m_pPixelShader;					// 픽셸 셰이더
	ComPtr<ID3D11InputLayout> m_pInputLayout;					// 입력 레이아웃, 정점 셰이더에 입력되는 데이터의 형식을 정의

	// 오브젝트의 샘플러
	ComPtr<ID3D11SamplerState> m_pSamplerState;

	/// 지역변수화 고려.
	// 오브젝트의 상수 버퍼
	ComPtr<ID3D11Buffer> m_pVertexConstantBuffer;				// 오브젝트 버텍스 상수버퍼의 할당을 위해 한번만 쓰이는 변수
	ComPtr<ID3D11Buffer> m_pPixelConstantBuffer;				// 오브젝트 픽셀 상수버퍼의 할당을 위해 한번만 쓰이는 변수

	// 자주 사용하는 함수들의 묶음을 사용
	D3dUtilities* d3dUtilities;

public:
	virtual void Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys);	// assimp로  정보를 가져오는 메쉬 초기화
	virtual void UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);	// 상수버퍼 업데이트
	virtual void Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);												// 렌더
	virtual void CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
		, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld);																					// 업데이트
	virtual void LightUpdate(float m_diffuse,float m_specualr, bool m_useTexture, float m_scale, int m_lightType, NestGraphics::Light m_lightFromGUI);
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

};