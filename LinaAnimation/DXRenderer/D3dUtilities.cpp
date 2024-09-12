#include "D3dUtilities.h"
#include <directxtk/DDSTextureLoader.h> // ť��� ���� �� �ʿ�
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/// <summary>
/// // ���� ���۸� ����� �Լ�
/// </summary>
/// <param name="device">����̽�</param>
/// <param name="screenWidth">��ũ�� �ʺ�</param>
/// <param name="screenHeight">��ũ�� ����</param>
/// <param name="numQualityLevels">������ ���� ������ ǰ�� ������ ��Ÿ���� ����</param>
/// <param name="depthStencilView">������ ���� ���ٽ� �� ������</param>
/// <returns></returns>
bool D3dUtilities::CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, ComPtr<ID3D11DepthStencilView>& depthStencilView)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;		// ���� ���ٽ� ���� Ư�� ����
	depthStencilBufferDesc.Width = screenWidth;			// ��ũ�� �ʺ� ����
	depthStencilBufferDesc.Height = screenHeight;		// ��ũ�� ���� ����
	depthStencilBufferDesc.MipLevels = 1;				// MipLevel �˻� �� ����, 1 : �̼���
	depthStencilBufferDesc.ArraySize = 1;				// ArraySize �˻� �� ����, 1 : �̼���
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ���� ���ٽ� ���� ���� ���� : 24��Ʈ ���̿� 8��Ʈ ���ٽ�
	if (numQualityLevels > 0)							// ��Ƽ ���ø��� �����Ǵ� ��� ���� ���ٽ� ���
	{
		depthStencilBufferDesc.SampleDesc.Count = 4;						// ��Ƽ ���ø� ����
		depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;	// ��Ƽ ���ø� ǰ�� ���� ����
	}
	else
	{
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// ���� ���ٽ� ������ ��� ����� ���� : �⺻ ����
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// ���� ���ٽ� ������ ���ε� �÷��� ���� : ���� ���ٽ� �ڿ����� ���ε���
	depthStencilBufferDesc.CPUAccessFlags = 0;						// CPU������ �׼��� �÷��׸� ���� : ������ �Ұ�
	depthStencilBufferDesc.MiscFlags = 0;							// ��Ÿ �÷��׸� ���� : ���� ����

	ComPtr<ID3D11Texture2D> depthStencilBuffer;			// ���� ���ٽ� ���� ���� ����

	// �����
	if (FAILED(device->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf()))) // ���� ���ٽ� ���۸� �����ϰ�, �̸� depthStencilBuffer�� ����
	{
		std::cout << "CreateTexture2D() failed." << std::endl;
	}
	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf()))) // ���� ���ٽ� ���ۿ� ���� ���� ���ٽ� �並 �����ϰ�, �̸� depthStencilView�� ����
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
void D3dUtilities::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, 
													const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, 
													ComPtr<ID3D11VertexShader>& vertexShader, 
													ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob; // �����ϵ� ���̴� �ڵ带 �����ϱ� ���� �������̽�
	ComPtr<ID3DBlob> errorBlob; // ���̴� ������ �� �߻��� ���� �޽����� �����ϱ� ���� �������̽�

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	///TO DO : �и��ϴ� ���� �ʿ������� ���⼭�� �ϴ� �������� �Ѵ�.
	// ����: ���̴��� �������� �̸��� "main"�� �Լ��� ����
	// D3D_COMPILE_STANDARD_FILE_INCLUDE �߰�: ���̴����� include ���
	HRESULT hr =
		// HLSL �ڵ带 ������ ��� ���� ����Ʈ�ڵ�� ������
		// ���� �̸�, ��ũ�� ��� ����, ���̴����� include ����ϴ���, ���̴� ������ �̸�, ���̴� ����, �÷���, 0(���̴� ���� 0���� ����), shaderBlob, errorBlob
		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Animation_Main", "vs_5_0",
			compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	CheckResult(hr, errorBlob.Get());

	// �Է�-����� �ܰ迡 ���� �Է� ���� �����͸� �����ϴ� �Է� ���̾ƿ� ��ü�� �����. 
	// (�Է� ����� �ܰ� �Է� ������ ������ �迭, input-elements �迭�� �Է� ������ ���� ��, �����ϵ� ���̴��� ���� ������, �����ϵ� ���̴��� ũ��, ������ �Է� ���̾ƿ� ��ü�� ���� ������)
	///���� �̻�..?
	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

	// �����ϵ� ���̴����� ������ ���̴� ��ü�� �����. (�����ϵ� ���̴��� ���� ������, �����ϵ� ������ ���̴��� ũ��, Ŭ���� ���� �������̽��� ���� ������, ���ؽ� ���̴� �ּ�)
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),	NULL, &vertexShader);
}

void D3dUtilities::CreateVertexShaderAndInputLayoutNor(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob; // �����ϵ� ���̴� �ڵ带 �����ϱ� ���� �������̽�
	ComPtr<ID3DBlob> errorBlob; // ���̴� ������ �� �߻��� ���� �޽����� �����ϱ� ���� �������̽�

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	///TO DO : �и��ϴ� ���� �ʿ������� ���⼭�� �ϴ� �������� �Ѵ�.
	// ����: ���̴��� �������� �̸��� "main"�� �Լ��� ����
	// D3D_COMPILE_STANDARD_FILE_INCLUDE �߰�: ���̴����� include ���
	HRESULT hr =
		// HLSL �ڵ带 ������ ��� ���� ����Ʈ�ڵ�� ������
		// ���� �̸�, ��ũ�� ��� ����, ���̴����� include ����ϴ���, ���̴� ������ �̸�, ���̴� ����, �÷���, 0(���̴� ���� 0���� ����), shaderBlob, errorBlob
		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
			compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	CheckResult(hr, errorBlob.Get());

	// �Է�-����� �ܰ迡 ���� �Է� ���� �����͸� �����ϴ� �Է� ���̾ƿ� ��ü�� �����. 
	// (�Է� ����� �ܰ� �Է� ������ ������ �迭, input-elements �迭�� �Է� ������ ���� ��, �����ϵ� ���̴��� ���� ������, �����ϵ� ���̴��� ũ��, ������ �Է� ���̾ƿ� ��ü�� ���� ������)
	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

	// �����ϵ� ���̴����� ������ ���̴� ��ü�� �����. (�����ϵ� ���̴��� ���� ������, �����ϵ� ������ ���̴��� ũ��, Ŭ���� ���� �������̽��� ���� ������, ���ؽ� ���̴� �ּ�)
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

// �ٷ� ���� CreateVertexShaderAndInputLayout()�� ������ ����. ���ؽ��� �ȼз� �����ϸ� �ȴ�.
// �Ϲ������� �ȼ� ���̴��� ���ؽ� ���̴��� �Ŀ� �����ǹǷ�, �Է� ���̾ƿ� ��ü�� ���� �� ������ �ʴ´�.
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

	// �����ϵ� ���̴����� �ȼ� ���̴��� ���� ��ü�� �����. (�����ϵ� ���̴��� ���� ������, �����ϵ� �ȼ� ���̴��� ũ��, Ŭ���� ���� �������̽��� ���� ������, �ȼ� ���̴� �ּ�)
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, pixelShader.GetAddressOf());
}

// ������Ʈ ���� ������ �ε��� �����͸� �ʱ� �ε��� �Ҵ� ���ۿ� ���� �� �ε��� �����͸� �������� �ʱ� �ε��� �Ҵ� ���۸� ����!
void D3dUtilities::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
	//�ε��� ������ ������ ����
	D3D11_BUFFER_DESC bufferDesc = {};								 // ����Ʈ ������ �ʱ�ȭ
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;						 // �ʱ�ȭ �� ����X
	bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());	 // ����Ʈ ũ�� ���� (�ε��� * uint32_t)
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					 // ���ε� �÷��� : ���۸� �ε��� ���۷� �Է� ����� �ܰ迡 ���ε�
	bufferDesc.CPUAccessFlags = 0;									 // 0 : no CPU access is necessary.
	bufferDesc.StructureByteStride = sizeof(uint32_t);				 // ����ü�� ����Ʈ ���� �Ÿ� : 4����Ʈ

	// �ε��� ���� �����͸� �ʱ�ȭ�ϴ� ���긮�ҽ� ����ü�� ����
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };					 // ����Ʈ ������ �ʱ�ȭ
	indexBufferData.pSysMem = indices.data();						 // �ý��� �޸𸮿� �ִ� ������ ������
	indexBufferData.SysMemPitch = 0;								 // �� ������ ����Ʈ ���� �Ÿ�
	indexBufferData.SysMemSlicePitch = 0;							 // �����̽� ������ ����Ʈ ���� �Ÿ�
	// cf. �� : 2D �̹����� ���� ����, �����̽� : 3D �ؽ�ó���� ���� 2D �̹���

	// ����̽��� ����Ͽ� �ε��� ���۸� ���� (�ε��� ���� ���� �ּ�, �ε��� ���� ������, ���� �ε��� ������ ������)
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

	//RGB ������ �̹����� RGBA �������� ��ȯ. stb_image.h�� ����Ͽ� �̹����� �ε��ϰ�, ��ȯ�� �����͸� std::vector<uint8_t>�� ����
	int width(0), height(0), channels(0);												// �̹����� ��(width), ����(height), ä�� ��(channels)�� ��Ÿ���� ������ �����ϰ� �ʱⰪ�� 0���� ����
	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);	//������ ����(filename)���� �̹����� �о�ͼ� img �����Ϳ� ����/ width, height, channels �������� �̹����� ũ��� ä�� ���� ������
	std::vector<uint8_t> image;										//��ȯ�� �̹��� �����͸� ������ ���͸� ����
	image.resize(width * height * 4);								//�̹��� �����͸� ������ ����(image)�� ũ�⸦ �̹����� ��(width), ����(height), 4���� ä��(RGBA)�� ����Ͽ� ����

	for (size_t pixel = 0; pixel < width * height; pixel++)			// �̹����� ��� �ȼ��� ���� �ݺ��ϴ� ����
	{
		for (size_t rgb = 0; rgb < 3; rgb++)						// �� �ȼ��� RGB ���� ó���ϴ� ����
		{
			image[4 * pixel + rgb] = img[pixel * channels + rgb];	// ���� �ȼ��� RGB ���� image ���Ϳ� ����. 4 * pixel + rgb�� �� �ȼ����� R, G, B �� ä�ο� �ش��ϴ� �ε����� ���
		}
		image[4 * pixel + 3] = 255;									//�� �ȼ��� Alpha ä���� 255(������)�� ����
	}
		
	//�ؽ�ó �������� ������ ����.
	D3D11_TEXTURE2D_DESC texDesc = {};				// ��� ����� 0 �Ǵ� null�� �ʱ�ȭ
	texDesc.Width = width;							// �ؽ�ó�� ���� ũ�⸦ �̹����� ���� ũ��(width)�� ����
	texDesc.Height = height;						// �ؽ�ó�� ���� ũ�⸦ �̹����� ���� ũ��(height)�� ����
	texDesc.MipLevels = texDesc.ArraySize = 1;		// �ؽ�ó�� �̼��� ���� �� �� �迭�� ũ�⸦ 1�� ���� (��� ����)
	// cf. MipLevels : �ؽ�ó�� �ػ󵵸� �ٿ� ������ ����Ű�� �� ���, ���� �ؽ�ó�� �ػ󵵸� ������ �ٿ����鼭 ����, �ָ� ������ ��ü�� ���� �ؽ�ó ������ �� ȿ�������� ó��
	// cf. ArraySize : ���� ���� �ؽ�ó�� �ϳ��� �ؽ�ó�� ����ϴ� ���, �ؽ�ó �迭�� ����ϸ� �ٸ� ũ�� �Ǵ� �ٸ� �̹����� ���� ������ �ϳ��� �ؽ�ó�� ���� ���
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �ؽ�ó�� �ȼ� ������ 32��Ʈ RGBA �������� ����
	texDesc.SampleDesc.Count = 1;					// ��Ƽ���ø� ���� 1�� ���� : ��Ƽ ���ø� ��� X
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;			// �ؽ�ó�� ��� ��� : �ؽ�ó�� ������ ������ �ǹ��ϸ�, �ʱ�ȭ �Ŀ��� �������� �ʴ´�
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // �ؽ�ó�� ���̴� ���ҽ��� ���ε��ϵ��� ����

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;				// �ؽ�ó�� �ʱ� �����͸� ����
	InitData.pSysMem = image.data();				// �ʱ� �����ͷ� �̹��� ������ ������ �����͸� ����
	InitData.SysMemPitch = texDesc.Width * sizeof(uint8_t) * 4; // �̹����� ���� �� ������ ����Ʈ ���� �Ÿ��� ���� 

	device->CreateTexture2D(&texDesc, &InitData, texture.GetAddressOf()); // 2D �ؽ�ó ���� : �ʱ� ������(InitData)�� ���� �ְ�, texture �ּҿ� ����ִ´�.
	device->CreateShaderResourceView(texture.Get(), nullptr, textureResourceView.GetAddressOf()); // ���̴� ���ҽ� �並 ����
	//(���̴��� ���� �Է����� ����� ���ҽ� ������, ���̴� ���ҽ� �� ���� ���� ������, ID3D11ShaderResourceView�� ���� �������� �ּ�)
	//cf. ���̴� ���ҽ� �� : �ؽ�ó�� ���̴����� ���� �� �ֵ��� �ϴ� ��
}

/// <summary>
/// ť������� �ؽ�ó�� �����Ѵ�.
/// </summary>
/// <param name="device">����̽�</param>
/// <param name="filepath">�ؽ�ó ������ ���</param>
/// <param name="texResView">���̴� ���ҽ� ��(���� SRV�� ��Ī�ϰ���)</param>
void D3dUtilities::CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filepath, ComPtr<ID3D11ShaderResourceView>& texResView)
{
	ComPtr<ID3D11Texture2D> texture; // �ؽ�ó ������ : DDS ���Ϸκ��� ������ ť��� �ؽ�ó�� �޸𸮸� ����Ŵ
	//CreateDDSTextureFromFileEx : DDS ���Ͽ��� �ؽ�ó�� �ε��ϴ� �Լ�, DDS ������ �ؽ�ó�� ��ȯ�ϰ�, �ؽ�ó�� ���� SRV�� ����
	//(����̽� ������, ���� ���, ���� Ÿ�� ��(RTV) �Ǵ� ����-���ٽ� ��(DSV)�� �������� ����,
	// �ؽ�ó ����� : GPU���� �б� �� ���Ⱑ ������ �⺻ ��� ���, �ؽ�ó�� ���̴� ���ҽ��� ���ε��� ������ ����
	// ť��� �ؽ�ó�� ���� ������ ����, DDS �δ� �÷��׸� ����(false:�⺻����), ������ �ؽ�ó�� ����Ű�� ������, ������ �ؽ�ó�� ���� SRV ������, �Լ� ȣ���� �����ߴ����� Ȯ��)
	HRESULT hr = DirectX::CreateDDSTextureFromFileEx(device.Get(), filepath, 0,
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
