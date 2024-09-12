#include "EntityFactory.h"
#include "EntityBuilder.h"
#include "Entity.h"

EntityFactory::EntityFactory()
{

	m_pEntityBuilder = new EntityBuilder;
}

EntityFactory::~EntityFactory()
{
	delete m_pEntityBuilder;
}

std::shared_ptr<Entity> EntityFactory::CreateEntity(std::string _entityName, std::vector<std::string> _strVec)
{
	std::shared_ptr<Entity> tempEntity = std::make_shared<Entity>();
	tempEntity->Initialize(_entityName);

	for (const auto& e : _strVec)
	{
		//key값과 _strVec내의 값이 같기 떄문에 가능하다.
		tempEntity->AddComponent(e, m_pEntityBuilder->BuildEntity(e));

		//오류나면 여기서 문제 생기겠지 뭐
		std::cout <<e<<" - " << typeid(m_pEntityBuilder->BuildEntity(e)).name() << " 컴포넌트 추가\n";
	}

	return tempEntity;
}
