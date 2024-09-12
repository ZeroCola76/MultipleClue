#pragma once
#include <string>

///Object들의 가지고 있을 태그를 Enum으로.
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
	DEFALUT,				//defalut는 되도록이면 마지막에 두자
};

///IObject - IGameObject - Player 순으로 상속을 받게한다.
class IObject
{
public:
	IObject() {}
	virtual ~IObject() {}

protected:
	std::string m_Name;
	int tag;
};