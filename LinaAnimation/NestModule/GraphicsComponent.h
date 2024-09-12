#pragma once
#include "IComponent.h"
#include "Geometry.h"
#include "ConstantBuffer.h"
#include "light.h"

///���ӿ������� �׷��Ƚ��� �����ߵ� ������
class GraphicsComponent : public IComponent
{
public:
	GraphicsComponent();
	~GraphicsComponent();

public:
	///Geometry.h
	NestGraphics::Material* m_Material;				 //����
	NestGraphics::ModelData* m_ModelData;			 //�𵨿� ���� �޽õ��� �迭 ����

	///Light.h 
	NestGraphics::Light* m_Light;					 //����
};




//������ �־����� .. ����� �Ѱ͵�. �Ŀ� ������ȭ�� �������� �����Ƿ� �δ� �����. 
///Geometry.h
//NestGraphics::MeshData m_MeshData;				 //�޽� ������ : �⺻������ ���ؽ��� ��ġ�� �ε����� ����.
//NestGraphics::Mesh m_Mesh;						 //�޽��� ����


/// ConstantBuffer.h
//NestGraphics::VertexConstantBuffer m_VertexConstantBuffer;					   //���ؽ� �������
//NestGraphics::PixelShaderConstantBuffer m_PixelShaderConstantBuffer;		   //�ȼ� �������
//NestGraphics::NormalShaderConstantBuffer m_NormalShaderConstantBuffer;		   //�븻 �������