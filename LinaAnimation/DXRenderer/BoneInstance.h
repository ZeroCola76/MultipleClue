#pragma once
#include "Geometry.h"

class BoneInstance
{
public:

	Matrix m_localTrm;
	Matrix m_worldTrm;
	int BoneIndex = -1;			 // ∫ª ¿Œµ¶Ω∫
	std::string m_boneName;
	Matrix offsetTrm;

	BoneInstance* m_pBoneParent;
	//std::vector<BoneInstance> m_BoneChild;

	std::shared_ptr<NestGraphics::asKeyframeContainer> m_KeyframeContainer;
};

