#pragma once
#include "ResourceBase.h"

namespace GameEngine
{
	class Shader : public ResourceBase
	{
	public:
		Shader(ResourceType type) : ResourceBase(type) {}
		~Shader() {}

	private:
		std::wstring filePath;

	public:
		void SetFilePath(const std::wstring& _filepath) { filePath = _filepath; }
		std::wstring GetFilePath() { return filePath; }
	};

}
