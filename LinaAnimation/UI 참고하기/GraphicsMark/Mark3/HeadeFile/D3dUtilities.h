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
/// D3D���� ���� ����ϴ� �Լ����� ����
/// 2024.1.24. ���� ���� ��
/// </summary>
class D3dUtilities
{
private:
	ComPtr<ID3D11Texture2D> m_pD3dUtilDepthStencilBuffer;

public:
	// ���� ���۸� ����� �Լ�
	static bool CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, ComPtr<ID3D11DepthStencilView>& depthStencilView);
	// ���ؽ� ���̴��� ��ǲ ���̾ƿ��� ����� �Լ� : ���ø��� �� ���� ��������(ctrl+���콺 ����)
	void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
										  ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout);
	// �ȼ� ���̴��� ����� �Լ�
	static void CreatePixelShader(ComPtr<ID3D11Device>& device,	const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader);
	// �ε��� ���۸� ����� �Լ�
	static void CreateIndexBuffer(ComPtr<ID3D11Device>& device,	const vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer);
	// �ؽ�ó�� ����� �Լ�
	static void	CreateTexture(ComPtr<ID3D11Device>& device, const std::string filename,
							ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
	// ť�� ������ ����� �Լ�
	static void	CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView);
	// ������ �Լ� (���̴� �������̳� D3D API ȣ�⿡�� �߻��ϴ� ������ üũ�ϰ�, ������ �߻����� ��� ���� �޽����� ���)
	static void CheckResult(HRESULT hr, ID3DBlob* errorBlob);

public:
	template <typename T_VERTEX>
	void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<T_VERTEX>& vertices, ComPtr<ID3D11Buffer>& vertexBuffer)
	{
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

		const HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
		if (FAILED(hr)) { std::cout << "CreateBuffer() failed. " << std::hex << hr << std::endl; };
	}

	template <typename T_CONSTANT>
	void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
	{
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.ByteWidth = sizeof(constantBufferData);
		cbDesc.Usage = D3D11_USAGE_DYNAMIC; // �ʱ�ȭ �� ���� ���� (������� ���� �޶���)
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU�� ��� ���ۿ��� ���� �����ؾ� ��
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

