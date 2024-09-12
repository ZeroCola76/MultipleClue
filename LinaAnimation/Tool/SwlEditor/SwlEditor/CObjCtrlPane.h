#pragma once
#include "CObjectCtrl.h"
// CObjCtrlPane

/// <summary>
/// 도킹 사이트에 도킹되거나 탭 창에 포함될 수 있는 창입니다.
/// 
/// </summary>
class CObjCtrlPane : public CDockablePane
{
	DECLARE_DYNAMIC(CObjCtrlPane)

public:
	CObjCtrlPane();
	virtual ~CObjCtrlPane();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CObjectCtrl m_objCtrl;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


