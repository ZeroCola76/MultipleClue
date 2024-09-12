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
		//key���� _strVec���� ���� ���� ������ �����ϴ�.
		tempEntity->AddComponent(e, m_pEntityBuilder->BuildEntity(e));

		//�������� ���⼭ ���� ������� ��
		std::cout <<e<<" - " << typeid(m_pEntityBuilder->BuildEntity(e)).name() << " ������Ʈ �߰�\n";
	}

	return tempEntity;
}
