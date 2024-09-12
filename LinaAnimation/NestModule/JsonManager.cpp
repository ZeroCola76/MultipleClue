#include "JsonManager.h"

JsonManager::JsonManager()
{
	std::cout << "JsonManager ����" << std::endl;
	m_readerBuilder;
	m_pReader = m_readerBuilder.newCharReader();

	// �ҷ����� �� ��ȯ�� �� �Ǵ��� �׽�Ʈ
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
/// Json������ �ε��ϰ�, �Ľ��ϴ� �Լ��Դϴ�.
/// </summary>
/// <param name="file">���ϰ��+���ϸ�</param>
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
/// Json::Value��ü�� EntityString��ü�� ��ȯ��Ű�� �Լ��Դϴ�.
/// 2024.02.01 TODO : ��Ÿ�� ���� �����Ұ� 
/// </summary>
/// <param name="value">�Ľ��� ��ü</param>
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
