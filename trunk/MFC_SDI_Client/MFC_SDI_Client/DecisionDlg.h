#pragma once
#include "afxcmn.h"
// CDecisionDlg dialog
#include "NewJDecisionDlg.h"
#include "NewSpcialDecisionDlg.h"
#include "NewWeekDecisionDlg.h"
class CDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CDecisionDlg)

public:
	CDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDecisionDlg();

// Dialog Data
	enum { IDD = IDD_DECISION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	CNewWeekDecisionDlg* m_pWeekDlg;
	CNewJDecisionDlg*  m_pJieDlg;
	CNewSpcialDecisionDlg* m_pSpacialDlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
