#pragma once
#include <string>
#include <vector>
#include <memory>
#include "directxtk/SimpleMath.h"

class IComponent;

using DirectX::SimpleMath::Vector3;

class Entity
{
public:
	Entity();
	~Entity();
private:
	std::string m_Name;							//��ƼƼ�� �̸�.
	std::unordered_map<std::string, std::shared_ptr<IComponent>> m_pComponents;		//������Ʈ���� ������ �ִ� ����

public:
	//����� �ʱ�ȭ.
	void Initialize(std::string _name);

	//��������� �����ϱ� ����
	void Finalize();

	//Entity�� ���� Component�� �����Ѵ�.
	void AddComponent(std::string ComName, std::shared_ptr<IComponent> _Component);
};

