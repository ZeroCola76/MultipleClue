#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11�� ���� ������Ʈ���� �����ϴ� Ŭ����
/// RasterizeState, BlendState, DepthStencilState
/// ������ ���� ������Ʈ�� ���ҽ� �Ŵ��������� �����Ұ�?
/// �켱�� ComPtr�� ������ �ְ�, Getter�� ���������� ��������������.
/// </summary>
class DxRenderState
{
public:
	DxRenderState();
	~DxRenderState();

	void CreateRenderStates(ID3D11Device* pDevice);
	ID3D11RasterizerState* GetWireframeRS() { return m_pWireframeRasterizerState.Get(); }
	ID3D11RasterizerState* GetSolidRS() { return m_pSolidRasterizerState.Get(); }

private:
	ComPtr<ID3D11RasterizerState> m_pWireframeRasterizerState;
	ComPtr<ID3D11RasterizerState> m_pSolidRasterizerState;

};

