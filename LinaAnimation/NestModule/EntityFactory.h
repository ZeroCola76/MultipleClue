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
	/// 엔티티를 만든다. [엔티티명, 엔티티가 가질 컴포넌트의 vector_string>]
	std::shared_ptr<Entity> CreateEntity(std::string _entityName, std::vector<std::string> _strVec);
};