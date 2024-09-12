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
	std::string m_Name;							//엔티티의 이름.
	std::unordered_map<std::string, std::shared_ptr<IComponent>> m_pComponents;		//컴포넌트들을 가지고 있는 벡터

public:
	//명시적 초기화.
	void Initialize(std::string _name);

	//명시적으로 해제하기 위해
	void Finalize();

	//Entity가 갖는 Component를 저장한다.
	void AddComponent(std::string ComName, std::shared_ptr<IComponent> _Component);
};

