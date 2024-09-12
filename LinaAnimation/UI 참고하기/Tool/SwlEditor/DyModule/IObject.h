#pragma once

class IObject abstract
{
	IObject() {}
	virtual ~IObject() {}

	/// <summary>
	/// 오브젝트 초기화
	/// </summary>
	virtual void Initilaize() abstract;

	/// <summary>
	/// 프레임 기반 오브젝트 업데이트
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 프레임 속도에 관계없이 일정하게 호출되는 함수
	/// 물리 계산 및 업데이트 등에 주로 사용된다.
	/// </summary>
	virtual void FixedUpdate() abstract;

	/// <summary>
	/// 업데이트가 완전히 끝난 후 프레임당 한 번 호출되는 함수
	/// 플레이어의 움직임을 Update에서 완료하고 이동한 위치에 따라
	/// 카메라의 위치를 LateUpdate에서 이동하는 식으로 구현할 때 사용된다. 
	/// </summary>
	virtual void LateUpdate() abstract;

};

