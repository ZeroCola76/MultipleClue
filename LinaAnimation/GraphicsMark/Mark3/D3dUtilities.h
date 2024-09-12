#pragma once
// �̰��� ����� ���� cpp�� �ű��� �ʴ´�.
#include <wrl/client.h> // ComPtr
#include <vector>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <directxtk/DDSTextureLoader.h> // ť��� ���� �� �ʿ�
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <iostream>

using Microsoft::WRL::ComPtr;
using std::vector;
using std::wstring;

/// <summary>
/// D3D���� ���� ����ϴ� �Լ����� ����
/// 2024.1.25. uint32_t�� ���� �ٲ��. ���� üũ����.
/// </summary>
class D3dUtilities
{
public:
	// ���� ���۸� ����� �Լ�
	bool CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, 
						   ComPtr<ID3D11DepthStencilView>& depthStencilView);
	// ���ؽ� ���̴��� ��ǲ ���̾ƿ��� ����� �Լ� : ���ø��� �� ���� ��������(ctrl+���콺 ����)
	void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
										  ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout);
	// �ȼ� ���̴��� ����� �Լ�
	void CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader);
	// �ε��� ���۸� ����� �Լ�
	void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer);
	// �ؽ�ó�� ����� �Լ�
	void CreateTexture(ComPtr<ID3D11Device>& device, const std::string filename,
					   ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView);
	// ť�� ������ ����� �Լ�
	void CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView);
	// ������ �Լ� (���̴� �������̳� D3D API ȣ�⿡�� �߻��ϴ� ������ üũ�ϰ�, ������ �߻����� ��� ���� �޽����� ���)
	void CheckResult(HRESULT hr, ID3DBlob* errorBlob);

public:
	template <typename T_VERTEX>																									   ///�Ʒ� �Լ� ����!
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
	// CreateConstantBuffer(����̽�, ������Ʈ ���� �ּ�, �ʱ� �Ҵ� ���� �ּ�) // ������Ʈ ������ �����͸� �ʱ� �Ҵ� ���ۿ� �����ؼ� �ʱ� �Ҵ� ���۸� ����!
	void CreateConstantBuffer(ComPtr<ID3D11Device>& device, const T_CONSTANT& constantBufferData, ComPtr<ID3D11Buffer>& constantBuffer)
	{
		D3D11_BUFFER_DESC cbDesc; // ������ ���¸� ����
		cbDesc.ByteWidth = sizeof(constantBufferData);  // ������ ũ��(����Ʈ)
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;				// �ʱ�ȭ �� ���� ���� (������� ���� �޶���)
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;  //���۰� ���������ο� ���ε��Ǵ� ����� �ĺ� : ���۸� ���̴� �ܰ迡 ��� ���۷� ���ε�
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU ���� �÷��� : CPU�� ��� ���ۿ��� ���� �����ؾ� ��
		cbDesc.MiscFlags = 0;							// ��Ÿ �÷��� : �̻��� 0
		cbDesc.StructureByteStride = 0;					// ���۰� ����ȭ�� ���۸� ��Ÿ���� ��� ���� ������ �� ��� ũ��(����Ʈ)

		D3D11_SUBRESOURCE_DATA initData;				// ���� ���ҽ��� �ʱ�ȭ�ϱ� ���� ������ ����
		initData.pSysMem = &constantBufferData;			// �ʱ�ȭ �����Ϳ� ���� ������
		initData.SysMemPitch = 0;						// 3D �ؽ�ó�� 2D ���� 1������ SysMemSlicePitch ����� �ش� �ؽ�ó�� �ִ� ���� 2D ������ ù ��° �ȼ������� �Ÿ��� ����
		initData.SysMemSlicePitch = 0;					// �� ���� ������ ���ۺ��� ���� ���ر����� �Ÿ�(����Ʈ)

		// CreateBuffer(���۸� ���¸� ����, �ʱ�ȭ �����͸� ����, �ʱ� �Ҵ� ���� ���� ��ü �ּ� ������)
		HRESULT hr = device->CreateBuffer(&cbDesc, &initData, constantBuffer.GetAddressOf()); // ����(������ ����, �ε��� ���� �Ǵ� ���̴� ��� ����)�� �����.

		if (FAILED(hr)) { std::cout << "CreateConstantBuffer() CreateBuffer failed()." << std::endl; } // ����⿡ �����ϸ� ����� â�� ���.
	}

	// ���۸� ������Ʈ�ϴ� �Լ�
	template <typename T_DATA>
	// (����̽�, ���ؽ�Ʈ(���۸� ��Ī�ϰ� �������), ������Ʈ�� �����Ͱ� ���Ե� ����, ������Ʈ�� ���ۿ� ���� ���� ����(�ʱ� �Ҵ� ����))
	void UpdateBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context,
		const T_DATA& bufferData, ComPtr<ID3D11Buffer>& buffer)
	{
		if (!buffer) { std::cout << "UpdateBuffer() buffer was not initialized." << std::endl; } // �ʱ� ���� ������ ���� �޼���

		D3D11_MAPPED_SUBRESOURCE ms;											// ���ε� ���� ���ҽ��� ��Ÿ���� ����ü�� ����
		context->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// ���۸� �����Ͽ� �����͸� �� �� �ֵ��� �غ� : ���� ������ �����ϰ� ���� �۾��� ����
		memcpy(ms.pData, &bufferData, sizeof(bufferData));						// ���ε� ���� ���ҽ��� ���ο� �����͸� ����
		context->Unmap(buffer.Get(), NULL);										// ���۸� ������Ͽ� GPU�� ������Ʈ�� �����͸� ����
	}
};

