#pragma once

//Entitiy�� ���¸� ��Ÿ����.
enum class ENTITY_STATE
{
	NONE,		//�ƹ��͵� �ƴ� ����, ��, �ٴ�, ����
	ALIVE,		//�÷��̾�, ��ų �� �����̴� �͵�.
	DESTROY,	//�÷��̾�, ��ų�� ����.��
};

//Entity�� Ÿ�Ը� ��Ÿ���� .
enum class ENTITY_TYPE
{
	OBJECT,			//������Ʈ 
	EFFECT,
	CAMERA,			//ī�޶� 
	BACKGROUND,		//���
};
