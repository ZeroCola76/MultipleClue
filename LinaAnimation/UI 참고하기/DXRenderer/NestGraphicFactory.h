#pragma once
#include <vector>
/// <summary>
/// 이제 여기서 그리는 오브젝트를 뱉을거다.
/// 박스나 원이나 쿼드같은거.
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

