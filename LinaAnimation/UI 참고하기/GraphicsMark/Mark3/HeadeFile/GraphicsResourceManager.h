#pragma once
#include "DXDefine.h"
#include <d3d11.h>
/// <summary>
/// 그래픽스에서 공통으로 쓰는 리소스 함수를 그냥 모아둔 거다.
/// 일일히 코드를 다 치는건 이상한 짓이다. 템플릿을 써보자. 
/// </summary>
class GraphicsResourceManager
{
	ComPtr<ID3D11Device> m_pD3dDevice;
	ComPtr<ID3D11DeviceContext> m_pD3dContext;
public:
	GraphicsResourceManager(ComPtr<ID3D11Device> pD3dDevice, ComPtr<ID3D11DeviceContext> pD3dContext);
	~GraphicsResourceManager();

	/// <summary>
   /// template 구현 부분
   /// </summary>
   /// <typeparam name="T_VERTEX"></typeparam>
   /// <param name="vertices"></param>
   /// <param name="vertexBuffer"></param>
	template <typename T_VERTEX>
	void CreateVertexBuffer(const vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer) {

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

		const HRESULT hr =
			m_pD3dDevice->CreateBuffer(&bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
		if (FAILED(hr)) {
			std::cout << "CreateBuffer() failed. " << std::hex << hr << std::endl;
		};
	}

	template <typename T_CONSTANT>
	void CreateConstantBuffer(const T_CONSTANT& constantBufferData,
		ComPtr<ID3D11Buffer>& constantBuffer) {
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(constantBufferData);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC; // 초기화 후 변경 가능 (상수버퍼 값이 달라짐)
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU는 상수 버퍼에는 접근 가능해야 함
		cbDesc.MiscFlags = 0;
		cbDesc.StructureByteStride = 0;

		// Fill in the subresource data.
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &constantBufferData;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		m_pD3dDevice->CreateBuffer(&cbDesc, &InitData, constantBuffer.GetAddressOf());
	}

	template <typename T_DATA>
	void UpdateBuffer(const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer) {
		D3D11_MAPPED_SUBRESOURCE ms;
		m_pD3dContext->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, &bufferData, sizeof(bufferData));
		m_pD3dContext->Unmap(buffer.Get(), NULL);
	}


};

