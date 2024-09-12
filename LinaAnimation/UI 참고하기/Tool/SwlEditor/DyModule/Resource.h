#pragma once
#include <algorithm>
#include <string>

class Resource abstract
{
public:
	Resource() {}
	virtual ~Resource() {}

	virtual void Load() abstract;
	virtual void Umload() abstract;

};

class Texture : public Resource
{

};

class Buffer : public Resource
{

};

class Shader : public Resource
{

};