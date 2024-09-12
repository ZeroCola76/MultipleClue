#pragma once
#define MAX_LIGHTS 3

#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <d3dcommon.h>
#include <d3d11.h>
#include <dxgi.h>   
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include <Windows.h>
#include <wrl.h> 

#include "Geometry.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
                                                             LPARAM lParam);

using Microsoft::WRL::ComPtr;
using std::vector;
using std::wstring;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

namespace Nest
{
	// Α¶Έν
	//struct Light {
	//	Vector3 strength = Vector3(1.0f);              // 12
	//	float fallOffStart = 0.0f;                     // 4
	//	Vector3 direction = Vector3(0.0f, 0.0f, 1.0f); // 12
	//	float fallOffEnd = 10.0f;                      // 4
	//	Vector3 position = Vector3(0.0f, 0.0f, -2.0f); // 12
	//	float spotPower = 1.0f;                        // 4
	//};

	//struct VertexConstantBuffer {
	//	Matrix world;
	//	Matrix invTranspose;
	//	Matrix view;
	//	Matrix projection;
	//};

	//struct PixelShaderConstantBuffer {
	//	Vector3 eyeWorld;         // 12
	//	bool useTexture = 1;      // 4
	//	NestGraphics::Material material;        // 48
	//	Light lights[MAX_LIGHTS]; // 48 * MAX_LIGHTS
	//};

	//struct NormalShaderConstantBuffer {
	//	/*Matrix world;
	//	Matrix invTranspose;
	//	Matrix view;
	//	Matrix projection;*/
	//	float scale = 0.1f;
	//	float dummy[3];
	//};
}
//
//static_assert((sizeof(NestGraphics::VertexConstantBuffer) % 16) == 0,
//              "Constant Buffer size must be 16-byte aligned");
//
//
//
//static_assert((sizeof(NestGraphics::PixelShaderConstantBuffer) % 16) == 0,
//              "Constant Buffer size must be 16-byte aligned");

