#pragma once
#include "afxcmn.h"
#include "reportctrl.h"

// CAllDesitionCtrl dialog

class CAllDesitionCtrl : public CDialog
{
	DECLARE_DYNAMIC(CAllDesitionCtrl)

public:
	CAllDesitionCtrl(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAllDesitionCtrl();

// Dialog Data
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	CReportCtrl m_List;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
};
