#pragma once
#include <d3d11.h>
#include <dxgi.h>
#include <directxmath.h>
#include <DirectXPackedVector.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <DirectXColors.h>
#include <wrl.h>
#include <filesystem>
#include "directxtk\SimpleMath.h"

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft;
using namespace Microsoft::WRL;
using namespace SimpleMath;

/// <summary>
/// 아직 덜 완성된 Debug에서 디버그 정보를 출력하는 TRACE객체
/// </summary>
class DyTRACE
{
public:
	DyTRACE(const CHAR* strFile, DWORD dwLine, HRESULT _condition, const std::string& _outputString)
		: outputString(_outputString)
	{
		condition = static_cast<bool>(_condition);
#if defined(DEBUG)||defined(_DEBUG)
		if (condition != S_OK)
		{
			std::cout << "File:" << strFile
				<< " / Line:" << dwLine << " / " << outputString << "\n";
		}
#endif
	}
	DyTRACE(const CHAR* strFile, DWORD dwLine, bool _condition, const std::string& _outputString)
		:condition(_condition), outputString(_outputString)
	{
#if defined(DEBUG)||defined(_DEBUG)
		if (!condition)
		{
			std::cout << "File:" << strFile
				<< " / Line:" << dwLine << " / " << outputString << "\n";
		}
#endif
	}

private:
	bool condition;
	std::string outputString;
};

#define DYTRACE(x,y) 	{DyTRACE(__FILE__, (DWORD)__LINE__, x, y);}	