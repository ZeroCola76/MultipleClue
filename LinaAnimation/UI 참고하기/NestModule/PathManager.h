#pragma once
#include "IManager.h"
#include "ResourceEnum.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <filesystem>
/// <summary>
/// ���ҽ� ��θ� �����ϱ� ���� ���� �Ŵ���

/// </summary>

class PathManager : public IManager
{
public:
	PathManager();
	~PathManager();

private:
	std::filesystem::path m_rootPath;
	std::filesystem::path m_currentPath;
	std::wstring m_resourcePath;
	std::wstring m_fileName;
	std::unordered_map<std::wstring, ResourceType> m_resourceMap;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

public:
	std::wstring GetResourcePath(const std::wstring& _filepath);
};