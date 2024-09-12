#pragma once
//#include "../Main_DemoGraphics/GameObject.h"  //이것은 아주 문제가 있나..?

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

	//임시 함수 BeginRender와 EndRender 사이에 넣어서 그립니다. --TRS 만
	virtual void DrawMesh() abstract;

	// 렌더러 EndRender
	virtual void EndRender() abstract;

	// 렌더러 Finalize
	virtual void Finalize() abstract;

	//MeshData를 받아요
  ///  virtual void GetMeshData(NestGraphics::MeshData* m_getMeshData) abstract;
    virtual void SetModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) abstract;
	virtual void SetCubeMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) abstract;

    //virtual void SetMeshModelFilePath(std::vector<NestGraphics::MeshData>* _ModelMeshData, const std::wstring& _filePath, const float& _scale) abstract;

	//Texture경로를 받는다. (텍스처 이미지, 텍스처 이미지2, 버텍스셰이더, 픽셀셰이더);
//     virtual void GetTexturePath(const std::string filename, const std::string filename2,
//                                 const wstring filename3, const wstring filename4) abstract;

	// 슬프지만 상호참조하게 카메라를 만들자...일단 만들어...
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
};