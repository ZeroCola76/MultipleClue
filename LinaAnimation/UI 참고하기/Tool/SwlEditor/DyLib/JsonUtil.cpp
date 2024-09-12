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
/// <param name="file">���ϸ�</param>
void JsonUtil::SaveData(const std::wstring& file)
{
	std::ofstream outputFile(filePath + file);

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
