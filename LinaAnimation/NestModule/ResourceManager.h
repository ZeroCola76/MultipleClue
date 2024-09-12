#pragma once
#include <unordered_map>
#include <array>
#include <memory>
#include <vector>
#include <tuple>
#include "IManager.h"
#include "ResourceEnum.h"
#include "ResourceBase.h"
#include "Mesh.h"
#include "Shader.h"
#include "AllComponentHeader.h"

#define NOMINMAX	
class Converter;
class PathManager;

/// <summary>
/// 게임에 쓰일 리소스들을 관리하는 매니져
/// 나중에 imanager로부터 상속받은 것들을 손대자.
/// </summary>
class ResourceManager : public IManager
{
public:
	ResourceManager();
	~ResourceManager();
public:
	virtual void Initialize() override;
	virtual void Update() override {};
	virtual void Finalize() override;

private:
	//assimp - Converter
	Converter* m_pTestConverter;

	// PathManager
	PathManager* m_pPathManager;

	//pathManager에서 받은 값을 저장하는 변수 tuple<파일명, 경로, 폴더명>으로 구성
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> m_PathTuple;

	//pathManager에서 받은 값을 키 값(파일명)과 경로만으로 나눠 정리.
	std::vector<std::unordered_map<std::wstring, std::wstring>> ResourceVector;

public:
	void AddResource();

	NestGraphics::ModelData* GetFBXConvert(std::wstring _Key);

	// 리소스 타입과 파일명을 받아와서 리소스를 찾는 함수
	std::wstring GetResource(ResourceType _type, std::wstring _key);

};
