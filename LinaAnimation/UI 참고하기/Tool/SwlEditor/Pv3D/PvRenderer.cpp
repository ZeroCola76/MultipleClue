#include "PvRenderer.h"
#include "DxDevice.h"
#include "DxRenderState.h"
#include "DxRenderTarget.h"
#include "DxViewport.h"
#include "Camera.h"

#include <Effects.h>

PvRenderer::PvRenderer()
{
	m_pCamera = std::make_shared<Camera>();
}

PvRenderer::~PvRenderer()
{

}

bool PvRenderer::Initialize(int m_HWnd, int m_ScreenWidth, int m_ScreenHeight)
{
	m_pDevice = new DxDevice();
	m_pDevice->CreateDevice(m_HWnd, m_ScreenWidth, m_ScreenHeight);

	m_pRenderTarget = new DxRenderTarget(m_pDevice->GetDevice(), m_pDevice->GetDeviceContext()
		, m_pDevice->GetSwapChain(), m_ScreenWidth, m_ScreenHeight);

	m_pRenderTarget->SetRenderTarget();
	m_pRenderTarget->SetDepthStencilView();

	m_pViewport = new DxViewport(m_pDevice->GetDeviceContext(), Vector4(0.f, 0.f, m_ScreenWidth, m_ScreenHeight));

	m_pRenderState = new DxRenderState();
	m_pRenderState->CreateRenderStates(m_pDevice->GetDevice());

	m_pCamera->LookAt(Vector3(8.f, 8.f, -8.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f));

	primitive = DX11::GeometricPrimitive::CreateCube(m_pDevice->GetDeviceContext());

	return true;
}

bool PvRenderer::Update(float m_dTime)
{

	return true;
}

void PvRenderer::BeginRender()
{
	m_pViewport->SetViewport();

	m_pRenderTarget->ClearRenderTargetView(reinterpret_cast<const float*>(&Colors::Gray));
	m_pRenderTarget->ClearDepthStencilView();
}

void PvRenderer::DrawSomething()
{
	primitive->DirectX::DX11::GeometricPrimitive::Draw(XMMatrixIdentity(), m_pCamera->View(), m_pCamera->Proj());
}

void PvRenderer::EndRender()
{
	m_pDevice->Present();
}

void PvRenderer::Finalize()
{

}
