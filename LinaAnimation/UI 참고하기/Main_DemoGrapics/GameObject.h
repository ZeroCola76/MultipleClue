#pragma once
#include "../NestModule/Geometry.h"
#include <vector>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using namespace DirectX::SimpleMath;
using namespace std;

/// <summary>
/// 게임 오브젝트를 만드는 함수
/// 팩토리로 변경 예정
/// 2024.1.24. 순서 정리 끝
/// </summary>
class GameObject {
public:
	static NestGraphics::MeshData MakeSquare(const float scale); // 2D를 그릴 때 사용된다. 나중에 Grid로 대체 가능성 있음
	static NestGraphics::MeshData MakeBox(const float scale); // 가장 기본 박스를 그릴 때 사용한다. 굴곡 표현은 불가하다.
	static NestGraphics::MeshData MakeGrid(const float width, const float height, const int numSlices,
		const int numStacks, const float curve); // 굴곡 표현이 가능한 2D 이미지(가로, 세로, dx, dy, 굴곡률)
	static NestGraphics::MeshData MakeCylinder(const float bottomRadius, const float topRadius, const float height,
		const int numSlices, const int numStacks); // 실린더를 그리는 오브젝트 (아래 반지름, 위 반지름, 높이, dx, dy) // 텍스처와 dxdy를 수동으로 맞춰야 하는데 이유를 모르겠다 ㅠ
	static NestGraphics::MeshData MakeSphere(const float radius, const int numSlices, const int numStacks); // 구를 그리는 오브젝트(반지름, dx, dy)
	static vector<NestGraphics::MeshData> ReadFromFile(std::wstring filename);
};