#pragma once
#include "CObjectCtrl.h"
// CObjCtrlPane

/// <summary>
/// ��ŷ ����Ʈ�� ��ŷ�ǰų� �� â�� ���Ե� �� �ִ� â�Դϴ�.
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


