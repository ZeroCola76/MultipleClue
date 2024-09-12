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

	//박스 그림
	virtual void DrawBox() abstract;

	//구 그림
	virtual void DrawSphere() abstract;
	
	//큐브맵 그림(스카이 박스)
	virtual void DrawCubeMap() abstract;

	// 렌더러 EndRender
	virtual void EndRender() abstract;

	// 렌더러 Finalize
	virtual void Finalize() abstract;

	//MeshData를 받아요
   // virtual void GetMeshData(Nest::MeshData m_getMeshData) abstract;
   // virtual void GetMeshModelData(std::vector<Nest::MeshData> m_getMeshModelData) abstract;

	//Texture경로를 받는다. (텍스처 이미지, 텍스처 이미지2, 버텍스셰이더, 픽셀셰이더);
//     virtual void GetTexturePath(const std::string filename, const std::string filename2,
//                                 const wstring filename3, const wstring filename4) abstract;

		//임시 함수 BeginRender와 EndRender 사이에 넣어서 그립니다. --TRS 만 ///2024.1.24 김예리나 더이상 이 한방함수는 쓰지 않습니다.
	//virtual void DrawSomething() abstract;
};