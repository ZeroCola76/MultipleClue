// CObjectCtrl.cpp: 구현 파일
//

#include "pch.h"
#include "SwlEditor.h"
#include "afxdialogex.h"
#include "CObjectCtrl.h"
#include "MainFrm.h"
#include "SwlEditorDoc.h"
#include "Scene.h"
#include <iostream>

// CObjectCtrl 대화 상자

IMPLEMENT_DYNAMIC(CObjectCtrl, CDialogEx)

CObjectCtrl::CObjectCtrl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_INFO, pParent)
	, m_objPosX(0.0)
	, m_objPosY(0.0)
	, m_objPosZ(0.0)
	, m_objRotX(0.0)
	, m_objRotY(0.0)
	, m_objRotZ(0.0)
	, m_objSclX(0.0)
	, m_objSclY(0.0)
	, m_objSclZ(0.0)
{
}

CObjectCtrl::~CObjectCtrl()
{
}

void CObjectCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_objPosX);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_objPosY);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_objPosZ);
	DDX_Text(pDX, IDC_EDIT_ROT_X, m_objRotX);
	DDX_Text(pDX, IDC_EDIT_ROT_Y, m_objRotY);
	DDX_Text(pDX, IDC_EDIT_ROT_Z, m_objRotZ);
	DDX_Text(pDX, IDC_EDIT_SCL_X, m_objSclX);
	DDX_Text(pDX, IDC_EDIT_SCL_Y, m_objSclY);
	DDX_Text(pDX, IDC_EDIT_SCL_Z, m_objSclZ);
	DDX_Control(pDX, IDC_LIST_OBJ, m_listObj);
	DDX_Control(pDX, IDC_LIST_COMPONENT, m_listObj);
	DDX_Control(pDX, IDC_COMBO_OBJ_TYPE, m_cbObjType);
	DDX_Control(pDX, IDC_BUTTON_OBJ_DEL, m_objDel);
	DDX_Control(pDX, IDC_BUTTON_OBJ_ADD, m_objAdd);
	DDX_Control(pDX, IDC_BUTTON_COMPONENT_DEL, m_objDel);
	DDX_Control(pDX, IDC_BUTTON_COMPONENT_ADD, m_objAdd);

	// 데이터 유효성 검사
	DDV_MinMaxDouble(pDX, m_objPosX, -100000.0, 100000.0);
	DDV_MinMaxDouble(pDX, m_objPosY, -100000.0, 100000.0);
	DDV_MinMaxDouble(pDX, m_objPosZ, -100000.0, 100000.0);
	DDV_MinMaxDouble(pDX, m_objRotX, 0.0, 360.0);
	DDV_MinMaxDouble(pDX, m_objRotY, 0.0, 360.0);
	DDV_MinMaxDouble(pDX, m_objRotZ, 0.0, 360.0);
	DDV_MinMaxDouble(pDX, m_objSclX, -100000.0, 100000.0);
	DDV_MinMaxDouble(pDX, m_objSclY, -100000.0, 100000.0);
	DDV_MinMaxDouble(pDX, m_objSclZ, -100000.0, 100000.0);

}


BEGIN_MESSAGE_MAP(CObjectCtrl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_ADD, &CObjectCtrl::OnBnClickedButtonObjAdd)
	ON_BN_CLICKED(IDC_BUTTON_OBJ_DEL, &CObjectCtrl::OnBnClickedButtonObjDel)
	ON_BN_CLICKED(IDC_BUTTON_COMPONENT_ADD, &CObjectCtrl::OnBnClickedButtonComponentAdd)
	ON_BN_CLICKED(IDC_BUTTON_COMPONENT_DEL, &CObjectCtrl::OnBnClickedButtonComponentDel)
	ON_EN_CHANGE(IDC_EDIT_POS_X, &CObjectCtrl::OnEnChangeEditPosX)
	ON_EN_CHANGE(IDC_EDIT_POS_Y, &CObjectCtrl::OnEnChangeEditPosY)
	ON_EN_CHANGE(IDC_EDIT_POS_Z, &CObjectCtrl::OnEnChangeEditPosZ)
	ON_EN_CHANGE(IDC_EDIT_ROT_X, &CObjectCtrl::OnEnChangeEditRotX)
	ON_EN_CHANGE(IDC_EDIT_ROT_Y, &CObjectCtrl::OnEnChangeEditRotY)
	ON_EN_CHANGE(IDC_EDIT_ROT_Z, &CObjectCtrl::OnEnChangeEditRotZ)
	ON_EN_CHANGE(IDC_EDIT_SCL_X, &CObjectCtrl::OnEnChangeEditSclX)
	ON_EN_CHANGE(IDC_EDIT_SCL_Y, &CObjectCtrl::OnEnChangeEditSclY)
	ON_EN_CHANGE(IDC_EDIT_SCL_Z, &CObjectCtrl::OnEnChangeEditSclZ)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJ_TYPE, &CObjectCtrl::OnCbnSelchangeComboObjType)
	ON_LBN_DBLCLK(IDC_LIST_OBJ, &CObjectCtrl::OnLbnDblclkListObj)
	ON_LBN_DBLCLK(IDC_LIST_COMPONENT, &CObjectCtrl::OnLbnDblclkListObj)
END_MESSAGE_MAP()


// CObjectCtrl 메시지 처리기
/// 이하 도킹팬의 이벤트 처리기 함수들


BOOL CObjectCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_pFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (m_pFrame)
	{
		m_pDoc = dynamic_cast<CSwlEditorDoc*>(m_pFrame->GetActiveDocument());
	}

	// TODO_DOYO : objectList Box에 object들을 추가하는 코드를 추가할것
	// 임시로 문자열 추가해둠
	// m_listObj.AddString(_T("Cube"));
	// m_listObj.AddString(_T("Sphere"));
	// m_listObj.AddString(_T("Light"));
	// m_listObj.AddString(_T("Camera"));
	// m_listObj.AddString(reinterpret_cast<LPCTSTR>(m_pTest->name.c_str()));
	//  for (auto& obj : m_pScene->m_pObj)
	{
		//m_listObj.AddString(obj->name);
	}
	// TODO_DOYO : 오브젝트 타입들을 생성하기 위해 만든 콤보 박스, 오브젝트 타입과 연계할것
	m_cbObjType.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectCtrl::OnBnClickedButtonObjAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ComboBox에서 선택되어있는 오브젝트 타입을 ListBox에 추가합니다.
	// Factory 객체를 생성하고 Factory 에서 오브젝트를 생성하도록 설계할것
	// 우선 임시로 test 오브젝트 생성
	m_pTest = new TestObject();
}


void CObjectCtrl::OnBnClickedButtonObjDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트를 제거합니다.
}


void CObjectCtrl::OnBnClickedButtonComponentAdd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : 선택중인 Object에서 ComponentListBox에 선택되어있는 Component를 추가합니다.
}

void CObjectCtrl::OnBnClickedButtonComponentDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : 선택중인 Object에서 ComponentListBox에 선택되어있는 Component를 삭제합니다.
}

void CObjectCtrl::OnEnChangeEditPosX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 PosX값을 에디터로부터 입력받은 값으로 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_POS_X, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditPosY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 PosY값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_POS_Y, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditPosZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 PosZ값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_POS_Z, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditRotX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 RotX값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_ROT_X, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditRotY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 RotY값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_ROT_Y, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditRotZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 RotZ값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_ROT_Z, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditSclX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 SclX값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_SCL_X, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditSclY()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 SclY값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_SCL_Y, str);
	//if(m_pTest)
	{
	}
}


void CObjectCtrl::OnEnChangeEditSclZ()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : ListBox에서 선택되어있는 오브젝트의 SclZ값을 변경합니다.
	CString str;
	GetDlgItemText(IDC_EDIT_SCL_Z, str);
	//if (m_pTest)
	{
	}
}


void CObjectCtrl::OnCbnSelchangeComboObjType()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : 추가할 오브젝트 타입을 ComboBox에서 선택합니다.
	switch (m_cbObjType.GetCurSel())
	{
	case 0:
		break;
	case 1:
		break;
	default:
		break;
	}
}


void CObjectCtrl::OnLbnDblclkListObj()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : Value를 변경하거나 삭제할 오브젝트를 ListBox에서 선택합니다.
}


void CObjectCtrl::OnLbnDblclkListComponent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO_Doyo : Value를 변경하거나 삭제할 오브젝트를 ListBox에서 선택합니다.
}


/// <summary>
/// CArchive 객체를 사용해서 데이터를 직렬화하는 함수입니다.
/// Dcument의 Serialize()함수 내부에서 작동하게 만들예정입니다.
/// 데이터 직렬화를 Json형식으로 할 예정이라 더 이상 사용하지 않을 함수입니다.
/// </summary>
/// <param name="ar"></param>
void CObjectCtrl::Serialize(CArchive& ar)
{
// TODO_Doyo : 대신에 Json을 활용한 함수를 새로 만들것
	if (ar.IsStoring())
	{	// storing code
		// ar << m_pTest.pos.x << m_pTest.pos.y << m_pTest.pos.z
		// 	<< m_pTest.rot.x << m_pTest.rot.y << m_pTest.rot.z
		// 	<< m_pTest.scl.x << m_pTest.scl.y << m_pTest.scl.z;
	}
	else
	{	// loading code
		// ar >> m_pTest.pos.x >> m_pTest.pos.y >> m_pTest.pos.z
		// 	>> m_pTest.rot.x >> m_pTest.rot.y >> m_pTest.rot.z
		// 	>> m_pTest.scl.x >> m_pTest.scl.y >> m_pTest.scl.z;
	}
}
