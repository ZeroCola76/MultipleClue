#pragma once
#include <initializer_list>
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;

class IRenderer abstract
{
public:
	IRenderer() {};
	virtual ~IRenderer() {};

	/// <summary>
	/// 렌더러 초기화
	/// </summary>
	/// <param name="m_HWnd">핸들</param>
	/// <param name="m_ScreenWidth">스크린 너비</param>
	/// <param name="m_ScreenHeight">스크린 높이</param>
	/// <returns>true : 초기화 성공</returns>
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) abstract;

	/// <summary>
	/// 렌더러 업데이트
	/// </summary>
	/// <param name="m_dTime">실시간 타임</param>
	/// <returns>true : 업데이트 성공</returns>
	virtual bool Update(float m_dTime) abstract;

	/// <summary>
	/// 렌더러 BeginRender
	/// </summary>
	virtual void BeginRender() abstract;

	/// <summary>
	/// 임시 함수 BeginRender와 EndRender 사이에 넣어서 그립니다. --TRS 만
	/// </summary>
	virtual void DrawMesh() abstract;

	/// <summary>
	///	렌더러 EndRender
	/// </summary>
	virtual void EndRender() abstract;

	/// <summary>
	/// 렌더러 Finalize
	/// </summary>
	virtual void Finalize() abstract;

    /// <summary>
    /// ModelMeshData를 외부에서 받아서 랜더러에 넣어주는 함수 (일반 메쉬용)
    /// </summary>
    /// <param name="_ModelMeshData">메쉬 데이터의 포인터</param>
    virtual void SetModelMeshData(NestGraphics::ModelData* _ModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys) abstract;

	/// <summary>
	/// 김예리나가 작성한 애니를 위한 함수
	/// </summary>
	/// <param name="_ModelMeshData"></param>
	/// <param name="trsMatrix"></param>
	/// <param name="textureFileKeys"></param>
	virtual void SetSkeltaModelMeshData(NestGraphics::ModelData* _ModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys) abstract;


	/// <summary>
	/// 현재 마우스의 값을 카메라에 전달하기 위한 함수
	/// </summary>
	/// <param name="mouseX">현재 마우스의 x값</param>
	/// <param name="mouseY">현재 마우스의 y값</param>
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
};                       