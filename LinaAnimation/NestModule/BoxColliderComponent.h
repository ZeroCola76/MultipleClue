#pragma once
#include "ICollider.h"
class BoxColliderComponent : public ICollider
{
public:
	BoxColliderComponent();
	~BoxColliderComponent();
public:
	//ICollider 먼저 만들고.. 추가하자.
};

