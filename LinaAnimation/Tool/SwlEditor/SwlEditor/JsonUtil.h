#pragma once
#include <iostream>
#include <fstream>
#include "framework.h"

enum class JsonValueType
{
	STRING,
	INT,
	FLOAT,
	BOOLEAN,
	OBJECT,
	ARRAY,
	NULLTYPE
};

struct JsonValue
{
	JsonValue() : key(), value(), type(JsonValueType::NULLTYPE) {	}
	JsonValue(std::string key, std::string value, JsonValueType type)
		: key(key), value(value), type(JsonValueType::NULLTYPE) {	}
	~JsonValue() {}

	std::string key;
	std::string value;
	JsonValueType type;
	std::vector<JsonValue> children;
};

/// <summary>
/// 2024.01.23
/// JsonCPP ���̺귯���� ����Ͽ� Json ������ �а� �� �� �ֵ��� Ŭ������ ��������ϴ�.
/// �� ���̺귯�� �ܺο����� JsonCPP ���̺귯���� ���� �ǵ��� ������ �����Դϴ�.
/// -> ������ �ٸ� ������� �Ⱦ��ϱ� �׳� �����ص� �ɰͰ��׿�.
/// Json::Value ��ü�� Key���� Value������ �̷���� ������
/// Key���� Value��ó�� ����Ͽ� ���������� ���� �� �ֽ��ϴ�.
/// /// 2024.02.01
/// ����ü�� ���� ���� ��� value���� string�� ���·� �����ϵ��� �缳�� �����Ͽ���.
/// </summary>
class JsonUtil
{
public:
	JsonUtil();
	~JsonUtil();

	void SaveData();
	void SaveData(std::wstring& filePath);
	void LoadData(const std::wstring& file);

	void SetData(const Json::Value& val) { m_data = val; }
	Json::Value GetData() const { return m_data; }

private:
	Json::StreamWriterBuilder m_writerBuilder;
	Json::CharReaderBuilder m_readerBuilder;
	Json::StreamWriter* m_pWriter;
	Json::CharReader* m_pReader;
	Json::Value m_data;
	std::wstring filePath;
};