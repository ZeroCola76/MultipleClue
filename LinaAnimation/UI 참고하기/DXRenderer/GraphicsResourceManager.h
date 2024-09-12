#pragma once
#include "DXDefine.h"
#include <d3d11.h>
/// <summary>
/// �׷��Ƚ����� �������� ���� ���ҽ� �Լ��� �׳� ��Ƶ� �Ŵ�.
/// ������ �ڵ带 �� ġ�°� �̻��� ���̴�. ���ø��� �Ẹ��. 
/// </summary>
class GraphicsResourceManager
{
	ComPtr<ID3D11Device> m_pD3dDevice;
	ComPtr<ID3D11DeviceContext> m_pD3dContext;
public:
	GraphicsResourceManager(ComPtr<ID3D11Device> pD3dDevice, ComPtr<ID3D11DeviceContext> pD3dContext);
	~GraphicsResourceManager();

	/// <summary>
   /// template ���� �κ�
   /// </summary>
   /// <typeparam name="T_VERTEX"></typeparam>
   /// <param name="vertices"></param>
   /// <param name="vertexBuffer"></param>
	template <typename T_VERTEX>
	void CreateVertexBuffer(const vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer) {

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� �������� ������ ������
		bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * vertices.size());
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
		bufferDesc.StructureByteStride = sizeof(T_VERTEX);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // MS �������� �ʱ�ȭ�ϴ� ���
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
		cbDesc.Usage = D3D11_USAGE_DYNAMIC; // �ʱ�ȭ �� ���� ���� (������� ���� �޶���)
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�� ��� ���ۿ��� ���� �����ؾ� ��
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

