#pragma once
#include "string"
#include <vector>

/// <summary>
/// 2024.01.25
/// 씬을 만들기위해 임시로 만들어둔 테스트 오브젝트 클래스입니다.
/// 현재는 오브젝트 타입, TRS, 컴포넌트(문자열)배열을 가지고 있습니다.
/// </summary>
namespace Test
{
	struct Vector3
	{
		Vector3() :x(), y(), z() {}
		Vector3(float x, float y, float z) :x(x), y(y), z(z) {}
		float x, y, z;
	};

	enum class ObjectType
	{
		NONE,
		CUBE,
		SPHERE,
		CYLINDER,

		END
	};
	struct TestObject
	{
		std::string name;
		ObjectType objectType;
		Vector3 pos;
		Vector3 rot;
		Vector3 scl;
		std::vector<std::string> components;
	};
}