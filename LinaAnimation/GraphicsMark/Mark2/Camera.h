#pragma once
#include <directxtk/SimpleMath.h>

using namespace DirectX;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

/// <summary>
/// 카메라를 사용하는 클래스
/// 2024.1.24. 순서 정리 끝
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
	bool m_usePerspectiveProjection = true;				// 원근 투영 사용 초기값 : 사용함
	Vector3 m_localTranslation = Vector3(0.0f);			//물체 이동
	Vector3 m_localRotation = Vector3(0.0f);			// 물체 회전
	Vector3 m_localScaling = Vector3(0.5f);				// 물체 크기 : Grid는 스케일 값 무시됨
	Vector3 m_viewEyePos = { 0.0f, 0.15f, 0.0f };		// 현재 눈의 위치
	Vector3 m_viewEyeDir = { 0.0f, 0.0f, 1.0f };		// 고개를 돌릴 때 -> 물체가 마치 구면을 움직이는 것처럼 움직임
	Vector3 m_viewUp = { 0.0f, 1.0f, 0.0f };			// 
	Vector3 m_rightDir = Vector3(1.0f, 0.0f, 0.0f);
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;								// m_nearZ : 가장 가까운 평면
	float m_farZ = 100.0f;								//m_farZ 가장 먼 평면
	float m_aspect = 16.0f / 9.0f;						// 가로 / 세로 화면 비율
	float m_roll = 0.0f;								// 마우스 좌우 360도
	float m_yaw = 0.0f;									// 위 아래 90도
	float m_speed = 1.0f;								// 움직이는 속도

public:
	//void OnMouseMove(const int& screenHeight, const int& screenWidth, const int& mouseX, const int& mouseY);
	void OnMouseMove(int screenHeight, int screenWidth, int mouseX, int mouseY);
	void UpdateMouse(float mouseNdcX, float mouseNdcY); // 마우스 움직임 위치 업뎃
	void MoveForward(float dt); // 앞뒤로 움직임
	void MoveUp(float dt); // 높낮이 조정
	void MoveRight(float dt); // 좌우로 움직임

	// 카메라 행렬은 view x Projection x EyePosition으로 구현함 (3개 행렬 필요)
	Matrix GetViewRow();
	Matrix GetProjRow();
	Vector3 GetEyePos();

	void SetAspectRatio(float aspect); // 화면 비율 값 받아옴
	Vector3 setLocalPosition(const Vector3& localTranslation) { m_localTranslation = localTranslation; return m_localTranslation; }
	Vector3 getLocalPosition() const { return m_localTranslation;}
	float getProjFovAngleY() const { return m_projFovAngleY; }
	float getNearZ() const { return m_nearZ; }
	float getFarZ() const { return m_farZ; }
};
