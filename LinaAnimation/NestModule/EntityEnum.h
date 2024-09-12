#pragma once

//Entitiy의 상태를 나타낸다.
enum class ENTITY_STATE
{
	NONE,		//아무것도 아닌 상태, 벽, 바닥, 배경등
	ALIVE,		//플레이어, 스킬 등 움직이는 것들.
	DESTROY,	//플레이어, 스킬이 끝남.등
};

//Entity의 타입를 나타낸다 .
enum class ENTITY_TYPE
{
	OBJECT,			//오브젝트 
	EFFECT,
	CAMERA,			//카메라 
	BACKGROUND,		//배경
};
