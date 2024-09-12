#pragma once
#include "framework.h"

class TestObject;

/// <summary>
/// 2024.01.25
/// 하나의 데이터 파일에 저장될 하나의 씬입니다.
/// 상황에 따라 N개의 씬을 하나의 데이터에 저장하게 될 수도 있습니다.
/// 하나의 씬은 N개의 오브젝트를 가지고있습니다.
/// 2024.01.30
/// 어차피 module에서 Scene을 관리할것이기 때문에 필요없을듯?
/// ->일단 임시로 사용할 클래스만 만들어둡니다.
/// </summary>
class Scene
{
public:
	Scene() {}
	~Scene() {}


// private:
	std::vector<TestObject*> m_pObj;
	Json::Value m_data;
};

