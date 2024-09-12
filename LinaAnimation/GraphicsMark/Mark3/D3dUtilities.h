#pragma once
// 이곳의 헤더는 절대 cpp로 옮기지 않는다.
#include <wrl/client.h> // ComPtr
#include <vector>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <directxtk/DDSTextureLoader.h> // 큐브맵 읽을 때 필요
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <iostream>

using Microsoft::WRL::ComPtr;
using std::vector;
using std::wstring;

/// <summary>
/// D3D에서 자주 사용하는 함수들의 묶음
/// 2024.1.25. uint32_t로 전부 바꿨다. 오류 체크하자.
/// </summary>
class D3dUtilities
{
public:
	// 깊이 버퍼를 만드는 함수
	bool CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, 
						   ComPtr<ID3D11DepthStencilView>& depthStencilView);
	// 버텍스 쉐이더와 인풋 레이아웃을 만드는 함수 : 템플릿에 상세 설명 적혀있음(ctrl+마우스 왼쪽)
	void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
										  ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout);
	// 픽셀 쉐이더를 만드는 함수
	void CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader);
	// 인덱스 버퍼를 만드는 함수
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer);
	// 텍스처를 만드는 함수
	void CreateTexture(ComPtr<ID3D11Device>& device, const std::string filename,
					   ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
	// 큐브 매핑을 만드는 함수
	void CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView);
	// 디버깅용 함수 (셰이더 컴파일이나 D3D API 호출에서 발생하는 오류를 체크하고, 오류가 발생했을 경우 오류 메시지를 출력)
	void CheckResult(HRESULT hr, ID3DBlob* errorBlob);

public:
	template <typename T_VERTEX>																									   ///아래 함수 참조!
	void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // 초기화 후 변경하지 않음을 설정함
		bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
		bufferDesc.StructureByteStride = sizeof(T_VERTEX);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // MS 예제에서 초기화하는 방식
		vertexBufferData.pSysMem = vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		const HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
		if (FAILED(hr)) { std::cout << "CreateBuffer() failed. " << std::hex << hr << std::endl; };
	}

	template <typename T_CONSTANT>
	// CreateConstantBuffer(디바이스, 오브젝트 버퍼 주소, 초기 할당 버퍼 주소) // 오브젝트 버퍼의 데이터를 초기 할당 버퍼에 전달해서 초기 할당 버퍼를 생성!
	void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
	{
		D3D11_BUFFER_DESC cbDesc; // 버퍼의 상태를 설명
		cbDesc.ByteWidth = sizeof(constantBufferData);  // 버퍼의 크기(바이트)
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;				// 초기화 후 변경 가능 (상수버퍼 값이 달라짐)
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  //버퍼가 파이프라인에 바인딩되는 방법을 식별 : 버퍼를 셰이더 단계에 상수 버퍼로 바인딩
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU 접근 플래스 : CPU는 상수 버퍼에는 접근 가능해야 함
		cbDesc.MiscFlags = 0;							// 기타 플래그 : 미사용시 0
		cbDesc.StructureByteStride = 0;					// 버퍼가 구조화된 버퍼를 나타내는 경우 버퍼 구조의 각 요소 크기(바이트)

		D3D11_SUBRESOURCE_DATA initData;				// 하위 리소스를 초기화하기 위한 데이터 지정
		initData.pSysMem = &constantBufferData;			// 초기화 데이터에 대한 포인터
		initData.SysMemPitch = 0;						// 3D 텍스처의 2D 조각 1개에서 SysMemSlicePitch 멤버의 해당 텍스처에 있는 다음 2D 조각의 첫 번째 픽셀까지의 거리를 지정
		initData.SysMemSlicePitch = 0;					// 한 깊이 수준의 시작부터 다음 수준까지의 거리(바이트)

		// CreateBuffer(버퍼를 상태를 설명, 초기화 데이터를 설명, 초기 할당 버퍼 상태 개체 주소 포인터)
		HRESULT hr = device->CreateBuffer(&cbDesc, &initData, constantBuffer.GetAddressOf()); // 버퍼(꼭짓점 버퍼, 인덱스 버퍼 또는 셰이더 상수 버퍼)를 만든다.

		if (FAILED(hr)) { std::cout << "CreateConstantBuffer() CreateBuffer failed()." << std::endl; } // 만들기에 실패하면 디버깅 창에 뜬다.
	}

	// 버퍼를 업데이트하는 함수
	template <typename T_DATA>
	// (디바이스, 컨텍스트(버퍼를 매칭하고 언매핑함), 업데이트할 데이터가 포함된 버퍼, 업데이트할 버퍼에 대한 참조 버퍼(초기 할당 버퍼))
	void UpdateBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context,
		const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer)
	{
		if (!buffer) { std::cout << "UpdateBuffer() buffer was not initialized." << std::endl; } // 초기 버퍼 없으면 오류 메세지

		D3D11_MAPPED_SUBRESOURCE ms;											// 매핑된 서브 리소스를 나타내는 구조체를 선언
		context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// 버퍼를 매핑하여 데이터를 쓸 수 있도록 준비 : 이전 내용을 무시하고 쓰기 작업을 수행
		memcpy(ms.pData, &bufferData, sizeof(bufferData));						// 매핑된 서브 리소스에 새로운 데이터를 복사
		context->Unmap(buffer.Get(), NULL);										// 버퍼를 언매핑하여 GPU에 업데이트된 데이터를 전달
	}
};

