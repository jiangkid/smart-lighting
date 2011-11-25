#if !defined(AFX_ONLINE_H__09748AAC_0BF5_4491_BD73_D7A76109DCC6__INCLUDED_)
#define AFX_ONLINE_H__09748AAC_0BF5_4491_BD73_D7A76109DCC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OnVIewStatusList.h"
// COnViewStatus
//class COnVIewStatusList;
class COnViewStatus : public CDialogBar
{
	DECLARE_DYNAMIC(COnViewStatus)

public:
//	void OnSize(UINT nType, int cx, int cy);
	COnVIewStatusList* m_ListCtrl;
	COnViewStatus();
	virtual ~COnViewStatus();

protected:
//	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
private:
	void AddExStyle();
};

#endif
