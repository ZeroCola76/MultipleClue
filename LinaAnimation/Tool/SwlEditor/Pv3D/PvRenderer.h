#pragma once
#include "IRenderer.h"
#include "PvDefine.h"
#include "directxtk\GeometricPrimitive.h"

class DxDevice;
class DxRenderTarget;
class DxRenderState;
class DxShader;
class DxViewport;
class Camera;

class PvRenderer : public IRenderer
{
public:
	PvRenderer();
	~PvRenderer();

	virtual bool Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight) override;
	virtual bool Update(float m_dTime) override;
	virtual void BeginRender() override;
	virtual void DrawSomething() override;
	virtual void EndRender() override;
	virtual void Finalize() override;

private:
	DxDevice* m_pDevice;
	DxRenderTarget* m_pRenderTarget;
	DxViewport* m_pViewport;
	DxRenderState* m_pRenderState;
	DxShader* m_pShader;

	ID3D11RenderTargetView* pRenderTargetView;
	D3D11_TEXTURE2D_DESC renderTargetTexture;

	std::shared_ptr<Camera> m_pCamera;

	std::shared_ptr< DirectX::DX11::GeometricPrimitive> primitive;

	int index=0;

};

