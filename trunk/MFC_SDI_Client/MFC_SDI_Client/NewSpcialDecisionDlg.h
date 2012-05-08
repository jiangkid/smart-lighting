#pragma once
#include "afxdtctl.h"


// CNewSpcialDecisionDlg dialog

class CNewSpcialDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewSpcialDecisionDlg)

public:
	CNewSpcialDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewSpcialDecisionDlg();

// Dialog Data
	enum { IDD = IDD_New_SpcialD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Type;
	afx_msg void OnBnClickedBtnQx();
	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_sd;
	CDateTimeCtrl m_ba;
	//CDateTimeCtrl m_bh;
	//CDateTimeCtrl m_eh;
	CDateTimeCtrl m_ea;
	afx_msg void OnBnClickedBtnBc();
};
