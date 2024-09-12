// CObjCtrlPane.cpp: 구현 파일
//

#include "pch.h"
#include "SwlEditor.h"
#include "CObjectCtrl.h"
#include "CObjCtrlPane.h"


// CObjCtrlPane

IMPLEMENT_DYNAMIC(CObjCtrlPane, CDockablePane)

CObjCtrlPane::CObjCtrlPane()
{

}

CObjCtrlPane::~CObjCtrlPane()
{
}


BEGIN_MESSAGE_MAP(CObjCtrlPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CObjCtrlPane 메시지 처리기




int CObjCtrlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (!m_objCtrl.Create(IDD_DIALOG_INFO, this))
	{
		TRACE0("오브젝트 컨트롤 윈도우를 만들지 못했습니다.\n");
		return -1;
	}
	m_objCtrl.ShowWindow(SW_SHOW);
	return 0;
}


void CObjCtrlPane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_objCtrl.GetSafeHwnd())
	{
		m_objCtrl.MoveWindow(0, 0, cx, cy);
		m_objCtrl.SetFocus();
	}
}
