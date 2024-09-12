#pragma once
#include <iostream>
#include "EntityBuilder.h"
#include "AllComponentHeader.h"
EntityBuilder::EntityBuilder()
{

}

EntityBuilder::~EntityBuilder()
{

}

std::shared_ptr<IComponent> EntityBuilder::BuildEntity(std::string _comName)
{
	if (_comName == "TRSComponent")
	{
		ReturnComponent = std::make_shared<TRSComponent>();
	}
	else if (_comName == "GraphicsComponent")
	{
		ReturnComponent = std::make_shared<GraphicsComponent>();
	}
	else if (_comName == "BoxColliderComponent")
	{
		ReturnComponent = std::make_shared<BoxColliderComponent>();
	}
	else if (_comName == "SphereColliderComponent")
	{
		ReturnComponent = std::make_shared<SphereColliderComponent>();
	}
	else
	{
		std::cout << _comName <<" 이름과 맞는 Component가 없습니다.\n";
		return nullptr;
	}
	return ReturnComponent;
}
