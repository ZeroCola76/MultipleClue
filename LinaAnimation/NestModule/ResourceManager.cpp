#define NOMINMAX	
#include "ResourceManager.h"
#include "Converter.h"
#include "PathManager.h"

ResourceManager::ResourceManager()
{
	std::cout << "리소스 매니저 생성" << std::endl;
	
	//예시
	//std::wstring tempWStr = L"../NestResources/Model/box.fbx";
	//AddResource(tempWStr);
}

ResourceManager::~ResourceManager()
{

}

void GetFilePath(std::vector<std::tuple<std::wstring, std::wstring, ResourceType>> _temp)
{

}

///명시적 초기화
void ResourceManager::Initialize()
{
	m_pPathManager = new PathManager();								 //PathManager 생성
	//m_pTestConverter = new Converter();
	m_pPathManager->Initialize();									 //PathManager 명시적 초기화
	m_pPathManager->LoadResource();									 //파일의 주소를 저장한다 .
	m_PathTuple = m_pPathManager->GetResourceTuple();				 //저장된 주소들을 반환한다.(주소값)
}

/// 명시적으로 메모리를 정리한다.
void ResourceManager::Finalize()
{
	//delete m_pPathManager;
}

std::wstring ResourceManager::GetResource(ResourceType _type, std::wstring _key)
{
	// _type을 int형으로 변환한다.
	int vecNum = static_cast<int>(_type);


	// 조건 : ResourceVector가 비어있지 않고 범위를 벗어나지 않는다.
	if (!ResourceVector.empty() && vecNum < static_cast<int>(ResourceType::END))
	{
		// ResourceVector[vecNum]에 해당하는 unordered_map을 찾는다.
		auto& resMap = ResourceVector[vecNum];

		// resMap에 _key가 있는지 검색한다.
		auto iter = resMap.find(_key);

		// 리소스를 찾는 작업을 수행한다.
		if (iter != resMap.end())
		{
			// 리소스가 있을 경우
			return iter->second;
		}
		else
		{
			// 리소스가 없는 경우
			return L"리소스가 없습니다.";
		}
	}
	else
	{
		// ResourceVector가 비어있거나 범위를 벗어난 경우
		return L"잘못된 접근입니다.";
	}
}

void ResourceManager::AddResource()
{
	//enum class resourceType 개수 만큼 벡터를 만든다.
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
			std::cout << "잘못된 리소스 입니다." << std::endl;
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

NestGraphics::ModelData* ResourceManager::GetFBXConvert(std::wstring _Key)	///***이부분 반환값 고민을 좀 하시고. shared 쓸건지 포인터 쓸껀지 아님 그냥 갈껀디
{
	//m_pTestConverter->ReadFile(ResourceVector[static_cast<int>(ResourceType::Model)])
 	m_pTestConverter = new Converter();									  ///Converter 부분 코드 고쳤어야지 정신 좀 잡아

	std::unordered_map<std::wstring, std::wstring>::const_iterator it = ResourceVector[static_cast<int>(ResourceType::Model)].find(_Key);

	if (it != ResourceVector[static_cast<int>(ResourceType::Model)].end())
	{
		// 키를 찾았으면, Converter로 값을 보낸다.

		return m_pTestConverter->ReadFile(it->first, it->second);	///이부분도 더 생각해보고

	}
	else
	{
		std::cout << "키가.. 없습니다.\n";
		return nullptr;
	}

 	delete m_pTestConverter;
}

