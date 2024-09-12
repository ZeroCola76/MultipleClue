#pragma once
#include "IComponent.h"

/// 콜라이더를 위한 중간 클래스 
class ICollider : public IComponent
{
public:
	ICollider() {}
	~ICollider(){}

public:
	///TODO : 충돌체크에 필요한 것들을 체크한다.
	///콜라이더 크기를 시각화하기위해.. 그래픽스 관련 변수가 필요하지 않을까하는 생각이 든다.
};