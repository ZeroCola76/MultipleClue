#pragma once
#include "Resource.h"
#include <unordered_map>
#include <memory>

class Resource;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void Load();
	void Unload();

private:
	std::unordered_map<std::wstring, std::shared_ptr<Resource>> m_resources;

};

