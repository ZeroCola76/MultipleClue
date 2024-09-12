#pragma once
#include <string>

class IComponent
{
public:
	IComponent() {}
	virtual ~IComponent() {}

protected:
	std::string m_ComponentName;	//������Ʈ�� �̸� �Ŀ� ������ ��ȣ�ۿ뿡 ������ ���̱� �ٶ���.

public:
	virtual void SetComponentName(std::string _Name) { m_ComponentName = _Name; }
	std::string GetComponentName() { return m_ComponentName; }
};