#pragma once
#include "MeshGroup.h"

/// <summary>
/// 2D �簢��. 
/// </summary>
class Quad : public MeshGroup
{
public:
	Quad(ComPtr<ID3D11Device>& device);
	~Quad();
	//������ �д� �׷�..�׷� ������ �ʿ��ѵ� ���ҽ��Ŵ����� ���߾˰Ͱ���..
	//void GetTexturePath();
};

