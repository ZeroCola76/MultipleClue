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
/// ���ӿ� ���� ���ҽ����� �����ϴ� �Ŵ���
/// ���߿� imanager�κ��� ��ӹ��� �͵��� �մ���.
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

	//pathManager���� ���� ���� �����ϴ� ���� tuple<���ϸ�, ���, ������>���� ����
	std::vector<std::tuple<std::wstring, std::wstring, std::wstring>> m_PathTuple;

	//pathManager���� ���� ���� Ű ��(���ϸ�)�� ��θ����� ���� ����.
	std::vector<std::unordered_map<std::wstring, std::wstring>> ResourceVector;

public:
	void AddResource();

	NestGraphics::ModelData* GetFBXConvert(std::wstring _Key);

	// ���ҽ� Ÿ�԰� ���ϸ��� �޾ƿͼ� ���ҽ��� ã�� �Լ�
	std::wstring GetResource(ResourceType _type, std::wstring _key);

};
