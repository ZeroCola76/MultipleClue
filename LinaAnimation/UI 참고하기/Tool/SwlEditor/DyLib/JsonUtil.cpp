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
/// "file"에 m_data의 데이터를 저장하는 함수입니다.
/// </summary>
/// <param name="file">파일명</param>
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
/// "file"에서 m_data로 데이터를 불러오는 함수입니다.
/// </summary>
/// <param name="file">파일명</param>
void JsonUtil::LoadData(const std::wstring& file)
{
	std::ifstream inputFile(filePath + file);

	if (inputFile.is_open())
	{
		Json::parseFromStream(m_readerBuilder, inputFile, &m_data, nullptr);
	}
}
