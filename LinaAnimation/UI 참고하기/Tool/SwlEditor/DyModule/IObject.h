#pragma once

class IObject abstract
{
	IObject() {}
	virtual ~IObject() {}

	/// <summary>
	/// ������Ʈ �ʱ�ȭ
	/// </summary>
	virtual void Initilaize() abstract;

	/// <summary>
	/// ������ ��� ������Ʈ ������Ʈ
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// ������ �ӵ��� ������� �����ϰ� ȣ��Ǵ� �Լ�
	/// ���� ��� �� ������Ʈ � �ַ� ���ȴ�.
	/// </summary>
	virtual void FixedUpdate() abstract;

	/// <summary>
	/// ������Ʈ�� ������ ���� �� �����Ӵ� �� �� ȣ��Ǵ� �Լ�
	/// �÷��̾��� �������� Update���� �Ϸ��ϰ� �̵��� ��ġ�� ����
	/// ī�޶��� ��ġ�� LateUpdate���� �̵��ϴ� ������ ������ �� ���ȴ�. 
	/// </summary>
	virtual void LateUpdate() abstract;

};

