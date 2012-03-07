#pragma once
#include "btnst.h"


// CLightCtrlDlg dialog

class CLightCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightCtrlDlg)

public:
	CLightCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightCtrlDlg();

// Dialog Data
	enum { IDD = IDD_LIGHT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_GName;
	CString m_RName;
	CString m_LName;
	CString m_LID;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	CButtonST m_MainStatus;
	CButtonST m_AssistStatus;
	afx_msg void OnBnClickedBtnDon();
	afx_msg void OnBnClickedBtnMain();
	afx_msg void OnBnClickedBtnAssist();
	afx_msg void OnBnClickedBtnDoff();
};
