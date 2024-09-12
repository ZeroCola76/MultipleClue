
// SwlEditorView.cpp: CSwlEditorView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SwlEditor.h"
#endif

#include "SwlEditorDoc.h"
#include "SwlEditorView.h"
#include "MainFrm.h"
#include "DyTimer.h"
#include "IRenderer.h"
#include "PvRenderer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSwlEditorView

IMPLEMENT_DYNCREATE(CSwlEditorView, CView)

BEGIN_MESSAGE_MAP(CSwlEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CSwlEditorView 생성/소멸

CSwlEditorView::CSwlEditorView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
}

CSwlEditorView::~CSwlEditorView()
{
}

BOOL CSwlEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CSwlEditorView 그리기

// void CSwlEditorView::OnDraw(CDC* pDC)
// {
// 	CSwlEditorDoc* pDoc = GetDocument();
// 	ASSERT_VALID(pDoc);
// 	if (!pDoc)
// 		return;
// 
// // 	BeginRender();
// // 	Render();
// // 	EndRender();
// 
// }

void CSwlEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSwlEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSwlEditorView 진단

#ifdef _DEBUG
void CSwlEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CSwlEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSwlEditorDoc* CSwlEditorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSwlEditorDoc)));
	return (CSwlEditorDoc*)m_pDocument;
}

void CSwlEditorView::BeginRender()
{
	m_pRenderer->BeginRender();
}

/// <summary>
/// OnDraw 함수를 대신하여 렌더링을 담당할 함수입니다.
/// </summary>
void CSwlEditorView::Render()
{
	// TODO_Doyo : Document 클래스가 멤버로 가지고있는 오브젝트들을 그리는 코드 추가할것
	m_pRenderer->DrawSomething();

	/* 대충 이런느낌으로?
	CSwlEditorDoc* pDocument = GetDocument();
	for (const auto& object : pDocument->GetObject())
	{
		object.Render();
	}
	*/
}

/// <summary>
/// 
/// </summary>
void CSwlEditorView::EndRender()
{
	m_pRenderer->EndRender();
}

#endif //_DEBUG


// CSwlEditorView 메시지 처리기


void CSwlEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// TODO_DOYO : 메인프레임의 포인터를 얻고, 마우스의 위치를 출력한다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString str;
	str.Format(_T("Mouse Pos %d, %d"), point.x, point.y);

	pFrame->m_wndStatusBar.SetPaneText(4, str);
	CView::OnMouseMove(nFlags, point);
}


int CSwlEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pRenderer = new PvRenderer();
	m_pRenderer->Initialize(reinterpret_cast<int>(GetSafeHwnd()), 1024, 720);

	return 0;
}
