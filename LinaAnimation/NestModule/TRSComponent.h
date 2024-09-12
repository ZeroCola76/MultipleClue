#pragma once
#include "IComponent.h"
#include "directxtk/SimpleMath.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

/// <summary>
/// 거의 모든 엔티티가 가지고 있을 TRS Component
/// </summary>
class TRSComponent : public IComponent
{
public:
	TRSComponent(); 
	~TRSComponent();

public:
	Vector3 m_Position;
	//Vector3 m_Transform;	//후에 quaternion 추가 시 활성화를 시킨다. 
	Vector3 m_Rotation;
	Vector3 m_Scale;

public:
	// 그냥 임시로 만든 함수입니다.
	// T,R,S 값을 입력하면 TRS 매트릭스로 바꿔주는 함수
	Matrix MakeTRSMatrix(Vector3 m_Position, Vector3 m_Rotation, Vector3 m_Scale);
};

