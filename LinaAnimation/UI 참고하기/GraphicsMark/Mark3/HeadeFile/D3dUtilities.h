#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <windows.h>
#include <wrl/client.h> // ComPtr

using Microsoft::WRL::ComPtr;
using std::shared_ptr;
using std::vector;
using std::wstring;

/// <summary>
/// D3D에서 자주 사용하는 함수들의 묶음
/// 2024.1.24. 순서 정리 끝
/// </summary>
class D3dUtilities
{
private:
	ComPtr<ID3D11Texture2D> m_pD3dUtilDepthStencilBuffer;

public:
	// 깊이 버퍼를 만드는 함수
	static bool CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, ComPtr<ID3D11DepthStencilView>& depthStencilView);
	// 버텍스 쉐이더와 인풋 레이아웃을 만드는 함수 : 템플릿에 상세 설명 적혀있음(ctrl+마우스 왼쪽)
	void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
										  ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout);
	// 픽셀 쉐이더를 만드는 함수
	static void CreatePixelShader(ComPtr<ID3D11Device>& device,	const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader);
	// 인덱스 버퍼를 만드는 함수
	static void CreateIndexBuffer(ComPtr<ID3D11Device>& device,	const vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer);
	// 텍스처를 만드는 함수
	static void	CreateTexture(ComPtr<ID3D11Device>& device, const std::string filename,
							ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
	// 큐브 매핑을 만드는 함수
	static void	CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView);
	// 디버깅용 함수 (셰이더 컴파일이나 D3D API 호출에서 발생하는 오류를 체크하고, 오류가 발생했을 경우 오류 메시지를 출력)
	static void CheckResult(HRESULT hr, ID3DBlob* errorBlob);

public:
	template <typename T_VERTEX>
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
	void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
	{
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(constantBufferData);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC; // 초기화 후 변경 가능 (상수버퍼 값이 달라짐)
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU는 상수 버퍼에는 접근 가능해야 함
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = &constantBufferData;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		auto hr = device->CreateBuffer(&cbDesc, &initData, constantBuffer.GetAddressOf());

		if (FAILED(hr)) { std::cout << "CreateConstantBuffer() CreateBuffer failed()." << std::endl; }
	}

	template <typename T_DATA>
	void UpdateBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context,
		const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer)
	{
		if (!buffer) { std::cout << "UpdateBuffer() buffer was not initialized." << std::endl; }

		D3D11_MAPPED_SUBRESOURCE ms;
		context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &bufferData, sizeof(bufferData));
		context->Unmap(buffer.Get(), NULL);
	}
};

