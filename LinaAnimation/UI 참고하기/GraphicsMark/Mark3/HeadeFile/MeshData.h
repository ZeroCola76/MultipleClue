#pragma once
#include <string>
#include "Vertex.h"

using std::vector;

namespace NestGraphics
{
	// �޽� ������ : �⺻������ ���ؽ��� ��ġ�� �ε����� �����ϰ� �ִ�.
	struct MeshData 
	{
	std::vector<NestGraphics::Vertex> vertices;
	std::vector<uint16_t> indices;
	std::string materialName;		// mesh�� material�� �̸�
	std::string textureFilename; // �ؽ�ó�� ������ �������� �ʱ� ���� �÷���
	};
}


