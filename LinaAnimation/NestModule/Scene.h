#pragma once
#include <memory>
#include <vector>

/// <summary>
/// 인터페이스 씬
/// </summary>

class Entity;
class EntityFactory;
class ObjectManager;

class Scene
{
public:
	Scene();
	~Scene();

 	// 씬을 활성화하는 함수
	virtual void Initialize();

	// 씬을 업데이트하는 함수
	virtual void Update();

	// 씬에서 그려야 할 객체들을 렌더링하는 함수
	virtual void Render();

	// 씬을 비활성화하는 함수
	virtual void Finalize();

	// 엔티티 vector를 불러오는 함수
	std::vector<std::shared_ptr<Entity>> GetEntityVector();

private:
	// 엔티티를 저장할 vector
	std::vector<std::shared_ptr<Entity>> m_pEntities;
	
	// 활성화 여부
	bool m_isActive;
};