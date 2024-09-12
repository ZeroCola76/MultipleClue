#pragma once
#include "../Main_DemoGrapics/GameObject.h"  //�̰��� ���� ������ �ֳ�..?

class IRenderer abstract
{
public:
	IRenderer() {};
	virtual ~IRenderer() {};

	//������ �ʱ�ȭ (HWND, ���α׷� ����, ���α׷� ����)
	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) abstract;

	// ������ ������Ʈ(Ÿ�̸�)
	virtual bool Update(float m_dTime) abstract;

	// ������ BeginRender
	virtual void BeginRender() abstract;

	//�ڽ� �׸�
	virtual void DrawBox() abstract;

	//�� �׸�
	virtual void DrawSphere() abstract;
	
	//ť��� �׸�(��ī�� �ڽ�)
	virtual void DrawCubeMap() abstract;

	// ������ EndRender
	virtual void EndRender() abstract;

	// ������ Finalize
	virtual void Finalize() abstract;

	//MeshData�� �޾ƿ�
   // virtual void GetMeshData(Nest::MeshData m_getMeshData) abstract;
   // virtual void GetMeshModelData(std::vector<Nest::MeshData> m_getMeshModelData) abstract;

	//Texture��θ� �޴´�. (�ؽ�ó �̹���, �ؽ�ó �̹���2, ���ؽ����̴�, �ȼ����̴�);
//     virtual void GetTexturePath(const std::string filename, const std::string filename2,
//                                 const wstring filename3, const wstring filename4) abstract;

		//�ӽ� �Լ� BeginRender�� EndRender ���̿� �־ �׸��ϴ�. --TRS �� ///2024.1.24 �迹���� ���̻� �� �ѹ��Լ��� ���� �ʽ��ϴ�.
	//virtual void DrawSomething() abstract;
};