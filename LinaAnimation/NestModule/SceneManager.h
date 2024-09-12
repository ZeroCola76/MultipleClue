#pragma once
#include <memory>
#include <unordered_map>
#include "IManager.h"

/// <summary>
/// ���� ���� Scene���� �����ϴ� �Ŵ���
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
	// ������ ��Ƴ��� ����
	std::vector<Scene> m_Scenes;

	// ��ƼƼ���� ��Ƴ��� ����
	std::vector<std::shared_ptr<Entity>> m_Entities;
};