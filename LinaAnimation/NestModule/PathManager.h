#pragma once
#include "IManager.h"
#include "ResourceEnum.h"
#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <cassert>
/// <summary>
/// ���ҽ� ��θ� �����ϱ� ���� ���� �Ŵ���
/// </summary>

class PathManager : public IManager
{
public:
	PathManager();
	~PathManager();

private:
	std::filesystem::path m_rootPath;			// ���� ���
	std::filesystem::path m_currentPath;		// ���� ���
	std::wstring m_resourceDir;					// ���ҽ� ��ġ

	// NestResources ���� �ȿ� �ִ� ������
	std::vector<std::string> m_resourceFolderName;

	// [���ϸ�(Ű): ������(��)]���� �̷���� unordered_map.
	std::unordered_map<std::wstring, std::wstring> m_resourceMap;

	// [���ϸ�|���|������]���� �̷���� tuple�� ���� vector.
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> m_resourceTuple;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

	void LoadResource();
public:
	//���ϸ��� ������ ���ҽ� ��θ� �����Ѵ� .
	std::wstring GetResourcePath(const std::wstring& _filename);

	//������ �ּҰ��� ��ȯ�Ѵ� .
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> GetResourceTuple();
};