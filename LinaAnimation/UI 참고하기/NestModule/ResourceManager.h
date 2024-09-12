#pragma once
#include <unordered_map>
#include <array>
#include <memory>
#include <vector>
#include "IManager.h"
#include "ResourceBase.h"
#include "Mesh.h"
#include "Shader.h"

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
	virtual void Initialize() override {};
	virtual void Update() override {};
	virtual void Finalize() override {};
private:
	//KeyObjMap�̶�� ��Ī�� ������ش�. <Ű ��, shared_ptr>�� �����ȴ�.
	using KeyObjMap = std::unordered_map<std::wstring, std::shared_ptr<ResourceBase>>;

	//�迭�� �����. 
	std::array<KeyObjMap, static_cast<int>(ResourceType::END)> m_Resources;


public:
	// ���ҽ� ��� ��û�� �ϴ� �Լ�, ������ �� ���ҽ��� �ٸ��� �� ����� ���� �ǵ��� shared_ptr�� ����.
	template<typename T>
	bool Add(const std::wstring& key, std::shared_ptr<T> object);

	// Ű���� ���� �׿� �ش��ϴ� ���ҽ��� ã�� �Լ�
	template<typename T>
	std::shared_ptr<T> Get(const std::wstring& key);

	// Ÿ���� ���� ���ҽ� ã�� ���� �����ϴ� �Լ�
	template<typename T>
	ResourceType GetResourceType();

};

///template�� cpp�� �����ϸ� �ȵȴ�.---------------------------------------------------------------
// ���ҽ��� �߰��ϴ� �Լ� <Key��, shared_ptr>
template<typename T>
bool  ResourceManager::Add(const std::wstring& key, std::shared_ptr<T> object)
{
	//enum class�κ��� Ÿ���� �޾ƿ´�.
	ResourceType resourceType = GetResourceType<T>();

	//resourceType�� �ش��ϴ� KeyObjMap�� m_Resources �迭���� �����´�.(enumClass�������)
	KeyObjMap& keyObjMap = m_Resources[static_cast<int>(resourceType)];

	//���� Ű(key)�� keyObjMap�� �̹� �����ϴ��� �˻�-------------------
	auto findIter = keyObjMap.find(key);

	// �ش� Ű�� ��ϵ� ���ҽ� Ÿ���� �̹� �ִ��� �Ǵ� -> ������ ����
	if (findIter != keyObjMap.end()) { return false; }

	//���ٸ� keyObjMap�� ���ο� ���ҽ��� �߰�
	keyObjMap[key] = object;

	return true;
}

//Ű���� ���� �׿� �ش��ϴ� ���ҽ��� ã�� ����
template<typename T>
std::shared_ptr<T> ResourceManager::Get(const std::wstring& key)
{
	//enum class�κ��� Ÿ���� �޾ƿ´�.
	ResourceType resourceType = GetResourceType<T>();

	// ���ҽ� Ÿ���� ������ ��ȯ�ؼ� ã��
	KeyObjMap& keyObjMap = m_Resources[static_cast<int>(resourceType)];

	//���� Ű(key)�� keyObjMap�� �̹� �����ϴ��� �˻�-------------------
	auto findIter = keyObjMap.find(key);

	//findIt�� end()�� �ƴ� ��� -> ����
	if (findIter != keyObjMap.end())
	{
		return std::static_pointer_cast<T>(findIter->second);
	}

	//���� ��� null��ȯ
	return nullptr;
}

template<typename T>
ResourceType ResourceManager::GetResourceType()
{
	// ���ҽ� Ÿ���� ��ġ�ϴ°�
	if (std::is_same_v<T, GameEngine::Mesh>) 
	{
		std::cout << "MESH" << std::endl;
		return ResourceType::MESH; 
	}
 	else if (std::is_same_v<T, GameEngine::Shader>)
	{
		std::cout << "SHADER" << std::endl;
		return ResourceType::SHADER; 
	}
// 	else if (std::is_same_v<T, GameEngine::Texture>) { return ResourceType::TEXTURE; }
// 	else if (std::is_same_v<T, GameEngine::Matarial>) { return ResourceType::MATARIAL; }
// 	else if (std::is_same_v<T, GameEngine::Animation>) { return ResourceType::ANIMATION; }
	std::cout << "NONE" << std::endl;
	return ResourceType::NONE;
}

