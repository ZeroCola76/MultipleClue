#include "PathManager.h"

PathManager::PathManager()
{
	std::cout << "PathManager �ʱ�ȭ" << std::endl;
}

PathManager::~PathManager()
{

}

// ��� �Ŵ����� �ʱ�ȭ�Ѵ�.
void PathManager::Initialize()
{
	// ���� ��� ������
	m_currentPath = std::filesystem::current_path();

	// ���� ��� ������
	m_rootPath = m_currentPath.parent_path();

	// ���ҽ� ��ġ�� �̵��ϱ�
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
	// ���� ��� ������
	m_currentPath = std::filesystem::current_path();

	// ���� ��� ������
	m_rootPath = m_currentPath.parent_path();

	// NestReources ���� �ִ� ������ �ֱ� / NestReources���� ��� ������ vector<string>�� �ִ´�.
	for (const auto& entry : std::filesystem::directory_iterator(m_rootPath / "NestResources"))
	{
		std::filesystem::path path(entry);
		m_resourceFolderName.push_back(path.filename().string());
	}

	// m_resourceTuple�� ���ϸ�-���-���������� �̷���� Tuple �ֱ�
	for (const auto& folderName : m_resourceFolderName)
	{
		for (const auto& entry : std::filesystem::directory_iterator(m_rootPath / ("NestResources\\" + folderName)))
		{
			std::filesystem::path path(entry);
			std::wstring fileName = path.filename().wstring();
			m_resourceTuple.push_back(make_tuple(fileName, path, path.parent_path().filename().wstring()));
		}
	}

	// ��� ��� �׽�Ʈ��
	for (const auto& resource : m_resourceTuple)
	{
		// Ʃ���� �� ��ҿ� ����
		const auto& fileName = std::get<0>(resource);
		const auto& path = std::get<1>(resource);
		const auto& folderName = std::get<2>(resource);

		// ���
// 		std::wcout << fileName << L", ";
// 		std::wcout << path << L", ";
// 		std::wcout << folderName << std::endl;
	}
}

// ���ҽ� ��� �������ֱ�
// ���ҽ� ����� ���ϰ� ���ҽ��Ŵ���, ���ҽ� �Ŵ����� ��ģ ���ҽ����� �������� ����Ҽ� �ְ� �Ұ��ΰ�. ��� -> 01.29.
std::wstring PathManager::GetResourcePath(const std::wstring& _filename)
{
	std::wstring fullPath = m_rootPath.c_str() + m_resourceDir + _filename;
	return fullPath;
}

//vector�� �ּҰ��� ��ȯ�Ѵ�. 
std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> PathManager::GetResourceTuple()
{
	return m_resourceTuple;
}

