#pragma once
#include <memory>

template <typename T>
class ObjectFactory
{
	std::shared_ptr<T> CreateObject()
	{
		return std::make_shared<T>();
	}

};