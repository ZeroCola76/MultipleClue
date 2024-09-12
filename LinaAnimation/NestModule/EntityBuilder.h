#pragma once
#include <memory>

class IComponent;

class EntityBuilder
{
public:
	EntityBuilder();
	~EntityBuilder();
public:
	std::shared_ptr<IComponent> ReturnComponent;

	std::shared_ptr<IComponent> BuildEntity(std::string _comName);

};

