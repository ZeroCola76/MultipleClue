#pragma once
#include <directxtk/SimpleMath.h>

using namespace DirectX;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

/// <summary>
/// ī�޶� ����ϴ� Ŭ����
/// 2024.1.24. ���� ���� ��
/// </summary>
class Camera
{
public:
	Camera();
	~Camera();
private:
	int m_clientHeight;
	int m_clientWidth;
	int m_mouseX;
	int m_mouseY;
	HWND m_hwnd;
	bool m_usePerspectiveProjection = true;				// ���� ���� ��� �ʱⰪ : �����
	Vector3 m_localTranslation = Vector3(0.0f);			//��ü �̵�
	Vector3 m_localRotation = Vector3(0.0f);			// ��ü ȸ��
	Vector3 m_localScaling = Vector3(0.5f);				// ��ü ũ�� : Grid�� ������ �� ���õ�
	Vector3 m_viewEyePos = { 0.0f, 0.15f, 0.0f };		// ���� ���� ��ġ
	Vector3 m_viewEyeDir = { 0.0f, 0.0f, 1.0f };		// ���� ���� �� -> ��ü�� ��ġ ������ �����̴� ��ó�� ������
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };			// 
	Vector3 m_rightDir = Vector3(1.0f, 0.0f, 0.0f);
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;								// m_nearZ : ���� ����� ���
	float m_farZ = 100.0f;								//m_farZ ���� �� ���
	float m_aspect = 16.0f / 9.0f;						// ���� / ���� ȭ�� ����
	float m_roll = 0.0f;								// ���콺 �¿� 360��
	float m_yaw = 0.0f;									// �� �Ʒ� 90��
	float m_speed = 1.0f;								// �����̴� �ӵ�

public:
	//void OnMouseMove(const int& screenHeight, const int& screenWidth, const int& mouseX, const int& mouseY);
	void OnMouseMove(int screenHeight, int screenWidth, int mouseX, int mouseY);
	void UpdateMouse(float mouseNdcX, float mouseNdcY); // ���콺 ������ ��ġ ����
	void MoveForward(float dt); // �յڷ� ������
	void MoveUp(float dt); // ������ ����
	void MoveRight(float dt); // �¿�� ������

	// ī�޶� ����� view x Projection x EyePosition���� ������ (3�� ��� �ʿ�)
	Matrix GetViewRow();
	Matrix GetProjRow();
	Vector3 GetEyePos();

	void SetAspectRatio(float aspect); // ȭ�� ���� �� �޾ƿ�
	Vector3 setLocalPosition(const Vector3& localTranslation) { m_localTranslation = localTranslation; return m_localTranslation; }
	Vector3 getLocalPosition() const { return m_localTranslation;}
	float getProjFovAngleY() const { return m_projFovAngleY; }
	float getNearZ() const { return m_nearZ; }
	float getFarZ() const { return m_farZ; }
};
