#pragma once
#include <vector>
#include "IManager.h"
#include "InputEnum.h" 
#include "directxtk/SimpleMath.h"

/// <summary>
/// Ű���� ���콺�� input�� �����ϴ� �Ŵ���
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
	//enum class�� �����ϰ� ��� �־�� �Ѵ�.
	std::vector<int> m_key;

	std::vector<KeyInfo> m_keyInfo;
	
	HWND m_hwnd;

	//���콺 ������
	POINT m_currentMousePos;

	//���� �������� ���콺 ������
	POINT m_prevMousePos;

	//���콺�� ������ ����
	DirectX::SimpleMath::Vector2 deltaMousePos;

private:
	//â�� ��Ŀ�� ���θ� �Ǻ��Ͽ� ��Ŀ�� ���°� �ƴ϶�� ��� �Է��� ���� �ʴ´�.
	void InFocus();

	//â�� �ƿ���Ŀ�� �Ǿ��� ��
	void OutFocus();

	//���콺�� ��ġ
	void MousePos();

	//���콺�� ������ ����, ���߿� �� ���� ����� �ʿ��� ���� �ִ�.
	DirectX::SimpleMath::Vector2 GetDeltaMousePos();

public:
	KEY_STATE GetKeyState(KEY _Key);
	POINT GetMousePos();
};

