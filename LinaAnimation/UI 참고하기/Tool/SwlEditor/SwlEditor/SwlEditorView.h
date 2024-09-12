
// SwlEditorView.h: CSwlEditorView 클래스의 인터페이스
//

#pragma once

class CMainFrame;
class DyTimer;
class IRenderer;

class CSwlEditorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CSwlEditorView() noexcept;
	DECLARE_DYNCREATE(CSwlEditorView)

// 특성입니다.
public:
	CSwlEditorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CSwlEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DyTimer* m_pTimer;
	CMainFrame* m_pMainFrame;
	IRenderer* m_pRenderer;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // SwlEditorView.cpp의 디버그 버전
inline CSwlEditorDoc* CSwlEditorView::GetDocument() const
   { return reinterpret_cast<CSwlEditorDoc*>(m_pDocument); }
#endif

