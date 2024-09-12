#pragma once
#include <initializer_list>
#include <directxtk/SimpleMath.h>
#include "Geometry.h"


using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

class IRenderer abstract
{
public:
	IRenderer() {};
	virtual ~IRenderer() {};

	/// <summary>
	/// ������ �ʱ�ȭ
	/// </summary>
	/// <param name="m_HWnd">�ڵ�</param>
	/// <param name="m_ScreenWidth">��ũ�� �ʺ�</param>
	/// <param name="m_ScreenHeight">��ũ�� ����</param>
	/// <returns>true : �ʱ�ȭ ����</returns>
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) abstract;

	/// <summary>
	/// ������ ������Ʈ
	/// </summary>
	/// <param name="m_dTime">�ǽð� Ÿ��</param>
	/// <returns>true : ������Ʈ ����</returns>
	virtual bool Update(float m_dTime) abstract;

	/// <summary>
	/// ������ BeginRender
	/// </summary>
	virtual void BeginRender() abstract;

	/// <summary>
	/// �ӽ� �Լ� BeginRender�� EndRender ���̿� �־ �׸��ϴ�. --TRS ��
	/// </summary>
	virtual void DrawMesh() abstract;

	/// <summary>
	///	������ EndRender
	/// </summary>
	virtual void EndRender() abstract;

	/// <summary>
	/// ������ Finalize
	/// </summary>
	virtual void Finalize() abstract;

    /// <summary>
    /// ModelMeshData�� �ܺο��� �޾Ƽ� �������� �־��ִ� �Լ� (�Ϲ� �޽���)
    /// </summary>
    /// <param name="_ModelMeshData">�޽� �������� ������</param>
    virtual void SetModelMeshData(NestGraphics::ModelData* _ModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys) abstract;

	/// <summary>
	/// ModelMeshData�� �ܺο��� �޾Ƽ� �������� �־��ִ� �Լ� (ť�� �޽���)
	/// </summary>
	/// <param name="_ModelMeshData">ť�� �޽� �������� ������</param>
	virtual void SetCubeMeshData(NestGraphics::ModelData* _ModelMeshData, std::wstring originalkey,
									std::wstring diffusekey, std::wstring specularkey) abstract;

	/// <summary>
	/// ���� ���콺�� ���� ī�޶� �����ϱ� ���� �Լ�
	/// </summary>
	/// <param name="mouseX">���� ���콺�� x��</param>
	/// <param name="mouseY">���� ���콺�� y��</param>
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
	
	/// ���� Ű�� ���� ī�޶� �����ϱ� ���� �Լ�
	virtual void CurrentKeyPressed(int keyCode) abstract;

	/// �ٱ����� ������ TRS ������Ʈ �Լ�
	//virtual Matrix SetTRSMatrix(const Matrix& _trsMatrix) abstract;
};                       