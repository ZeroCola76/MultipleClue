#include "JsonManager.h"

JsonManager::JsonManager()
{
	std::cout << "JsonManager 생성" << std::endl;
	m_readerBuilder;
	m_pReader = m_readerBuilder.newCharReader();

	// 불러오기 및 변환이 잘 되는지 테스트
	std::cout << "Json Test start" << std::endl;
	LoadFromFile("../NestResources/Json/Test_0201.json");
	for (const auto& entity : m_pEntities)
	{
		std::cout << entity.name << std::endl;
		for (const auto& comp : entity.components)
		{
			std::cout << comp << std::endl;
		}
	}
	std::cout << "Json Test end" << std::endl;
}

JsonManager::~JsonManager()
{

}

void JsonManager::Initialize()
{
}

void JsonManager::Update()
{
}

void JsonManager::Finalize()
{

}

/// <summary>
/// Json파일을 로드하고, 파싱하는 함수입니다.
/// </summary>
/// <param name="file">파일경로+파일명</param>
void JsonManager::LoadFromFile(const std::string file)
{
	std::ifstream inputFile(file);

	if (inputFile.is_open())
	{
		Json::Value data;
		Json::parseFromStream(m_readerBuilder, inputFile, &data, nullptr);

		ConvertJsonToString(data);
	}
}

/// <summary>
/// Json::Value객체를 EntityString객체로 변환시키는 함수입니다.
/// 2024.02.01 TODO : 런타임 오류 수정할것 
/// </summary>
/// <param name="value">파싱한 객체</param>
void JsonManager::ConvertJsonToString(const Json::Value& value)
{
	const Json::Value& object = value["objs"];

	for (const auto& obj : object)
	{
		EntityString entityString;
		entityString.name = obj["name"].asString();

		const Json::Value& component = obj["comp"];

		for (const auto& comp : component)
		{
			entityString.components.push_back(comp.asString());
		}

		m_pEntities.push_back(entityString);
	}
}
