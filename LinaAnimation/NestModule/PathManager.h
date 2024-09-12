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
/// 리소스 경로를 관리하기 위해 만든 매니저
/// </summary>

class PathManager : public IManager
{
public:
	PathManager();
	~PathManager();

private:
	std::filesystem::path m_rootPath;			// 상위 경로
	std::filesystem::path m_currentPath;		// 현재 경로
	std::wstring m_resourceDir;					// 리소스 위치

	// NestResources 폴더 안에 있는 폴더명
	std::vector<std::string> m_resourceFolderName;

	// [파일명(키): 폴더명(값)]으로 이루어진 unordered_map.
	std::unordered_map<std::wstring, std::wstring> m_resourceMap;

	// [파일명|경로|폴더명]으로 이루어진 tuple로 묶은 vector.
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> m_resourceTuple;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

	void LoadResource();
public:
	//파일명을 넣으면 리소스 경로를 리턴한다 .
	std::wstring GetResourcePath(const std::wstring& _filename);

	//벡터의 주소값을 반환한다 .
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> GetResourceTuple();
};