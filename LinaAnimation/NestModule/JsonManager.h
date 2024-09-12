#pragma once
#include "IManager.h"
#include "json/json.h"
#include <fstream>
#include <iostream>

/// <summary>
/// 임시로 만들어놓은 Entity의 이름과 컴포넌트들의 이름을 넣을 구조체입니다.
/// </summary>
struct EntityString
{
	std::string name;
	std::vector<std::string> components;
};

/// <summary> 
/// 씬 생성시 툴에서 만든 Json파일을 통해 오브젝트들을 배치할 수 있도록하는 매니저입니다.
/// Json::Value객체를 std::string객체로 변환시킵니다.
/// Json파일은 리소스 매니저에서 관리하도록 설계할 예정입니다._2024.01.30
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

