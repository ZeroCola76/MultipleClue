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
	m_resourcePath = m_rootPath / L"NestResources";

	// �̵��� ���ҽ� ��ġ ���
	OutputDebugString(m_resourcePath.c_str());
	OutputDebugString(L"\n");
}

void PathManager::Update()
{

}	

void PathManager::Finalize()
{

}

// ���ҽ� ��� �������ֱ�
std::wstring PathManager::GetResourcePath(const std::wstring& _filepath)
{
	std::filesystem::path fullPath = m_resourcePath;
	fullPath /= _filepath;


	return fullPath.wstring();
}