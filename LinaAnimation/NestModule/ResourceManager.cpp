#define NOMINMAX	
#include "ResourceManager.h"
#include "Converter.h"
#include "PathManager.h"

ResourceManager::ResourceManager()
{
	std::cout << "���ҽ� �Ŵ��� ����" << std::endl;
	
	//����
	//std::wstring tempWStr = L"../NestResources/Model/box.fbx";
	//AddResource(tempWStr);
}

ResourceManager::~ResourceManager()
{

}

void GetFilePath(std::vector<std::tuple<std::wstring, std::wstring, ResourceType>> _temp)
{

}

///����� �ʱ�ȭ
void ResourceManager::Initialize()
{
	m_pPathManager = new PathManager();								 //PathManager ����
	//m_pTestConverter = new Converter();
	m_pPathManager->Initialize();									 //PathManager ����� �ʱ�ȭ
	m_pPathManager->LoadResource();									 //������ �ּҸ� �����Ѵ� .
	m_PathTuple = m_pPathManager->GetResourceTuple();				 //����� �ּҵ��� ��ȯ�Ѵ�.(�ּҰ�)
}

/// ��������� �޸𸮸� �����Ѵ�.
void ResourceManager::Finalize()
{
	//delete m_pPathManager;
}

std::wstring ResourceManager::GetResource(ResourceType _type, std::wstring _key)
{
	// _type�� int������ ��ȯ�Ѵ�.
	int vecNum = static_cast<int>(_type);


	// ���� : ResourceVector�� ������� �ʰ� ������ ����� �ʴ´�.
	if (!ResourceVector.empty() && vecNum < static_cast<int>(ResourceType::END))
	{
		// ResourceVector[vecNum]�� �ش��ϴ� unordered_map�� ã�´�.
		auto& resMap = ResourceVector[vecNum];

		// resMap�� _key�� �ִ��� �˻��Ѵ�.
		auto iter = resMap.find(_key);

		// ���ҽ��� ã�� �۾��� �����Ѵ�.
		if (iter != resMap.end())
		{
			// ���ҽ��� ���� ���
			return iter->second;
		}
		else
		{
			// ���ҽ��� ���� ���
			return L"���ҽ��� �����ϴ�.";
		}
	}
	else
	{
		// ResourceVector�� ����ְų� ������ ��� ���
		return L"�߸��� �����Դϴ�.";
	}
}

void ResourceManager::AddResource()
{
	//enum class resourceType ���� ��ŭ ���͸� �����.
	ResourceVector = std::vector<std::unordered_map<std::wstring, std::wstring>>(static_cast<int> (ResourceType::END));

	for (auto e : m_PathTuple)
	{
		if (std::get<2>(e) == L"cubemaps")
		{
			ResourceVector[static_cast<int>(ResourceType::cubemaps)].insert({ std::get<0>(e), std::get<1>(e) });
		}
		else if (std::get<2>(e) == L"hlsl")
		{
			ResourceVector[static_cast<int>(ResourceType::hlsl)].insert({ std::get<0>(e), std::get<1>(e) });
		}
		else if (std::get<2>(e) == L"image")
		{
			ResourceVector[static_cast<int>(ResourceType::image)].insert({ std::get<0>(e), std::get<1>(e) });
		}
		else if (std::get<2>(e) == L"Model")
		{
			ResourceVector[static_cast<int>(ResourceType::Model)].insert({ std::get<0>(e), std::get<1>(e) });
		}
		else if (std::get<2>(e) == L"skybox")
		{
			ResourceVector[static_cast<int>(ResourceType::skybox)].insert({ std::get<0>(e), std::get<1>(e) });
		}
		else if (std::get<2>(e) == L"Sound")
		{
			ResourceVector[static_cast<int>(ResourceType::Sound)].insert({ std::get<0>(e), std::get<1>(e) });
		}
		else
		{
			std::cout << "�߸��� ���ҽ� �Դϴ�." << std::endl;
		}
	}

	for (int i = 0; i < static_cast<int>(ResourceType::END); i++)
	{
		std::wcout << i << "ResourceType-------------------------------- " << std::endl;

		for (auto& e : ResourceVector[i])
		{
			std::wcout << "Key : " << e.first << " Value : " << e.second << std::endl;
		}
	}
}

NestGraphics::ModelData* ResourceManager::GetFBXConvert(std::wstring _Key)	///***�̺κ� ��ȯ�� ����� �� �Ͻð�. shared ������ ������ ������ �ƴ� �׳� ������
{
	//m_pTestConverter->ReadFile(ResourceVector[static_cast<int>(ResourceType::Model)])
 	m_pTestConverter = new Converter();									  ///Converter �κ� �ڵ� ���ƾ���� ���� �� ���

	std::unordered_map<std::wstring, std::wstring>::const_iterator it = ResourceVector[static_cast<int>(ResourceType::Model)].find(_Key);

	if (it != ResourceVector[static_cast<int>(ResourceType::Model)].end())
	{
		// Ű�� ã������, Converter�� ���� ������.

		return m_pTestConverter->ReadFile(it->first, it->second);	///�̺κе� �� �����غ���

	}
	else
	{
		std::cout << "Ű��.. �����ϴ�.\n";
		return nullptr;
	}

 	delete m_pTestConverter;
}

