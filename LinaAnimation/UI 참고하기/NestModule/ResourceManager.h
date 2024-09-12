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
/// 게임에 쓰일 리소스들을 관리하는 매니져
/// 나중에 imanager로부터 상속받은 것들을 손대자.
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
	//KeyObjMap이라는 별칭을 만들어준다. <키 값, shared_ptr>로 구성된다.
	using KeyObjMap = std::unordered_map<std::wstring, std::shared_ptr<ResourceBase>>;

	//배열을 만든다. 
	std::array<KeyObjMap, static_cast<int>(ResourceType::END)> m_Resources;


public:
	// 리소스 등록 요청을 하는 함수, 씬마다 쓸 리소스가 다르니 씬 종료시 해제 되도록 shared_ptr을 쓴다.
	template<typename T>
	bool Add(const std::wstring& key, std::shared_ptr<T> object);

	// 키값을 통해 그에 해당하는 리소스를 찾는 함수
	template<typename T>
	std::shared_ptr<T> Get(const std::wstring& key);

	// 타입을 통해 리소스 찾는 것을 유도하는 함수
	template<typename T>
	ResourceType GetResourceType();

};

///template은 cpp에 정의하면 안된다.---------------------------------------------------------------
// 리소스를 추가하는 함수 <Key값, shared_ptr>
template<typename T>
bool  ResourceManager::Add(const std::wstring& key, std::shared_ptr<T> object)
{
	//enum class로부터 타입을 받아온다.
	ResourceType resourceType = GetResourceType<T>();

	//resourceType에 해당하는 KeyObjMap을 m_Resources 배열에서 가져온다.(enumClass순서대로)
	KeyObjMap& keyObjMap = m_Resources[static_cast<int>(resourceType)];

	//현재 키(key)가 keyObjMap에 이미 존재하는지 검사-------------------
	auto findIter = keyObjMap.find(key);

	// 해당 키로 등록된 리소스 타입이 이미 있는지 판단 -> 있으면 실패
	if (findIter != keyObjMap.end()) { return false; }

	//없다면 keyObjMap에 새로운 리소스를 추가
	keyObjMap[key] = object;

	return true;
}

//키값을 통해 그에 해당하는 리소스를 찾아 리턴
template<typename T>
std::shared_ptr<T> ResourceManager::Get(const std::wstring& key)
{
	//enum class로부터 타입을 받아온다.
	ResourceType resourceType = GetResourceType<T>();

	// 리소스 타입을 정수로 변환해서 찾기
	KeyObjMap& keyObjMap = m_Resources[static_cast<int>(resourceType)];

	//현재 키(key)가 keyObjMap에 이미 존재하는지 검사-------------------
	auto findIter = keyObjMap.find(key);

	//findIt이 end()가 아닌 경우 -> 리턴
	if (findIter != keyObjMap.end())
	{
		return std::static_pointer_cast<T>(findIter->second);
	}

	//없을 경우 null반환
	return nullptr;
}

template<typename T>
ResourceType ResourceManager::GetResourceType()
{
	// 리소스 타입이 일치하는가
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

