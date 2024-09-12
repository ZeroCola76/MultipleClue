#pragma once
#include "PvDefine.h"


class DxShader
{
public:
	DxShader(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~DxShader();

	void Update();
	void SetShader();

	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector4 Color;
		Vector2 TexCoord;
	};

	struct ConstantBuffer
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

private:
	void CreateVertexShader();
	void CreatePixelShader();
	void CreateComstantBuffer();

private:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	
	ComPtr<ID3D11VertexShader> m_pVertexShader;
	ComPtr<ID3D11PixelShader> m_pPixelShader;
	ComPtr<ID3D11Buffer> m_pConstBuffer;

};

