#pragma once
//#include "../Main_DemoGraphics/GameObject.h"  //�̰��� ���� ������ �ֳ�..?

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

	//�ӽ� �Լ� BeginRender�� EndRender ���̿� �־ �׸��ϴ�. --TRS ��
	virtual void DrawMesh() abstract;

	// ������ EndRender
	virtual void EndRender() abstract;

	// ������ Finalize
	virtual void Finalize() abstract;

	//MeshData�� �޾ƿ�
  ///  virtual void GetMeshData(NestGraphics::MeshData* m_getMeshData) abstract;
    virtual void SetModelMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) abstract;
	virtual void SetCubeMeshData(std::vector<NestGraphics::MeshData>* _ModelMeshData) abstract;

    //virtual void SetMeshModelFilePath(std::vector<NestGraphics::MeshData>* _ModelMeshData, const std::wstring& _filePath, const float& _scale) abstract;

	//Texture��θ� �޴´�. (�ؽ�ó �̹���, �ؽ�ó �̹���2, ���ؽ����̴�, �ȼ����̴�);
//     virtual void GetTexturePath(const std::string filename, const std::string filename2,
//                                 const wstring filename3, const wstring filename4) abstract;

	// �������� ��ȣ�����ϰ� ī�޶� ������...�ϴ� �����...
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
};