#pragma once
#include "afxcmn.h"
#include "DayDecisionDlg.h"
#include "WeekDecisionDlg.h"
#include "JieDecisionDlg.h"
#include "SpacialDecisionDlg.h"
// CDecisionDlg dialog

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
	CDayDecisionDlg*  m_pDayDlg;
	CWeekDecisionDlg* m_pWeekDlg;
	CJieDecisionDlg*  m_pJieDlg;
	CSpacialDecisionDlg* m_pSpacialDlg;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
