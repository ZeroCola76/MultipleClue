
// MainFrm.h: CMainFrame 클래스의 인터페이스
//
#include "CObjCtrlPane.h"
#include "IRenderer.h"
#pragma once

class CMainFrame : public CFrameWndEx
{

protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

	// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter;
public:

	// 작업입니다.
public:

	// 재정의입니다.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

	// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:	// 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

	// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()

public:
	CObjCtrlPane m_paneInfo;

	int m_FPS;
	float m_ms;
	float m_deltaTime;
	void UpdateTime(int FPS, float ms);
	IRenderer* m_pRenderer;

	HWND m_hWnd;
	afx_msg void OnOpenDockingPane();
};


