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
	// ObjectManager�� �ִ� entity���� �־���� �ϳ�? <- �̰� SceneManager�� ����ϹǷ� Ż��.
	// �׷��ٸ� EntityFactory���� ������� entity���� �־���� �ұ�?

	return m_pEntities;
}
