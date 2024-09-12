#include "PathManager.h"

PathManager::PathManager()
{
	std::cout << "PathManager 초기화" << std::endl;
}

PathManager::~PathManager()
{

}

// 경로 매니저를 초기화한다.
void PathManager::Initialize()
{
	// 현재 경로 얻어오기
	m_currentPath = std::filesystem::current_path();

	// 상위 경로 얻어오기
	m_rootPath = m_currentPath.parent_path();

	// 리소스 위치로 이동하기
	m_resourcePath = m_rootPath / L"NestResources";

	// 이동한 리소스 위치 출력
	OutputDebugString(m_resourcePath.c_str());
	OutputDebugString(L"\n");
}

void PathManager::Update()
{

}	

void PathManager::Finalize()
{

}

// 리소스 경로 리턴해주기
std::wstring PathManager::GetResourcePath(const std::wstring& _filepath)
{
	std::filesystem::path fullPath = m_resourcePath;
	fullPath /= _filepath;


	return fullPath.wstring();
}