#pragma once
#include "ResourceBase.h"
/// <summary>
/// �׽�Ʈ �޽��Դϴ�. ���� Ŭ�����Դϴ�!
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
