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

	//virtual void DrawMesh() abstract;

	// ������ EndRender
	virtual void EndRender() abstract;

	// ������ Finalize
	virtual void Finalize() abstract;

	//�ӽ÷� ��� ������Ʈ���� �޴� �Լ�
	//virtual void GetGeometry( std::vector<NestGraphics::Vertex> m_vertece,
	//	std::vector<uint16_t> m_indices, std::string m_string, std::string m_textureStr,
	//	ComPtr<ID3D11Buffer> m_vBuffer, ComPtr<ID3D11Buffer> m_iBuffer, ComPtr<ID3D11Texture2D> m_texture,
	//	ComPtr<ID3D11ShaderResourceView> m_textureView, ComPtr<ID3D11Buffer> m_vConstBuffer,
	//	ComPtr<ID3D11Buffer> m_pConstBuffer) abstract;

	//MeshData�� �޾ƿ�
   // virtual void GetMeshData(NestGraphics::MeshData m_getMeshData) abstract;
   // virtual void GetMeshModelData(std::vector<NestGraphics::MeshData> m_getMeshModelData) abstract;

	//Texture��θ� �޴´�. (�ؽ�ó �̹���, �ؽ�ó �̹���2, ���ؽ����̴�, �ȼ����̴�);
//     virtual void GetTexturePath(const std::string filename, const std::string filename2,
//                                 const wstring filename3, const wstring filename4) abstract;

		//�ӽ� �Լ� BeginRender�� EndRender ���̿� �־ �׸��ϴ�. --TRS �� ///2024.1.24 �迹���� ���̻� �� �ѹ��Լ��� ���� �ʽ��ϴ�.
	virtual void DrawSomething() abstract;
	virtual void CurrentMousePos(const int& mouseX, const int& mouseY) abstract;
};