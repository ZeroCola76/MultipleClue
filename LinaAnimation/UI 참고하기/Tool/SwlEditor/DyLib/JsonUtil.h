#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"

/// <summary>
/// 2024.01.23
/// JsonCPP 라이브러리를 사용하여 Json 파일을 읽고 쓸 수 있도록 클래스로 만들었습니다.
/// 이 라이브러리 외부에서는 JsonCPP 라이브러리를 몰라도 되도록 설계할 예정입니다.
/// -> 어차피 다른 사람들은 안쓰니까 그냥 오픈해도 될것같네요.
/// Json::Value 객체는 Key값과 Value값으로 이루어져 있으며
/// Key값을 Value값처럼 사용하여 계층구조를 만들 수 있습니다.
/// </summary>
class JsonUtil
{
public:
	JsonUtil();
	~JsonUtil();

	void SaveData(const std::wstring& file);
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