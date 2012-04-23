#pragma once
#include "afxwin.h"
#include "reportctrl.h"


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
	CReportCtrl m_List;
	afx_msg void OnBnClickedButton1();
	void FullFillList(int nItem, USERINFO* strLocationName);
	void FullFillListBind(int nItem, BIND* pGetInfo);
	afx_msg void OnBnClickedButton10();
	CReportCtrl m_List1;
	afx_msg void OnNMDblclkListGlobal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL EnableWindow(UINT uID, bool bEnable);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CString str1;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnBnClickedRadio3();
};
