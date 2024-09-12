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
	/// �迹������ �ۼ��� �ִϸ� ���� �Լ�
	/// </summary>
	/// <param name="_ModelMeshData"></param>
	/// <param name="trsMatrix"></param>
	/// <param name="textureFileKeys"></param>
	virtual void SetSkeltaModelMeshData(NestGraphics::ModelData* _ModelMeshData, Matrix trsMatrix, std::initializer_list<std::wstring> textureFileKeys) abstract;


	/// <summary>
	/// ���� ���콺�� ���� ī�޶� �����ϱ� ���� �Լ�
	/// </summary>
	/// <param name="mouseX">���� ���콺�� x��</param>
	/// <param name="mouseY">���� ���콺�� y��</param>
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
};                       