#include "Scene.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "ObjectManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

std::vector<std::shared_ptr<Entity>> Scene::GetEntityVector()
{
	// ObjectManager에 있는 entity들을 넣어줘야 하나? <- 이건 SceneManager가 담당하므로 탈락.
	// 그렇다면 EntityFactory에서 만들어진 entity들을 넣어줘야 할까?

	return m_pEntities;
}
