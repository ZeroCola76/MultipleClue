#pragma once
#include <memory>
#include <vector>

/// <summary>
/// �������̽� ��
/// </summary>

class Entity;
class EntityFactory;
class ObjectManager;

class Scene
{
public:
	Scene();
	~Scene();

 	// ���� Ȱ��ȭ�ϴ� �Լ�
	virtual void Initialize();

	// ���� ������Ʈ�ϴ� �Լ�
	virtual void Update();

	// ������ �׷��� �� ��ü���� �������ϴ� �Լ�
	virtual void Render();

	// ���� ��Ȱ��ȭ�ϴ� �Լ�
	virtual void Finalize();

	// ��ƼƼ vector�� �ҷ����� �Լ�
	std::vector<std::shared_ptr<Entity>> GetEntityVector();

private:
	// ��ƼƼ�� ������ vector
	std::vector<std::shared_ptr<Entity>> m_pEntities;
	
	// Ȱ��ȭ ����
	bool m_isActive;
};