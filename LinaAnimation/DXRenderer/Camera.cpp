#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
}

Camera::~Camera()
{
}

// �¿� ���������� �����ϸ� ����
Matrix Camera::GetViewRow()
{
	return Matrix::CreateTranslation(-m_localTranslation) * Matrix::CreateRotationY(-m_yaw) * Matrix::CreateRotationX(m_roll);
}

// �յ�(z��) ���������� �����ϸ� ����
Matrix Camera::GetProjRow()
{
	return m_usePerspectiveProjection
		? XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), m_aspect, m_nearZ, m_farZ)
		: XMMatrixOrthographicOffCenterLH(-m_aspect, m_aspect, -1.0f, 1.0f, m_nearZ, m_farZ);
}

// ���� ��ġ�� ��ȯ
Vector3 Camera::GetEyePos()
{
	return m_localTranslation;
}

//void Camera::OnMouseMove(const int& screenHeight,const int& screenWidth,const int& mouseX,const int& mouseY)
void Camera::OnMouseMove(int screenHeight, int screenWidth, int mouseX, int mouseY)
{
	// ���콺 Ŀ���� ��ġ�� NDC�� ��ȯ
	// ���콺 Ŀ���� ���� ��� (0, 0), ���� �ϴ�(width-1, height-1)
	// NDC�� ���� �ϴ��� (-1, -1), ���� ���(1, 1)
	float x = mouseX * 1.0f / screenWidth - 1.0f;
	float y = -mouseY * 1.0f / screenHeight + 1.0f;

	// Ŀ���� ȭ�� ������ ������ ��� ���� ����
	// ���ӿ����� Ŭ������ ���� ���� �ִ�.
	x = std::clamp(x, -1.0f, 1.0f); 
	y = std::clamp(y, -1.0f, 1.0f);

	// ī�޶� ���� ȸ��
	if (m_usePerspectiveProjection)
	{
		UpdateMouse(x, y);
	}
}

//���콺 ������ ������Ʈ (x,y)
void Camera::UpdateMouse(float mouseNdcX, float mouseNdcY)
{
	// �󸶳� ȸ�������� ������ ����
	m_yaw = mouseNdcX * DirectX::XM_2PI;     // �¿� 360��
	m_roll = mouseNdcY * DirectX::XM_PIDIV2; // �� �Ʒ� 90��

	// �̵��� �� ������ ����/ ������ ���� �������� �������.
	m_viewEyeDir = Vector3::Transform(Vector3(0.0f, 0.0f, 1.0f), Matrix::CreateRotationY(m_yaw));
	m_rightDir = m_viewUp.Cross(m_viewEyeDir);
}

//������ �̵�
void Camera::MoveForward(float dt)
{
	m_localTranslation += m_viewEyeDir * m_speed * dt;
}

// ���� �̵�
void Camera::MoveUp(float dt) 
{
	// �̵�����_��ġ = ����_��ġ + �̵����� * �ӵ� * �ð�����;
	m_localTranslation += m_viewUp * m_speed * dt;
}

//���������� �̵�
void Camera::MoveRight(float dt)
{
	m_localTranslation += m_rightDir * m_speed * dt;
}

//ȭ�� ���� ����
void Camera::SetAspectRatio(float aspect)
{
	m_aspect = aspect;
}
