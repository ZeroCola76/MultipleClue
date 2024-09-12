#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent()
{
	//컴포넌트의 이름을 정한다.
	SetComponentName("GraphicsComponent");	///보통 클래스명을 그대로 따르기로한다.
	m_Material = new NestGraphics::Material();
	m_ModelData = new NestGraphics::ModelData();
	m_Light = new NestGraphics::Light();
}

GraphicsComponent::~GraphicsComponent()
{

}

