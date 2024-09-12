#pragma once
#include <string>
#include<iostream>
#include "ResourceEnum.h"

/// <summary>
/// �̰� ���߿� ������Ʈ�� �����Ǵ°� �ƴѰ�..?
/// </summary>
class ResourceBase
{
public:
	ResourceBase(ResourceType type) : m_Type(type) { std::cout << "ResourceBase ����" << std::endl; };
	ResourceBase() {}
	virtual ~ResourceBase() {}

public:
	ResourceType m_Type;
	std::wstring m_Name;

public:
	ResourceType GetType() { return m_Type; }
	std::wstring GetName() const { return m_Name; }
	void SetName(const std::wstring& _name) { m_Name = _name; }
};

