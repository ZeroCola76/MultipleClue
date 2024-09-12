#pragma once
#include "../Main_DemoGrapics/GameObject.h"  //이것은 아주 문제가 있나..?

class IRenderer abstract
{
public:
	IRenderer() {};
	virtual ~IRenderer() {};

	//렌더러 초기화 (HWND, 프로그램 길이, 프로그램 높이)
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) abstract;

	// 렌더러 업데이트(타이머)
	virtual bool Update(float m_dTime) abstract;

	// 렌더러 BeginRender
	virtual void BeginRender() abstract;

	//virtual void DrawMesh() abstract;

	// 렌더러 EndRender
	virtual void EndRender() abstract;

	// 렌더러 Finalize
	virtual void Finalize() abstract;

	//임시로 모든 지오메트리를 받는 함수
	//virtual void GetGeometry( std::vector<NestGraphics::Vertex> m_vertece,
	//	std::vector<uint16_t> m_indices, std::string m_string, std::string m_textureStr,
	//	ComPtr<ID3D11Buffer> m_vBuffer, ComPtr<ID3D11Buffer> m_iBuffer, ComPtr<ID3D11Texture2D> m_texture,
	//	ComPtr<ID3D11ShaderResourceView> m_textureView, ComPtr<ID3D11Buffer> m_vConstBuffer,
	//	ComPtr<ID3D11Buffer> m_pConstBuffer) abstract;

	//MeshData를 받아요
   // virtual void GetMeshData(NestGraphics::MeshData m_getMeshData) abstract;
   // virtual void GetMeshModelData(std::vector<NestGraphics::MeshData> m_getMeshModelData) abstract;

	//Texture경로를 받는다. (텍스처 이미지, 텍스처 이미지2, 버텍스셰이더, 픽셀셰이더);
//     virtual void GetTexturePath(const std::string filename, const std::string filename2,
//                                 const wstring filename3, const wstring filename4) abstract;

		//임시 함수 BeginRender와 EndRender 사이에 넣어서 그립니다. --TRS 만 ///2024.1.24 김예리나 더이상 이 한방함수는 쓰지 않습니다.
	virtual void DrawSomething() abstract;
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
};