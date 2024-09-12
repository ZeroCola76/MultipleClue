#pragma once
#include <vector>
#include "IManager.h"
#include "InputEnum.h" 
#include "directxtk/SimpleMath.h"

/// <summary>
/// 키보드 마우스의 input을 관리하는 매니저
/// </summary>
class InputManager : public IManager
{
public:
	struct KeyInfo
	{
		KEY_STATE state;
		bool isPushed;
	};
public:
	InputManager(HWND hwnd);
	~InputManager();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Finalize() override;

private:
	//enum class와 동일하게 들고 있어야 한다.
	std::vector<int> m_key;

	std::vector<KeyInfo> m_keyInfo;
	
	HWND m_hwnd;

	//마우스 포지션
	POINT m_currentMousePos;

	//이전 프레임의 마우스 포지션
	POINT m_prevMousePos;

	//마우스의 움직임 정도
	DirectX::SimpleMath::Vector2 deltaMousePos;

private:
	//창의 포커싱 여부를 판별하여 포커싱 상태가 아니라면 모든 입력을 받지 않는다.
	void InFocus();

	//창이 아웃포커싱 되었을 때
	void OutFocus();

	//마우스의 위치
	void MousePos();

	//마우스가 움직인 정도, 나중에 그 길이 계산이 필요할 수도 있다.
	DirectX::SimpleMath::Vector2 GetDeltaMousePos();

public:
	KEY_STATE GetKeyState(KEY _Key);
	POINT GetMousePos();
};

