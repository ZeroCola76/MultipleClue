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
/// JsonCPP 라이브러리를 사용하여 Json 파일을 읽고 쓸 수 있도록 클래스로 만들었습니다.
/// 이 라이브러리 외부에서는 JsonCPP 라이브러리를 몰라도 되도록 설계할 예정입니다.
/// -> 어차피 다른 사람들은 안쓰니까 그냥 오픈해도 될것같네요.
/// Json::Value 객체는 Key값과 Value값으로 이루어져 있으며
/// Key값을 Value값처럼 사용하여 계층구조를 만들 수 있습니다.
/// /// 2024.02.01
/// 구조체를 새로 만들어서 모든 value들을 string의 형태로 보관하도록 재설계 시작하였다.
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