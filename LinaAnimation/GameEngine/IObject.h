#pragma once
#include <string>

///Object���� ������ ���� �±׸� Enum����.
enum EObjectTag
{
	PLAYER,					//1
	MONSTER,				//2
	OBJECT,					//3
							//
							//
							//
							//
							//
							//
							//
							//
	DEFALUT,				//defalut�� �ǵ����̸� �������� ����
};

///IObject - IGameObject - Player ������ ����� �ް��Ѵ�.
class IObject
{
public:
	IObject() {}
	virtual ~IObject() {}

protected:
	std::string m_Name;
	int tag;
};