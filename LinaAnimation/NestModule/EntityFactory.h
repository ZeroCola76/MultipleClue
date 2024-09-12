#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Entity;
class EntityBuilder;

class EntityFactory
{
public:
	EntityFactory();
	~EntityFactory();
private:
	EntityBuilder* m_pEntityBuilder;
public:
	/// ��ƼƼ�� �����. [��ƼƼ��, ��ƼƼ�� ���� ������Ʈ�� vector_string>]
	std::shared_ptr<Entity> CreateEntity(std::string _entityName, std::vector<std::string> _strVec);
};