#pragma once
#include "afxcmn.h"
#include "Modify.h"
#include "ModifyName.h"
#include "MphoneDlg.h"
// CModifyAllDlg dialog

class CModifyAllDlg : public CDialog
{
	DECLARE_DYNAMIC(CModifyAllDlg)

public:
	CModifyAllDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyAllDlg();

// Dialog Data
	enum { IDD = IDD_MAll };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CModify* m_pModifyPass;
	CModifyName* m_pModifyName;
	CMphoneDlg* m_pMPhone;
	CTabCtrl m_Tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
