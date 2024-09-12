#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace NestGraphics
{
	struct Mesh {
		ComPtr<ID3D11Buffer> m_vertexBuffer;
		ComPtr<ID3D11Buffer> m_indexBuffer;
		ComPtr<ID3D11Buffer> m_vertexConstantBuffer;
		ComPtr<ID3D11Buffer> m_pixelConstantBuffer;

		ComPtr<ID3D11Texture2D> texture;
		ComPtr<ID3D11ShaderResourceView> textureResourceView;

		UINT m_indexCount;
	};
}

