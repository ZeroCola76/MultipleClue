
// SwlEditorDoc.h: CSwlEditorDoc 클래스의 인터페이스
//


#pragma once

class Scene;
class JsonUtil;

class CSwlEditorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CSwlEditorDoc() noexcept;
	DECLARE_DYNCREATE(CSwlEditorDoc)

	// 특성입니다.
public:

	// 작업입니다.
	void Update();
public:

	// 재정의입니다.
public:
	// virtual BOOL OnNewDocument();
	// virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	// virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	// 	virtual void Serialize(CArchive& ar);
	void Serialize();
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// 구현입니다.
public:
	virtual ~CSwlEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

private:
	std::shared_ptr<Scene> m_pScene;
	std::shared_ptr<JsonUtil> m_pJsonUtil;
public:
};
