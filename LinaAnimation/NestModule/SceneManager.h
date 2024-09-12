#pragma once
#include <memory>
#include <unordered_map>
#include "IManager.h"

/// <summary>
/// 여러 개의 Scene들을 관리하는 매니저
/// </summary>

class Scene;
class Entity;
class InputManager;
class SoundManager;
class ObjectManager;
class ResourceManager;

class SceneManager : public IManager
{
public:
	SceneManager();
	~SceneManager();

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

public:
	void SetTempEntityVector(std::vector<std::shared_ptr<Entity>> entities) { m_Entities = entities; }

private:
	// 씬들을 모아놓은 벡터
	std::vector<Scene> m_Scenes;

	// 엔티티들을 모아놓은 벡터
	std::vector<std::shared_ptr<Entity>> m_Entities;
};