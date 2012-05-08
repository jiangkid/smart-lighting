#pragma once
#include "afxdtctl.h"
#include "reportctrl.h"


// CNewJDecisionDlg dialog

class CNewJDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CNewJDecisionDlg)

public:
	CNewJDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewJDecisionDlg();

// Dialog Data
	enum { IDD = IDD_New_JieD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_Tyepe;
	afx_msg void OnBnClickedBtnD();
	CDateTimeCtrl m_Data;
	virtual BOOL OnInitDialog();
	CReportCtrl m_List;
	afx_msg void OnNMRClickListData(NMHDR *pNMHDR, LRESULT *pResult);
	int m_NItem;
	afx_msg void On32896();
	afx_msg void OnBnClickedBtnc();
	afx_msg void OnBnClickedBtnss();
	CDateTimeCtrl m_ba;
	CDateTimeCtrl m_ea;
	//CDateTimeCtrl m_bh;
	//CDateTimeCtrl m_eh;
	CString m_JName;
};
