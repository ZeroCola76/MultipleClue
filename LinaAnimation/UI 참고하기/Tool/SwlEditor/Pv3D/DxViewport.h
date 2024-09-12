#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11�� Viewport
/// �������� N���� ����Ʈ�� ������ �ִ�.
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

