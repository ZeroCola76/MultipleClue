#include "D3dUtilities.h"
#include <directxtk/DDSTextureLoader.h> // ť��� ���� �� �ʿ�
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool D3dUtilities::CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, ComPtr<ID3D11DepthStencilView>& depthStencilView)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
	depthStencilBufferDesc.Width = screenWidth;
	depthStencilBufferDesc.Height = screenHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (numQualityLevels > 0)
	{
		depthStencilBufferDesc.SampleDesc.Count = 4; // how many multisamples
		depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;
	}
	else
	{
		depthStencilBufferDesc.SampleDesc.Count = 1; // how many multisamples
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;

	if (FAILED(device->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf())))
	{
		std::cout << "CreateTexture2D() failed." << std::endl;
	}
	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf())))
	{
		std::cout << "CreateDepthStencilView() failed." << std::endl;
	}
	return true;
}
/// <summary>
/// ���ؽ��� ����ü ������ �ʿ�� ��
/// </summary>
/// <param name="filename">hlsl ����</param>
/// <param name="inputElements">�Է� ���̾ƿ� ���� �ۼ��� ��</param>
/// <param name="vertexShader">���ؽ� ������ �ּ�</param>
/// <param name="inputLayout">��ǲ ���̾ƿ� �ּ� ����</param>
void D3dUtilities::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob; // �����ϵ� ���̴� �ڵ带 �����ϱ� ���� �������̽�
	ComPtr<ID3DBlob> errorBlob; // ���̴� ������ �� �߻��� ���� �޽����� �����ϱ� ���� �������̽�

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	// ����: ���̴��� �������� �̸��� "main"�� �Լ��� ����
	// D3D_COMPILE_STANDARD_FILE_INCLUDE �߰�: ���̴����� include ���
	HRESULT hr =
		// HLSL �ڵ带 ������ ��� ���� ����Ʈ�ڵ�� ������
		// ���� �̸�, ��ũ�� ��� ����, ���̴����� include ����ϴ���, ���̴� ������ �̸�, ���̴� ����, �÷���, 0(���̴� ���� 0���� ����), shaderBlob, errorBlob
		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
			compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	CheckResult(hr, errorBlob.Get());

	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		NULL, &vertexShader);

	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);
}

// ���� ���� CreateVertexShaderAndInputLayout()�� ������ ����. ���ؽ��� �ȼз� �����ϸ� �ȴ�.
void D3dUtilities::CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& filename, ComPtr<ID3D11PixelShader>& pixelShader)
{
	ComPtr<ID3DBlob> shaderBlob;
	ComPtr<ID3DBlob> errorBlob;

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr =
		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0",
			compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	CheckResult(hr, errorBlob.Get());

	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
		NULL, pixelShader.GetAddressOf());
}

void D3dUtilities::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint16_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
	bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
	bufferDesc.StructureByteStride = sizeof(uint16_t);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	device->CreateBuffer(&bufferDesc, &indexBufferData, indexBuffer.GetAddressOf());
}

/// <summary>
/// �ؽ�ó�� �����ϴ� �Լ�
/// </summary>
/// <param name="filename">���� ��θ� ������ �̸�</param>
/// <param name="m_texture">������ �ؽ�ó�� �ּ� ����</param>
/// <param name="texResourceView">�ؽ�ó�� �̹��� ��</param>
void D3dUtilities::CreateTexture(ComPtr<ID3D11Device>& device, const std::string filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{
	int width(0), height(0), channels(0);
	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	std::vector<uint8_t> image;
	image.resize(width * height * 4);

	for (size_t i = 0; i < width * height; i++)
	{
		for (size_t c = 0; c < 3; c++)
		{
			image[4 * i + c] = img[i * channels + c];
		}
		image[4 * i + 3] = 255;
	}

	// Create texture.
	D3D11_TEXTURE2D_DESC texDesc = {};
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = image.data();
	InitData.SysMemPitch = texDesc.Width * sizeof(uint8_t) * 4;

	device->CreateTexture2D(&texDesc, &InitData, texture.GetAddressOf());
	device->CreateShaderResourceView(texture.Get(), nullptr, textureResourceView.GetAddressOf());
}

void D3dUtilities::CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& texResView)
{
	ComPtr<ID3D11Texture2D> texture;
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(device.Get(), filename, 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0,
		D3D11_RESOURCE_MISC_TEXTURECUBE, DirectX::DX11::DDS_LOADER_FLAGS(false),
		(ID3D11Resource**)texture.GetAddressOf(), texResView.GetAddressOf(), nullptr);

	if (FAILED(hr))
	{
		std::cout << "CreateDDSTextureFromFileEx() failed" << std::endl;
	}
}

void D3dUtilities::CheckResult(HRESULT hr, ID3DBlob* errorBlob)
{
	if (FAILED(hr))
	{
		// ������ ���� ���
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) { std::cout << "File not found." << std::endl; }
		if (errorBlob) // ���� �޽����� ������ ���
		{
			std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}
}
