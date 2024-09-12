#include "TRSComponent.h"

TRSComponent::TRSComponent()
{
	SetComponentName("TRSComponent");	//������Ʈ�� �̸� ����
}

TRSComponent::~TRSComponent()
{

}

// ����� ���� �ٲ��� �� ��
Matrix TRSComponent::MakeTRSMatrix(Vector3 m_Position, Vector3 m_Rotation, Vector3 m_Scale)
{
	Matrix TRSmatrix = Matrix::CreateScale(m_Scale) *
		Matrix::CreateRotationZ(m_Rotation.z) *
		Matrix::CreateRotationY(m_Rotation.y) *
		Matrix::CreateRotationX(m_Rotation.x) *
		Matrix::CreateTranslation(m_Position);

	return TRSmatrix;
}

