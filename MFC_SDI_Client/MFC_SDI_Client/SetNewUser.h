#pragma once
#include "afxwin.h"


// CSetNewUser dialog

class CSetNewUser : public CDialog
{
	DECLARE_DYNAMIC(CSetNewUser)

public:
	CSetNewUser(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetNewUser();

// Dialog Data
	enum { IDD = IDD_SetUser };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeUsername();
	afx_msg void OnCbnSelchangeCombo1();
	BOOL IsBegin;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	CString m_strAreaName;
};
