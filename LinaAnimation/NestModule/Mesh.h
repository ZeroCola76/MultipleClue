#pragma once
#include "ResourceBase.h"
/// <summary>
/// 테스트 메쉬입니다. 지울 클래스입니다!
/// </summary>
namespace GameEngine
{
	class Mesh : public ResourceBase
	{
	public:
		Mesh(ResourceType type) : ResourceBase(type) {}
		virtual ~Mesh() {}
	};

}
