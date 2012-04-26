#pragma once
#include "afxwin.h"


// CLoginDlg dialog

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

	// Dialog Data
	enum { IDD = IDD_Login };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox;
	afx_msg void OnBnClickedOk();
	CString m_szNamePassword;
	virtual BOOL OnInitDialog();
private:
	bool m_bAuto;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnEditchangeComboId();
	int nIndex;
	afx_msg void OnBnClickedBtnSet();
	CString m_UserName;
	CString m_PassWord;
	afx_msg void OnBnClickedCancel();
};
