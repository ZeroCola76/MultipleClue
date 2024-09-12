#pragma once
#include <string>

class IComponent
{
public:
	IComponent() {}
	virtual ~IComponent() {}

protected:
	std::string m_ComponentName;	//컴포넌트의 이름 후에 툴과의 상호작용에 유용히 쓰이길 바란다.

public:
	virtual void SetComponentName(std::string _Name) { m_ComponentName = _Name; }
	std::string GetComponentName() { return m_ComponentName; }
};