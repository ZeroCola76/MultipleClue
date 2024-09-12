#pragma once

class IRenderer abstract
{
public:
	IRenderer() {}
	virtual ~IRenderer() {}

	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) abstract;
	virtual bool Update(float m_dTime) abstract;
	virtual void BeginRender() abstract;
	virtual void DrawSomething() abstract;
	virtual void EndRender() abstract;
	virtual void Finalize() abstract;

};
