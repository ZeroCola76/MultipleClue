#pragma once
#include "string"
#include <vector>

/// <summary>
/// 2024.01.25
/// ���� ��������� �ӽ÷� ������ �׽�Ʈ ������Ʈ Ŭ�����Դϴ�.
/// ����� ������Ʈ Ÿ��, TRS, ������Ʈ(���ڿ�)�迭�� ������ �ֽ��ϴ�.
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