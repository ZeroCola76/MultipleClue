#pragma once
#include "IComponent.h"
#include "Geometry.h"
#include "ConstantBuffer.h"
#include "light.h"

///게임엔진에서 그래픽스로 보내야될 정보들
class GraphicsComponent : public IComponent
{
public:
	GraphicsComponent();
	~GraphicsComponent();

public:
	///Geometry.h
	NestGraphics::Material* m_Material;				 //재질
	NestGraphics::ModelData* m_ModelData;			 //모델에 속한 메시들의 배열 정보

	///Light.h 
	NestGraphics::Light* m_Light;					 //조명
};




//원래는 있었지만 .. 뺴기로 한것들. 후에 구조변화가 있을수도 있으므로 인단 남긴다. 
///Geometry.h
//NestGraphics::MeshData m_MeshData;				 //메쉬 데이터 : 기본적으로 버텍스의 위치와 인덱스를 저장.
//NestGraphics::Mesh m_Mesh;						 //메쉬의 정보


/// ConstantBuffer.h
//NestGraphics::VertexConstantBuffer m_VertexConstantBuffer;					   //버텍스 상수버퍼
//NestGraphics::PixelShaderConstantBuffer m_PixelShaderConstantBuffer;		   //픽셀 상수버퍼
//NestGraphics::NormalShaderConstantBuffer m_NormalShaderConstantBuffer;		   //노말 상수버퍼