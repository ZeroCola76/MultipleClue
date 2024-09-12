
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

#include "Scene.h"
#include "JsonUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSwlEditorDoc

IMPLEMENT_DYNCREATE(CSwlEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CSwlEditorDoc, CDocument)
	ON_COMMAND(ID_FILE_NEW, &CSwlEditorDoc::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CSwlEditorDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CSwlEditorDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CSwlEditorDoc::OnFileSaveAs)

END_MESSAGE_MAP()


// CSwlEditorDoc 생성/소멸

CSwlEditorDoc::CSwlEditorDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_pJsonUtil = std::make_shared<JsonUtil>();
	m_pScene = std::make_shared<Scene>();
}

CSwlEditorDoc::~CSwlEditorDoc()
{

}

/*
BOOL CSwlEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}


BOOL CSwlEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return TRUE;
}


BOOL CSwlEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDocument::OnSaveDocument(lpszPathName);
}
*/


// CSwlEditorDoc serialization

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CSwlEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
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
		CMFCFilterChunkValueImpl* pChunk = nullptr;
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
// Doc 클래스가 아닌 APP이나 MainFrame에서 재정의 해야한다네요.
// ->아니라 그냥 여기서 하는게 맞다네요.

/// <summary>
/// '새로 만들기'이벤트 처리 함수 재정의
/// </summary>
void CSwlEditorDoc::OnFileNew()
{
	m_pJsonUtil->SetData(0);
}

/// <summary>
/// '열기'이벤트 처리 함수 재정의
/// </summary>
void CSwlEditorDoc::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(true, 0, 0, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY
		, _T("JSON Files (*.json)|*.json|"), 0, 0, true);

	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		m_pJsonUtil->LoadData(std::wstring(fileName.GetString()));
		Json::Value val = m_pJsonUtil->GetData();
	}
}

/// <summary>
/// '저장'이벤트 처리 함수 재정의
/// </summary>
void CSwlEditorDoc::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_pJsonUtil->SaveData();
}

/// <summary>
/// '다른 이름으로 저장'이벤트 처리 함수 재정의
/// </summary>
void CSwlEditorDoc::OnFileSaveAs()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlg(false, 0, 0, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY
		, _T("JSON Files (*.json)|*.json|"), 0, 0, true);

	if (dlg.DoModal() == IDOK)
	{
		CString fileName = dlg.GetPathName();
		m_pJsonUtil->SaveData(std::wstring(fileName.GetString()));
	}
}

/// <summary>
/// Application 클래스의 OnIdle루프에서 호출할 Document 클래스의 업데이트 함수입니다.
/// </summary>
void CSwlEditorDoc::Update()
{

}

/// <summary>
/// 데이터 직렬화 함수입니다.
/// 현재 씬의 정보들을 Json::Value로 변환하거나 Json::Value를 씬의 정보들로 변환합니다.
/// </summary>
void CSwlEditorDoc::Serialize()
{
	// m_pScene -> m_pJsonUtil->SetData();
	
}

