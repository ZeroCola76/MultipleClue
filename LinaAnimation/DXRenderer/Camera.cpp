#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
}

Camera::~Camera()
{
}

// 좌우 움직임으로 이해하면 편함
Matrix Camera::GetViewRow()
{
	return Matrix::CreateTranslation(-m_localTranslation) * Matrix::CreateRotationY(-m_yaw) * Matrix::CreateRotationX(m_roll);
}

// 앞뒤(z축) 움직임으로 이해하면 편함
Matrix Camera::GetProjRow()
{
	return m_usePerspectiveProjection
		? XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), m_aspect, m_nearZ, m_farZ)
		: XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, m_nearZ, m_farZ);
}

// 로컬 위치를 반환
Vector3 Camera::GetEyePos()
{
	return m_localTranslation;
}

//void Camera::OnMouseMove(const int& screenHeight,const int& screenWidth,const int& mouseX,const int& mouseY)
void Camera::OnMouseMove(int screenHeight, int screenWidth, int mouseX, int mouseY)
{
	// 마우스 커서의 위치를 NDC로 변환
	// 마우스 커서는 좌측 상단 (0, 0), 우측 하단(width-1, height-1)
	// NDC는 좌측 하단이 (-1, -1), 우측 상단(1, 1)
	float x = mouseX * 1.0f / screenWidth - 1.0f;
	float y = -mouseY * 1.0f / screenHeight + 1.0f;

	// 커서가 화면 밖으로 나갔을 경우 범위 조절
	// 게임에서는 클램프를 안할 수도 있다.
	x = std::clamp(x, -1.0f, 1.0f); 
	y = std::clamp(y, -1.0f, 1.0f);

	// 카메라 시점 회전
	if (m_usePerspectiveProjection)
	{
		UpdateMouse(x, y);
	}
}

//마우스 포지션 업데이트 (x,y)
void Camera::UpdateMouse(float mouseNdcX, float mouseNdcY)
{
	// 얼마나 회전할지를 범위를 한정
	m_yaw = mouseNdcX * DirectX::XM_2PI;     // 좌우 360도
	m_roll = mouseNdcY * DirectX::XM_PIDIV2; // 위 아래 90도

	// 이동할 때 기준은 정면/ 오른쪽 방향 기준으로 계산하자.
	m_viewEyeDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationY(m_yaw));
	m_rightDir = m_viewUp.Cross(m_viewEyeDir);
}

//앞으로 이동
void Camera::MoveForward(float dt)
{
	m_localTranslation += m_viewEyeDir * m_speed * dt;
}

// 위로 이동
void Camera::MoveUp(float dt) 
{
	// 이동후의_위치 = 현재_위치 + 이동방향 * 속도 * 시간차이;
	m_localTranslation += m_viewUp * m_speed * dt;
}

//오른쪽으로 이동
void Camera::MoveRight(float dt)
{
	m_localTranslation += m_rightDir * m_speed * dt;
}

//화면 비율 세팅
void Camera::SetAspectRatio(float aspect)
{
	m_aspect = aspect;
}
