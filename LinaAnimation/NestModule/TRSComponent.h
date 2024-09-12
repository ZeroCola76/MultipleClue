#pragma once
#include "IComponent.h"
#include "directxtk/SimpleMath.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

/// <summary>
/// ���� ��� ��ƼƼ�� ������ ���� TRS Component
/// </summary>
class TRSComponent : public IComponent
{
public:
	TRSComponent(); 
	~TRSComponent();

public:
	Vector3 m_Position;
	//Vector3 m_Transform;	//�Ŀ� quaternion �߰� �� Ȱ��ȭ�� ��Ų��. 
	Vector3 m_Rotation;
	Vector3 m_Scale;

public:
	// �׳� �ӽ÷� ���� �Լ��Դϴ�.
	// T,R,S ���� �Է��ϸ� TRS ��Ʈ������ �ٲ��ִ� �Լ�
	Matrix MakeTRSMatrix(Vector3 m_Position, Vector3 m_Rotation, Vector3 m_Scale);
};

