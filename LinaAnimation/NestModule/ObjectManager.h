#pragma once
#include "IManager.h"
#include "JsonManager.h"
#include "EntityFactory.h"

/// <summary>
/// 게임내에 쓰이는 모든 오브젝트들을 관리한다.
/// </summary>
class ObjectManager : public IManager
{
public:
	ObjectManager();
	~ObjectManager();

public:
	//매니저의 초기화를 명시적으로 할 수 있도록 한다. 
	virtual void Initialize() override;

	//내용이 바꿀 때 쓰일 업데이트 함수
	virtual void Update() override;

	//매니저의 해제 역시 명시적으로
	virtual void Finalize() override;

private:
	JsonManager* m_pJsonManager;		//Json으로부터 object정보를 받아온다.
	EntityFactory* m_pEntityFactory;	//받아온 정보를 바탕으로 객체를 생성한다.
	
	std::vector<std::shared_ptr<Entity>> m_EntityVec;	//엔티티들을 보관하는 벡터. sharedptr로 관리 할 생각..

public:
	void ReadJson();
	// 객체를 추가한다.
	void AddObject(std::string _entityName, std::vector<std::string> comNameVec);
	
	// 모든 엔티티들이 저장된 벡터의 주소값을 반환한다. (	씬매니저에 넘겨주기 위해서)
	std::vector<std::shared_ptr<Entity>>* GetEntityVecAddress();
};

