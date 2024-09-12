#pragma once
#include "IManager.h"
#include "json/json.h"
#include <fstream>
#include <iostream>

/// <summary>
/// �ӽ÷� �������� Entity�� �̸��� ������Ʈ���� �̸��� ���� ����ü�Դϴ�.
/// </summary>
struct EntityString
{
	std::string name;
	std::vector<std::string> components;
};

/// <summary> 
/// �� ������ ������ ���� Json������ ���� ������Ʈ���� ��ġ�� �� �ֵ����ϴ� �Ŵ����Դϴ�.
/// Json::Value��ü�� std::string��ü�� ��ȯ��ŵ�ϴ�.
/// Json������ ���ҽ� �Ŵ������� �����ϵ��� ������ �����Դϴ�._2024.01.30
/// </summary>
class JsonManager : public IManager
{
public:
	JsonManager();
	virtual ~JsonManager();

private:
	Json::CharReaderBuilder m_readerBuilder;
	Json::CharReader* m_pReader;
	std::vector<EntityString> m_pEntities;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

	void LoadFromFile(const std::string file);

private:
	void ConvertJsonToString(const Json::Value& value);

};

