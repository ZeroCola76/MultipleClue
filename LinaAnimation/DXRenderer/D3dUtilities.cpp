#include "D3dUtilities.h"
#include <directxtk/DDSTextureLoader.h> // 큐브맵 읽을 때 필요
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/// <summary>
/// // 깊이 버퍼를 만드는 함수
/// </summary>
/// <param name="device">디바이스</param>
/// <param name="screenWidth">스크린 너비</param>
/// <param name="screenHeight">스크린 높이</param>
/// <param name="numQualityLevels">생성된 깊이 버퍼의 품질 수준을 나타내는 변수</param>
/// <param name="depthStencilView">생성된 깊이 스텐실 뷰 포인터</param>
/// <returns></returns>
bool D3dUtilities::CreateDepthBuffer(ComPtr<ID3D11Device>& device, int screenWidth, int screenHeight, UINT& numQualityLevels, ComPtr<ID3D11DepthStencilView>& depthStencilView)
{
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc;		// 깊이 스텐실 버퍼 특성 설명
	depthStencilBufferDesc.Width = screenWidth;			// 스크린 너비 설정
	depthStencilBufferDesc.Height = screenHeight;		// 스크린 높이 설정
	depthStencilBufferDesc.MipLevels = 1;				// MipLevel 검색 후 참조, 1 : 미설정
	depthStencilBufferDesc.ArraySize = 1;				// ArraySize 검색 후 참조, 1 : 미설정
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 깊이 스텐실 버퍼 형실 설정 : 24비트 깊이와 8비트 스텐실
	if (numQualityLevels > 0)							// 멀티 샘플링이 지원되는 경우 깊이 스텐실 사용
	{
		depthStencilBufferDesc.SampleDesc.Count = 4;						// 멀티 샘플링 개수
		depthStencilBufferDesc.SampleDesc.Quality = numQualityLevels - 1;	// 멀티 샘플링 품질 수준 설정
	}
	else
	{
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
	}
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;				// 깊이 스텐실 버퍼의 사용 방법을 설정 : 기본 설정
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// 깊이 스텐실 버퍼의 바인딩 플래그 설정 : 깊이 스텐실 자원으로 바인딩됨
	depthStencilBufferDesc.CPUAccessFlags = 0;						// CPU에서의 액세스 플래그를 설정 : 엑세스 불가
	depthStencilBufferDesc.MiscFlags = 0;							// 기타 플래그를 설정 : 설정 없음

	ComPtr<ID3D11Texture2D> depthStencilBuffer;			// 깊이 스텐실 버퍼 변수 선언

	// 디버깅
	if (FAILED(device->CreateTexture2D(&depthStencilBufferDesc, 0, depthStencilBuffer.GetAddressOf()))) // 깊이 스텐실 버퍼를 생성하고, 이를 depthStencilBuffer에 저장
	{
		std::cout << "CreateTexture2D() failed." << std::endl;
	}
	if (FAILED(device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf()))) // 깊이 스텐실 버퍼에 대한 깊이 스텐실 뷰를 생성하고, 이를 depthStencilView에 저장
	{
		std::cout << "CreateDepthStencilView() failed." << std::endl;
	}
	return true;
}
/// <summary>
/// 버텍스의 구조체 정보를 필요로 함
/// </summary>
/// <param name="filename">hlsl 파일</param>
/// <param name="inputElements">입력 레이아웃 설명 작성한 값</param>
/// <param name="vertexShader">버텍스 쉐이터 주소</param>
/// <param name="inputLayout">인풋 레이아웃 주소 연결</param>
void D3dUtilities::CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& filename, 
													const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, 
													ComPtr<ID3D11VertexShader>& vertexShader, 
													ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob; // 컴파일된 셰이더 코드를 저장하기 위한 인터페이스
	ComPtr<ID3DBlob> errorBlob; // 셰이더 컴파일 시 발생한 오류 메시지를 저장하기 위한 인터페이스

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	///TO DO : 분리하는 구조 필요하지만 여기서는 일단 구현에만 한다.
	// 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
	// D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
	HRESULT hr =
		// HLSL 코드를 지정된 대상에 대한 바이트코드로 컴파일
		// 파일 이름, 매크로 사용 유무, 쉐이더에서 include 사용하는지, 쉐이더 진입점 이름, 쉐이더 버전, 플래그, 0(쉐이더 사용시 0으로 설정), shaderBlob, errorBlob
		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Animation_Main", "vs_5_0",
			compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	CheckResult(hr, errorBlob.Get());

	// 입력-어셈블러 단계에 대한 입력 버퍼 데이터를 설명하는 입력 레이아웃 개체를 만든다. 
	// (입력 어셈블러 단계 입력 데이터 형식의 배열, input-elements 배열의 입력 데이터 형식 수, 컴파일된 셰이더에 대한 포인터, 컴파일된 셰이더의 크기, 생성된 입력 레이아웃 개체에 대한 포인터)
	///뭔가 이상..?
	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

	// 컴파일된 셰이더에서 꼭짓점 셰이더 개체를 만든다. (컴파일된 셰이더에 대한 포인터, 컴파일된 꼭짓점 셰이더의 크기, 클래스 연결 인터페이스에 대한 포인터, 버텍스 쉐이더 주소)
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),	NULL, &vertexShader);
}

void D3dUtilities::CreateVertexShaderAndInputLayoutNor(ComPtr<ID3D11Device>& device, const wstring& filename, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> shaderBlob; // 컴파일된 셰이더 코드를 저장하기 위한 인터페이스
	ComPtr<ID3DBlob> errorBlob; // 셰이더 컴파일 시 발생한 오류 메시지를 저장하기 위한 인터페이스

	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	///TO DO : 분리하는 구조 필요하지만 여기서는 일단 구현에만 한다.
	// 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
	// D3D_COMPILE_STANDARD_FILE_INCLUDE 추가: 쉐이더에서 include 사용
	HRESULT hr =
		// HLSL 코드를 지정된 대상에 대한 바이트코드로 컴파일
		// 파일 이름, 매크로 사용 유무, 쉐이더에서 include 사용하는지, 쉐이더 진입점 이름, 쉐이더 버전, 플래그, 0(쉐이더 사용시 0으로 설정), shaderBlob, errorBlob
		D3DCompileFromFile(filename.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0",
			compileFlags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

	CheckResult(hr, errorBlob.Get());

	// 입력-어셈블러 단계에 대한 입력 버퍼 데이터를 설명하는 입력 레이아웃 개체를 만든다. 
	// (입력 어셈블러 단계 입력 데이터 형식의 배열, input-elements 배열의 입력 데이터 형식 수, 컴파일된 셰이더에 대한 포인터, 컴파일된 셰이더의 크기, 생성된 입력 레이아웃 개체에 대한 포인터)
	device->CreateInputLayout(inputElements.data(), UINT(inputElements.size()),
		shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), &inputLayout);

	// 컴파일된 셰이더에서 꼭짓점 셰이더 개체를 만든다. (컴파일된 셰이더에 대한 포인터, 컴파일된 꼭짓점 셰이더의 크기, 클래스 연결 인터페이스에 대한 포인터, 버텍스 쉐이더 주소)
	device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, &vertexShader);
}

// 바로 위의 CreateVertexShaderAndInputLayout()와 변수가 같다. 버텍스만 픽셸로 이해하면 된다.
// 일반적으로 픽셸 쉐이더는 버텍스 쉐이더의 후에 생성되므로, 입력 레이아웃 개체는 따로 더 만들지 않는다.
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

	// 컴파일된 셰이더에서 픽셸 쉐이더에 대한 개체를 만든다. (컴파일된 셰이더에 대한 포인터, 컴파일된 픽셀 셰이더의 크기, 클래스 연결 인터페이스에 대한 포인터, 픽셸 쉐이더 주소)
	device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, pixelShader.GetAddressOf());
}

// 오브젝트 정점 버퍼의 인덱스 데이터를 초기 인덱스 할당 버퍼에 전달 후 인덱스 데이터를 바탕으로 초기 인덱스 할당 버퍼를 생성!
void D3dUtilities::CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& indices, ComPtr<ID3D11Buffer>& indexBuffer)
{
	//인덱스 버퍼의 설명을 설정
	D3D11_BUFFER_DESC bufferDesc = {};								 // 디폴트 값으로 초기화
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;						 // 초기화 후 변경X
	bufferDesc.ByteWidth = UINT(sizeof(uint32_t) * indices.size());	 // 바이트 크기 설정 (인덱스 * uint32_t)
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;					 // 바인드 플래스 : 버퍼를 인덱스 버퍼로 입력 어셈블러 단계에 바인딩
	bufferDesc.CPUAccessFlags = 0;									 // 0 : no CPU access is necessary.
	bufferDesc.StructureByteStride = sizeof(uint32_t);				 // 구조체의 바이트 단위 거리 : 4바이트

	// 인덱스 버퍼 데이터를 초기화하는 서브리소스 구조체를 생성
	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };					 // 디폴트 값으로 초기화
	indexBufferData.pSysMem = indices.data();						 // 시스템 메모리에 있는 데이터 포인터
	indexBufferData.SysMemPitch = 0;								 // 행 사이의 바이트 단위 거리
	indexBufferData.SysMemSlicePitch = 0;							 // 슬라이스 사이의 바이트 단위 거리
	// cf. 행 : 2D 이미지의 가로 방향, 슬라이스 : 3D 텍스처에서 여러 2D 이미지

	// 디바이스를 사용하여 인덱스 버퍼를 생성 (인덱스 버퍼 설명 주소, 인덱스 버퍼 데이터, 만들 인덱스 버퍼의 포인터)
	device->CreateBuffer(&bufferDesc, &indexBufferData, indexBuffer.GetAddressOf());
}

/// <summary>
/// 텍스처를 생성하는 함수
/// </summary>
/// <param name="filename">파일 경로를 포함한 이름</param>
/// <param name="m_texture">생성한 텍스처의 주소 참조</param>
/// <param name="texResourceView">텍스처의 이미지 뷰</param>
void D3dUtilities::CreateTexture(ComPtr<ID3D11Device>& device, const std::string filename, ComPtr<ID3D11Texture2D>& texture, ComPtr<ID3D11ShaderResourceView>& textureResourceView)
{

	//RGB 형식의 이미지를 RGBA 형식으로 변환. stb_image.h를 사용하여 이미지를 로드하고, 변환된 데이터를 std::vector<uint8_t>에 저장
	int width(0), height(0), channels(0);												// 이미지의 폭(width), 높이(height), 채널 수(channels)를 나타내는 변수를 선언하고 초기값을 0으로 설정
	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 0);	//지정된 파일(filename)에서 이미지를 읽어와서 img 포인터에 저장/ width, height, channels 변수에는 이미지의 크기와 채널 수가 설정됨
	std::vector<uint8_t> image;										//변환된 이미지 데이터를 저장할 벡터를 정의
	image.resize(width * height * 4);								//이미지 데이터를 저장할 벡터(image)의 크기를 이미지의 폭(width), 높이(height), 4개의 채널(RGBA)을 고려하여 설정

	for (size_t pixel = 0; pixel < width * height; pixel++)			// 이미지의 모든 픽셀에 대해 반복하는 루프
	{
		for (size_t rgb = 0; rgb < 3; rgb++)						// 각 픽셀의 RGB 값을 처리하는 루프
		{
			image[4 * pixel + rgb] = img[pixel * channels + rgb];	// 현재 픽셀의 RGB 값을 image 벡터에 저장. 4 * pixel + rgb는 각 픽셀에서 R, G, B 각 채널에 해당하는 인덱스를 계산
		}
		image[4 * pixel + 3] = 255;									//각 픽셀의 Alpha 채널을 255(불투명)로 설정
	}
		
	//텍스처 데이터의 설명을 설정.
	D3D11_TEXTURE2D_DESC texDesc = {};				// 모든 멤버를 0 또는 null로 초기화
	texDesc.Width = width;							// 텍스처의 가로 크기를 이미지의 가로 크기(width)로 설정
	texDesc.Height = height;						// 텍스처의 세로 크기를 이미지의 세로 크기(height)로 설정
	texDesc.MipLevels = texDesc.ArraySize = 1;		// 텍스처의 미세한 수준 수 및 배열의 크기를 1로 설정 (사용 안함)
	// cf. MipLevels : 텍스처의 해상도를 줄여 성능을 향상시키는 데 사용, 원본 텍스처의 해상도를 반으로 줄여가면서 생성, 멀리 떨어진 객체에 대한 텍스처 매핑을 더 효과적으로 처리
	// cf. ArraySize : 여러 개의 텍스처를 하나의 텍스처로 취급하는 기능, 텍스처 배열을 사용하면 다른 크기 또는 다른 이미지의 여러 버전을 하나의 텍스처로 쉽게 사용
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 텍스처의 픽셀 형식을 32비트 RGBA 형식으로 지정
	texDesc.SampleDesc.Count = 1;					// 멀티샘플링 수를 1로 설정 : 멀티 샘플링 사용 X
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;			// 텍스처의 사용 방법 : 텍스처가 변하지 않음을 의미하며, 초기화 후에는 수정되지 않는다
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // 텍스처를 셰이더 리소스로 바인딩하도록 설정

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;				// 텍스처의 초기 데이터를 설정
	InitData.pSysMem = image.data();				// 초기 데이터로 이미지 벡터의 데이터 포인터를 설정
	InitData.SysMemPitch = texDesc.Width * sizeof(uint8_t) * 4; // 이미지의 가로 행 사이의 바이트 단위 거리를 설정 

	device->CreateTexture2D(&texDesc, &InitData, texture.GetAddressOf()); // 2D 텍스처 생성 : 초기 데이터(InitData)를 설명에 넣고, texture 주소에 집어넣는다.
	device->CreateShaderResourceView(texture.Get(), nullptr, textureResourceView.GetAddressOf()); // 셰이더 리소스 뷰를 생성
	//(셰이더에 대한 입력으로 사용할 리소스 포인터, 셰이더 리소스 뷰 설명에 대한 포인터, ID3D11ShaderResourceView에 대한 포인터의 주소)
	//cf. 셰이더 리소스 뷰 : 텍스처를 셰이더에서 읽을 수 있도록 하는 뷰
}

/// <summary>
/// 큐브매핑의 텍스처를 생성한다.
/// </summary>
/// <param name="device">디바이스</param>
/// <param name="filepath">텍스처 파일의 경로</param>
/// <param name="texResView">셰이더 리소스 뷰(이후 SRV로 지칭하겠음)</param>
void D3dUtilities::CreateCubemapTexture(ComPtr<ID3D11Device>& device, const wchar_t* filepath, ComPtr<ID3D11ShaderResourceView>& texResView)
{
	ComPtr<ID3D11Texture2D> texture; // 텍스처 포인터 : DDS 파일로부터 생성된 큐브맵 텍스처의 메모리를 가리킴
	//CreateDDSTextureFromFileEx : DDS 파일에서 텍스처를 로드하는 함수, DDS 파일을 텍스처로 변환하고, 텍스처에 대한 SRV를 생성
	//(디바이스 포인터, 파일 경로, 렌더 타겟 뷰(RTV) 또는 깊이-스텐실 뷰(DSV)를 생성하지 않음,
	// 텍스처 사용방법 : GPU에서 읽기 및 쓰기가 가능한 기본 사용 방법, 텍스처가 셰이더 리소스로 바인딩될 것임을 지정
	// 큐브맵 텍스처로 사용될 것임을 지정, DDS 로더 플래그를 설정(false:기본설정), 생성된 텍스처를 가리키는 포인터, 생성된 텍스처에 대한 SRV 포인터, 함수 호출이 실패했는지를 확인)
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
		// 파일이 없을 경우
		if ((hr & D3D11_ERROR_FILE_NOT_FOUND) != 0) { std::cout << "File not found." << std::endl; }
		if (errorBlob) // 에러 메시지가 있으면 출력
		{
			std::cout << "Shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}
}
