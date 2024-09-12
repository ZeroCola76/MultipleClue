#include "Entity.h"
#include "IComponent.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}



void Entity::Initialize(std::string _name)
{
	m_Name = _name;
}

void Entity::Finalize()
{
	//shared_ptr이기 때문에 지우기만 해도 된다.
	m_pComponents.clear();
}

void Entity::AddComponent(std::string ComName, std::shared_ptr<IComponent> _Component)
{
	m_pComponents[ComName] = _Component;
}
