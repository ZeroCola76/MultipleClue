#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"

/// <summary>
/// 2024.01.23
/// JsonCPP ���̺귯���� ����Ͽ� Json ������ �а� �� �� �ֵ��� Ŭ������ ��������ϴ�.
/// �� ���̺귯�� �ܺο����� JsonCPP ���̺귯���� ���� �ǵ��� ������ �����Դϴ�.
/// -> ������ �ٸ� ������� �Ⱦ��ϱ� �׳� �����ص� �ɰͰ��׿�.
/// Json::Value ��ü�� Key���� Value������ �̷���� ������
/// Key���� Value��ó�� ����Ͽ� ���������� ���� �� �ֽ��ϴ�.
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