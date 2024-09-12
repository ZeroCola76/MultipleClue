#include "NestGraphicFactory.h"
#include "MeshGroup.h"

NestGraphicFactory::NestGraphicFactory()
{

}

NestGraphicFactory::~NestGraphicFactory()
{
}

void NestGraphicFactory::MakeDXObject(std::vector<MeshGroup*>& DXObjects, MeshGroup* pObject)
{
	DXObjects.push_back(pObject);
}

void NestGraphicFactory::Initialize()
{

}
