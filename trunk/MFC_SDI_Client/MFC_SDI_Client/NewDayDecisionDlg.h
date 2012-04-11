#pragma once
#include "afxdtctl.h"


// CNewDayDecisionDlg dialog

class CNewDayDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewDayDecisionDlg)

public:
	CNewDayDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewDayDecisionDlg();

// Dialog Data
	enum { IDD = IDD_New_DayD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Type;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnSure();
	CDateTimeCtrl m_BeginOpenAll;
	CDateTimeCtrl m_EndOpenAll;
	CDateTimeCtrl m_BeginHalf;
	CDateTimeCtrl m_EndHalf;
	CString strName;
};
