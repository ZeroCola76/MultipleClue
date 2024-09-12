#include <initializer_list>
#include "MeshGroup.h"
#include <stb_image.h>

MeshGroup::MeshGroup()
{
	d3dUtilities = new D3dUtilities();
}

MeshGroup::~MeshGroup()
{
	delete d3dUtilities;
}

void MeshGroup::Initialize(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, NestGraphics::ModelData* modelMeshDataes, Matrix _trsMatrix, std::initializer_list<std::wstring> textureFileKeys)
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
 		{"BLEND_INDICES", 0 ,DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,D3D11_INPUT_PER_VERTEX_DATA,0},
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
	std::cout << sizeof(a)<<std::endl;
}

// 상수 버퍼를 업데이트 하는 함수(디바이스, 렌더링 명령을 생성하는 디바이스 컨텍스트)
void MeshGroup::UpdateConstantBuffers(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	d3dUtilities->UpdateBuffer(device, context, objectVertexConstantData, m_pVertexConstantBuffer); // 버텍스 상수 버퍼 데이터 값을 토대로 버퍼를 업데이트 한다.
	d3dUtilities->UpdateBuffer(device, context, objectPixelConstantData, m_pPixelConstantBuffer);	// 노멀의 픽셸 값은 언제나 업데이트 된다.

}

/// <summary>
/// 메쉬를 랜더하는 함수
/// </summary>
/// <param name="context">랜더링을 제어하는 컨텍스트</param>
void MeshGroup::Render(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context) /// 여기에 int로 SRV 개수를 매개변수로 받아와야 하게 될 것..
{
	// 버텍스/인덱스 버퍼 설정
	UINT stride = sizeof(NestGraphics::Vertex); //버텍스의 각 요소 사이의 간격을 나타내는 변수입니다. (버텍스 버퍼에서 다음 버텍스로 이동할 때 사용)
	UINT offset = 0;							//버텍스 버퍼의 시작 위치에서의 오프셋

	// 버텍스 쉐이더를 설정한다
	context->VSSetShader(m_pVertexShader.Get(), 0, 0);				// 버텍스 쉐이더 설정
	context->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());	// 픽셀 샘플러 설정
	context->PSSetShader(m_pPixelShader.Get(), 0, 0);				// 픽셀 쉐이더 설정

	// 버텍스/인덱스 버퍼 설정
	for (const auto& meshes : m_pMeshes)
	{

		ID3D11ShaderResourceView* resViews[1] =												// 픽셀 쉐이더에서 사용할 셰이더 리소스 뷰를 설정(텍스처 개수만큼 넣는다.)
		{ meshes->textureResourceView.Get()};
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


void MeshGroup::CamUpdate(float m_dTime, ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context
	, Matrix camViewRow, Matrix camProjRow, Vector3 camEyeWorld)
{
	//위치관련 업뎃
	objectPixelConstantData.useTexture = true;
	objectPixelConstantData.eyeWorld = camEyeWorld;
	objectVertexConstantData.view = camViewRow.Transpose();
	objectVertexConstantData.projection = camProjRow.Transpose();

	UpdateConstantBuffers(device, context);
}

void MeshGroup::LightUpdate(float m_diffuse, float m_specualr, bool m_useTexture, float m_norScale, int m_lightType, NestGraphics::Light m_lightFromGUI)
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

std::string MeshGroup::WstringToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}