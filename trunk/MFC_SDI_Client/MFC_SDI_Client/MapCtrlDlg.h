#pragma once
#include "afxcmn.h"


// CMapCtrlDlg dialog

class CMapCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapCtrlDlg)

public:
	CMapCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapCtrlDlg();

// Dialog Data
	enum { IDD = IDD_MAPCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CReportCtrl m_MapList;
	CString strID;
	CString strName;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListTerminal(NMHDR *pNMHDR, LRESULT *pResult);
};
