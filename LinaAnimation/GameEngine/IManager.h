#pragma once
class IManager
{
public:
	IManager();
	virtual ~IManager();

	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Finalize() abstract;
};