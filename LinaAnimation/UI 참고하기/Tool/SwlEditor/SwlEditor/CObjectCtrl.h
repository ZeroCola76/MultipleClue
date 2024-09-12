#pragma once
#include "afxdialogex.h"
#include "TestObject.h"

class CMainFrame;
class CSwlEditorDoc;

// CObjectCtrl 대화 상자

/// <summary>
/// DokingPane에서 값들을 조정 할 수 있는 대화상자입니다.
/// 현재 씬의 정보들을 변경할 수 있도록 설계할 예정입니다.
/// </summary>
class CObjectCtrl : public CDialogEx
{
	DECLARE_DYNAMIC(CObjectCtrl)

public:
	CObjectCtrl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CObjectCtrl();

	afx_msg void OnBnClickedButtonObjAdd();
	afx_msg void OnBnClickedButtonObjDel();
	afx_msg void OnBnClickedButtonComponentAdd();
	afx_msg void OnBnClickedButtonComponentDel();
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditRotX();
	afx_msg void OnEnChangeEditRotY();
	afx_msg void OnEnChangeEditRotZ();
	afx_msg void OnEnChangeEditSclX();
	afx_msg void OnEnChangeEditSclY();
	afx_msg void OnEnChangeEditSclZ();
	afx_msg void OnCbnSelchangeComboObjType();
	afx_msg void OnLbnDblclkListObj();
	afx_msg void OnLbnDblclkListComponent();	
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_objPosX;
	CString m_objPosY;
	CString m_objPosZ;
	CString m_objRotX;
	CString m_objRotY;
	CString m_objRotZ;
	CString m_objSclX;
	CString m_objSclY;
	CString m_objSclZ;
	CListBox m_listObj;
	CListBox m_listComponent;
	CComboBox m_cbObjType;
	CButton m_objDel;
	CButton m_objAdd;
	CButton m_componentDel;
	CButton m_componentAdd;
	virtual BOOL OnInitDialog();

private:
	// CHANGE_Doyo : MFC 템플릿 기초 클래스들의 포인터
	CMainFrame* m_pFrame;
	CSwlEditorDoc* m_pDoc;

	// 테스트용 오브젝트
	Test::TestObject m_pTest;
public:
	virtual void Serialize(CArchive& ar);
	
};
