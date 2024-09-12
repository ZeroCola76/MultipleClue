#include "framework.h"
#include "Process.h"
#include "PvRenderer.h"
#include "DyTimer.h"
#include "JsonUtil.h"

bool Process::Initialize(HINSTANCE hInstance)
{
#ifdef _DEBUG
	CreateConsole();
#endif // _DEBUG

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = Process::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PVDEMO));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = L"PvDemo";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowW(L"PvDemo", L"PvDemo", WS_OVERLAPPEDWINDOW,
		m_screenPosX, m_screenPosY, m_screenWidth, m_screenHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		std::cout << "CreateWindow Error.\n";
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	// 렌더러 및 기타 모듈 생성 및 초기화
	m_p3DRenderer = new PvRenderer();
	if (!m_p3DRenderer->Initialize(reinterpret_cast<int>(hWnd), m_screenWidth, m_screenHeight))
	{
		std::cout << "Renderer Initializing Error.\n";
	}

	m_pTimer = DyTimer::GetInstance();
	m_pTimer->Reset();

	TestCode();

	return TRUE;
}

void Process::Update()
{
	m_pTimer->Tick();

	static int frameCount = 0;
	static float elapsedTime = 0.f;
	frameCount++;
	if ((m_pTimer->TotalTime() - elapsedTime) >= 1.f)
	{
		float fps = (float)frameCount;
		float  ms = 1000.f / fps;

		frameCount = 0;
		elapsedTime += 1.f;
		//std::cout << "FPS : " << fps<<"\t";

		POINT mousePoint;
		if (GetCursorPos(&mousePoint))
		{
			//std::cout << "mouse position : " << mousePoint.x << ", " << mousePoint.y << std::endl;
		}
	}

	m_p3DRenderer->Update(m_pTimer->DeltaTime());
}

void Process::Render()
{
	m_p3DRenderer->BeginRender();
	m_p3DRenderer->DrawSomething();
	m_p3DRenderer->EndRender();
}

void Process::CreateConsole()
{
	// 콘솔 창 할당
	AllocConsole();

	// 표준 입출력 리디렉션
	FILE* dummy;
	_wfreopen_s(&dummy, L"CONIN$", L"r", stdin);

	// 표준 출력 리디렉션
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stdout);

	// 표준 에러 리디렉션
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stderr);
}

void Process::TestCode()
{
	m_pJsonUtil = new JsonUtil();

	Json::Value data;

	data["obj1"]["type"] = "cube";
	data["obj1"]["pos"]["x"] = 1.0;
	data["obj1"]["pos"]["y"] = 1.0;
	data["obj1"]["pos"]["z"] = 1.0;

	m_pJsonUtil->SetData(data);

	m_pJsonUtil->SaveData(L"Test2.json");
}

void Process::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
				break;

			DispatchMessage(&m_msg);
		}
		else
		{
			Update();
			Render();
		}
	}
}

void Process::Finalize()
{
#ifdef _DEBUG
	FreeConsole();
#endif // _DEBUG

}

LRESULT CALLBACK Process::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

