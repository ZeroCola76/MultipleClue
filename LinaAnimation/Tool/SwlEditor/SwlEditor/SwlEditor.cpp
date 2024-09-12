
// SwlEditor.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SwlEditor.h"
#include "MainFrm.h"

#include "SwlEditorDoc.h"
#include "SwlEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSwlEditorApp

BEGIN_MESSAGE_MAP(CSwlEditorApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSwlEditorApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
// 	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
// 	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CSwlEditorApp 생성

CSwlEditorApp::CSwlEditorApp() noexcept
{
	m_bHiColorIcons = TRUE;


	// TODO: 아래 애플리케이션 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SwlEditor.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CSwlEditorApp 개체입니다.

CSwlEditorApp theApp;


// CSwlEditorApp 초기화

BOOL CSwlEditorApp::InitInstance()
{
	// 애플리케이션 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	m_pTimer = DyTimer::GetInstance();
	m_pTimer->Reset();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 애플리케이션의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSwlEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CSwlEditorView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

// CSwlEditorApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//	ON_WM_CREATE()
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CSwlEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSwlEditorApp 사용자 지정 로드/저장 방법

void CSwlEditorApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CSwlEditorApp::LoadCustomState()
{
}

void CSwlEditorApp::SaveCustomState()
{
}

// CSwlEditorApp 메시지 처리기





int CSwlEditorApp::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// timer의 업데이트는 여기서
	// 하는게 아니라 메인 프레임에서
	// 하는것도 아니라 OnIdle함수에서
// 	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
// 
// 	m_pTimer->Tick();
// 
// 	static int frameCount = 0;
// 	static float elapsedTime = 0.f;
// 	frameCount++;
// 
// 	// 프레임당 경과 시간을 얻기 위해 DeltaTime 사용
// 	elapsedTime += m_pTimer->DeltaTime();
// 
// 	if (elapsedTime >= 1.f)
// 	{
// 		// FPS와 MS 계산
// 		m_FPS = static_cast<int>(frameCount / elapsedTime);
// 		m_ms = 1000.f / m_FPS;
// 
// 		frameCount = 0;
// 		elapsedTime = 0.f;
// 
// 	}
// 
// 	// 메인 프레임에 상태바 업데이트 요청
// 	if (pMainFrame != nullptr)
// 	{
// 		pMainFrame->UpdateTime(m_FPS, m_ms);
// 	}

	return CWinAppEx::Run();
}


BOOL CSwlEditorApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 기본 클래스의 OnIdle 함수 호출
	if (!CWinAppEx::OnIdle(lCount))
	{
		return FALSE;
	}
	else
	{
		// DyTimer 업데이트
		m_pTimer->Tick();

		// FPS 및 MS 계산
		int FPS = static_cast<int>(1.0 / m_pTimer->DeltaTime());
		float ms = m_pTimer->DeltaTime() * 1000.0f;

		// 마우스 이벤트를 통해 자주 호출되는 경우를 방지하기 위한 일정 간격 설정
		static float elapsedTime;
		elapsedTime += m_pTimer->DeltaTime();

		m_pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
		m_pDocument = dynamic_cast<CSwlEditorDoc*>(m_pMainFrame->GetActiveDocument());
		m_pView = dynamic_cast<CSwlEditorView*>(m_pMainFrame->GetActiveView());
		if (elapsedTime > 1.f)
		{
			// 상태바 업데이트
			if (m_pMainFrame != nullptr)
			{
				m_pMainFrame->UpdateTime(FPS, ms);
			}

			elapsedTime -= 1.f;
		}
		
		m_pDocument->Update();
		m_pView->BeginRender();
		m_pView->Render();
		m_pView->EndRender();

	}

	return CWinAppEx::OnIdle(lCount);
}


//int CAboutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
//		return -1;
//
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

//	return 0;
//}

