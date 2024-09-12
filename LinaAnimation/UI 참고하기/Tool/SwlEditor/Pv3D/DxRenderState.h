#pragma once
#include "PvDefine.h"

/// <summary>
/// DX11의 렌더 스테이트들을 생성하는 클래스
/// RasterizeState, BlendState, DepthStencilState
/// 생성된 렌더 스테이트는 리소스 매니저등으로 관리할것?
/// 우선은 ComPtr로 가지고 있고, Getter로 렌더러에서 가져가도록하자.
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

