#pragma once
#include "IManager.h"
#include "JsonManager.h"
#include "EntityFactory.h"

/// <summary>
/// ���ӳ��� ���̴� ��� ������Ʈ���� �����Ѵ�.
/// </summary>
class ObjectManager : public IManager
{
public:
	ObjectManager();
	~ObjectManager();

public:
	//�Ŵ����� �ʱ�ȭ�� ��������� �� �� �ֵ��� �Ѵ�. 
	virtual void Initialize() override;

	//������ �ٲ� �� ���� ������Ʈ �Լ�
	virtual void Update() override;

	//�Ŵ����� ���� ���� ���������
	virtual void Finalize() override;

private:
	JsonManager* m_pJsonManager;		//Json���κ��� object������ �޾ƿ´�.
	EntityFactory* m_pEntityFactory;	//�޾ƿ� ������ �������� ��ü�� �����Ѵ�.
	
	std::vector<std::shared_ptr<Entity>> m_EntityVec;	//��ƼƼ���� �����ϴ� ����. sharedptr�� ���� �� ����..

public:
	void ReadJson();
	// ��ü�� �߰��Ѵ�.
	void AddObject(std::string _entityName, std::vector<std::string> comNameVec);
	
	// ��� ��ƼƼ���� ����� ������ �ּҰ��� ��ȯ�Ѵ�. (	���Ŵ����� �Ѱ��ֱ� ���ؼ�)
	std::vector<std::shared_ptr<Entity>>* GetEntityVecAddress();
};

