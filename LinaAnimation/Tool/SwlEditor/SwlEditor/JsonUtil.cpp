#include "JsonUtil.h"

JsonUtil::JsonUtil()
{
	m_pWriter = m_writerBuilder.newStreamWriter();
	m_pReader = m_readerBuilder.newCharReader();
	filePath = L"../Document/";

}

JsonUtil::~JsonUtil()
{
	delete m_pWriter;
	delete m_pReader;
}

/// <summary>
/// "file"�� m_data�� �����͸� �����ϴ� �Լ��Դϴ�.
/// </summary>
void JsonUtil::SaveData()
{
	static int i = 0;

	std::wstring fileName = L"SaveData" + std::to_wstring(i) + L".json";
	std::ofstream outputFile(filePath + fileName);

	if (outputFile.is_open())
	{
		m_pWriter->write(m_data, &outputFile);

		outputFile.close();
	}

	i++;
}

/// <summary>
/// ���� ��θ� �Ű� ������ �޾� �����ϴ� �Լ��Դϴ�.
/// </summary>
/// <param name="filePath">���� ���</param>
void JsonUtil::SaveData(std::wstring& filePath)
{
	std::wstring fileName = filePath /*+ L".json"*/;
	std::ofstream outputFile(fileName);

	if (outputFile.is_open())
	{
		m_pWriter->write(m_data, &outputFile);

		outputFile.close();
	}
}

/// <summary>
/// "file"���� m_data�� �����͸� �ҷ����� �Լ��Դϴ�.
/// </summary>
/// <param name="file">���ϸ�</param>
void JsonUtil::LoadData(const std::wstring& file)
{
	std::ifstream inputFile(filePath + file);

	if (inputFile.is_open())
	{
		Json::parseFromStream(m_readerBuilder, inputFile, &m_data, nullptr);
	}
}
