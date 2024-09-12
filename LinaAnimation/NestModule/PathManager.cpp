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
	m_resourceDir = L"\\NestResources\\";
}	

void PathManager::Update()
{

}	

void PathManager::Finalize()
{

}

void PathManager::LoadResource()
{
	// 현재 경로 얻어오기
	m_currentPath = std::filesystem::current_path();

	// 상위 경로 얻어오기
	m_rootPath = m_currentPath.parent_path();

	// NestReources 내에 있는 폴더명 넣기 / NestReources내의 모든 폴더를 vector<string>에 넣는다.
	for (const auto& entry : std::filesystem::directory_iterator(m_rootPath / "NestResources"))
	{
		std::filesystem::path path(entry);
		m_resourceFolderName.push_back(path.filename().string());
	}

	// m_resourceTuple에 파일명-경로-폴더명으로 이루어진 Tuple 넣기
	for (const auto& folderName : m_resourceFolderName)
	{
		for (const auto& entry : std::filesystem::directory_iterator(m_rootPath / ("NestResources\\" + folderName)))
		{
			std::filesystem::path path(entry);
			std::wstring fileName = path.filename().wstring();
			m_resourceTuple.push_back(make_tuple(fileName, path, path.parent_path().filename().wstring()));
		}
	}

	// 출력 결과 테스트용
	for (const auto& resource : m_resourceTuple)
	{
		// 튜플의 각 요소에 접근
		const auto& fileName = std::get<0>(resource);
		const auto& path = std::get<1>(resource);
		const auto& folderName = std::get<2>(resource);

		// 출력
// 		std::wcout << fileName << L", ";
// 		std::wcout << path << L", ";
// 		std::wcout << folderName << std::endl;
	}
}

// 리소스 경로 리턴해주기
// 리소스 경로의 리턴과 리소스매니저, 리소스 매니저를 거친 리소스들을 어떤방식으로 사용할수 있게 할것인가. 고민 -> 01.29.
std::wstring PathManager::GetResourcePath(const std::wstring& _filename)
{
	std::wstring fullPath = m_rootPath.c_str() + m_resourceDir + _filename;
	return fullPath;
}

//vector의 주소값을 반환한다. 
std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> PathManager::GetResourceTuple()
{
	return m_resourceTuple;
}

