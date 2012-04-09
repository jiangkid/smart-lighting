#pragma once

#include "PrintView.h"

// CPrintFrame 框架

class CPrintFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CPrintFrame)
public:
	CPrintFrame();           // 动态创建所使用的受保护的构造函数
	CPrintFrame(CDialog* pMainDlg);
	virtual ~CPrintFrame();

	void	DoPrintView();
	void	DoPrint();

public:
	CDialog*	m_pMainDlg;
	CPrintView* m_pPrintView;

public:
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
};