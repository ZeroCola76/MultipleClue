#pragma once
#include "IObject.h"

class IGameObject : public IObject
{
public:
	IGameObject();
	virtual ~IGameObject();
public: 
	//������Ʈ�� ������ �� (ECS)
	virtual void CreateObject() abstract; 

	///�Ʒ����ʹ� �׳� ������ �����. 
	///�̺κ��� ECS�� ������ �ִٰ� �����Ѵ�.
	/*-----------------------------------------------------
	//��κ��� ������Ʈ�� ������Ʈ ���� ������ �ִٰ� �����Ѵ�.
	virtual void UpdateObject() abstract;  

	//��� ������Ʈ�� Render�ɼ� �ִ�. 
	virtual void RenderObject()	abstract;
	------------------------------------------------------*/

}; 