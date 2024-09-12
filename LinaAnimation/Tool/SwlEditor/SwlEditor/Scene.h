#pragma once
#include "framework.h"

class TestObject;

/// <summary>
/// 2024.01.25
/// �ϳ��� ������ ���Ͽ� ����� �ϳ��� ���Դϴ�.
/// ��Ȳ�� ���� N���� ���� �ϳ��� �����Ϳ� �����ϰ� �� ���� �ֽ��ϴ�.
/// �ϳ��� ���� N���� ������Ʈ�� �������ֽ��ϴ�.
/// 2024.01.30
/// ������ module���� Scene�� �����Ұ��̱� ������ �ʿ������?
/// ->�ϴ� �ӽ÷� ����� Ŭ������ �����Ӵϴ�.
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

