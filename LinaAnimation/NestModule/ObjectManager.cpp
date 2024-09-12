#include "ObjectManager.h"

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	m_EntityVec.clear();
}

void ObjectManager::Initialize()
{
	//m_pJsonManager = new JsonManager();
	m_pEntityFactory = new EntityFactory();
}

void ObjectManager::Update()
{

}

void ObjectManager::Finalize()
{
	//delete m_pJsonManager;
	delete m_pEntityFactory;
}

void ObjectManager::ReadJson()
{
	///TODO : JsonManager�� ���� ������ �޾ƿ��� �ڵ� �ʿ�.
	// [�����ڵ�]
	//JsonManager���� ���� �޾ƿ´�.
	//ex) tuple<string, std::vector<string>> tempEntityInfo = m_pJsonManager -> GetEntityInformation();
	//AddObject(tempEntity.first, tempEntity.second);
}

void ObjectManager::AddObject(std::string _entityName, std::vector<std::string> comNameVec)
{
	//TODO - Converter�κ�.
	m_EntityVec.push_back(m_pEntityFactory->CreateEntity(_entityName, comNameVec));
}

std::vector<std::shared_ptr<Entity>>* ObjectManager::GetEntityVecAddress()
{
	return &m_EntityVec;
}
