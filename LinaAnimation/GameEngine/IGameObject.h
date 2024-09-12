#pragma once
#include "IObject.h"

class IGameObject : public IObject
{
public:
	IGameObject();
	virtual ~IGameObject();
public: 
	//오브젝트를 생성할 것 (ECS)
	virtual void CreateObject() abstract; 

	///아래부터는 그냥 생각을 적어본다. 
	///이부분은 ECS로 뺄수도 있다고 생각한다.
	/*-----------------------------------------------------
	//대부분의 오브젝트는 업데이트 값을 가질수 있다고 생각한다.
	virtual void UpdateObject() abstract;  

	//모든 오브젝트는 Render될수 있다. 
	virtual void RenderObject()	abstract;
	------------------------------------------------------*/

}; 