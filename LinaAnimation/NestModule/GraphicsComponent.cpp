#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent()
{
	//������Ʈ�� �̸��� ���Ѵ�.
	SetComponentName("GraphicsComponent");	///���� Ŭ�������� �״�� ��������Ѵ�.
	m_Material = new NestGraphics::Material();
	m_ModelData = new NestGraphics::ModelData();
	m_Light = new NestGraphics::Light();
}

GraphicsComponent::~GraphicsComponent()
{

}

