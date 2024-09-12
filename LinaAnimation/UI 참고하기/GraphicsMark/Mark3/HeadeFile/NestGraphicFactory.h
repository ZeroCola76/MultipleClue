#pragma once
#include <vector>
/// <summary>
/// ���� ���⼭ �׸��� ������Ʈ�� �����Ŵ�.
/// �ڽ��� ���̳� ���尰����.
/// </summary>
class MeshGroup;

class NestGraphicFactory
{
public:
	NestGraphicFactory();
	~NestGraphicFactory();

	void MakeDXObject(std::vector<MeshGroup*>& DXObjects, MeshGroup* pObject);
	void Initialize();
};

