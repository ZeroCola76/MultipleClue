#pragma once
#include "../NestModule/Geometry.h"
#include <vector>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using namespace DirectX::SimpleMath;
using namespace std;

/// <summary>
/// ���� ������Ʈ�� ����� �Լ�
/// ���丮�� ���� ����
/// 2024.1.24. ���� ���� ��
/// </summary>
class GameObject {
public:
	static NestGraphics::MeshData MakeSquare(const float scale); // 2D�� �׸� �� ���ȴ�. ���߿� Grid�� ��ü ���ɼ� ����
	static NestGraphics::MeshData MakeBox(const float scale); // ���� �⺻ �ڽ��� �׸� �� ����Ѵ�. ���� ǥ���� �Ұ��ϴ�.
	static NestGraphics::MeshData MakeGrid(const float width, const float height, const int numSlices,
		const int numStacks, const float curve); // ���� ǥ���� ������ 2D �̹���(����, ����, dx, dy, �����)
	static NestGraphics::MeshData MakeCylinder(const float bottomRadius, const float topRadius, const float height,
		const int numSlices, const int numStacks); // �Ǹ����� �׸��� ������Ʈ (�Ʒ� ������, �� ������, ����, dx, dy) // �ؽ�ó�� dxdy�� �������� ����� �ϴµ� ������ �𸣰ڴ� ��
	static NestGraphics::MeshData MakeSphere(const float radius, const int numSlices, const int numStacks); // ���� �׸��� ������Ʈ(������, dx, dy)
	static vector<NestGraphics::MeshData> ReadFromFile(std::wstring filename);
};