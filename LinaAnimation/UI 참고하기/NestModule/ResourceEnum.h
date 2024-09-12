#pragma once
enum class ResourceType
{
	NONE,				//0 -> 아무것도 아닌것. 항상 디버그창에 오류가 뜨도록 설정하자.
	MESH,				//1
	SHADER,				//2
	TEXTURE,			//3
	MATARIAL,			//4
	ANIMATION,			//5

	END					//6, 마지막이어야 한다.
};