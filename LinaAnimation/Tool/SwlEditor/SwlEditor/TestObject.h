#pragma once
#include "framework.h"

/// <summary>
/// 2024.01.25
/// ���� ��������� �ӽ÷� ������ �׽�Ʈ ������Ʈ Ŭ�����Դϴ�.
/// ����� ������Ʈ Ÿ��, TRS, ������Ʈ(���ڿ�)�迭�� ������ �ֽ��ϴ�.
/// </summary>

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

/// <summary>
/// Json::Value��ü�� ������ �ִ� ������Ʈ
/// �����Ϳ��� �̸� ���� ��Ȱ�� �������� �׽�Ʈ�� ������Ʈ
/// �Ѱ��� Json���Ͽ� �������� ��Ȱ�� ������ ������Ʈ�������� �־, ������ ����ÿ� �ε��ϰ�, ����� �����Դϴ�.
/// </summary>
struct TestObject2
{
	std::string name;
	Json::Value m_data;
};