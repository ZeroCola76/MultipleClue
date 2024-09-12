#pragma once
#include "framework.h"

/// <summary>
/// 2024.01.25
/// 씬을 만들기위해 임시로 만들어둔 테스트 오브젝트 클래스입니다.
/// 현재는 오브젝트 타입, TRS, 컴포넌트(문자열)배열을 가지고 있습니다.
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
/// Json::Value객체를 가지고 있는 오브젝트
/// 에디터에서 미리 만들어서 재활용 가능한지 테스트용 오브젝트
/// 한개의 Json파일에 여러개의 재활용 가능한 오브젝트정보들을 넣어서, 에디터 실행시에 로드하고, 사용할 예정입니다.
/// </summary>
struct TestObject2
{
	std::string name;
	Json::Value m_data;
};