
// SwlEditorDoc.cpp: CSwlEditorDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SwlEditor.h"
#endif

#include "SwlEditorDoc.h"
#include "MainFrm.h"
#include "CObjectCtrl.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSwlEditorDoc

IMPLEMENT_DYNCREATE(CSwlEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSwlEditorDoc, CDocument)
END_MESSAGE_MAP()


// CSwlEditorDoc 생성/소멸

CSwlEditorDoc::CSwlEditorDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CSwlEditorDoc::~CSwlEditorDoc()
{
}

BOOL CSwlEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CSwlEditorDoc serialization

/// <summary>
/// 데이터 직렬화 함수입니다.
/// 데이터 직렬화를 Json포맷으로 할 예정이라 더 이상 사용하지 않을 함수입니다.
/// </summary>
/// <param name="ar"></param>
void CSwlEditorDoc::Serialize(CArchive& ar)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//pFrame->m_paneInfo.m_objCtrl.Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CSwlEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CSwlEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSwlEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CSwlEditorDoc 진단

#ifdef _DEBUG
void CSwlEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSwlEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSwlEditorDoc 명령
