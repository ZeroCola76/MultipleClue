#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11의 Viewport
/// 렌더러는 N개의 뷰포트를 가질수 있다.
/// </summary>
class DxViewport
{
public:
	DxViewport(ID3D11DeviceContext* pDeviceContext, Vector4 leftTopWidthHeight);
	~DxViewport();

	void SetViewport();

private:
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	D3D11_VIEWPORT m_viewport;
};

