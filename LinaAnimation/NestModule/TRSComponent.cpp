#include "TRSComponent.h"

TRSComponent::TRSComponent()
{
	SetComponentName("TRSComponent");	//컴포넌트의 이름 세팅
}

TRSComponent::~TRSComponent()
{

}

// 절대로 순서 바꾸지 말 것
Matrix TRSComponent::MakeTRSMatrix(Vector3 m_Position, Vector3 m_Rotation, Vector3 m_Scale)
{
	Matrix TRSmatrix = Matrix::CreateScale(m_Scale) *
		Matrix::CreateRotationZ(m_Rotation.z) *
		Matrix::CreateRotationY(m_Rotation.y) *
		Matrix::CreateRotationX(m_Rotation.x) *
		Matrix::CreateTranslation(m_Position);

	return TRSmatrix;
}

