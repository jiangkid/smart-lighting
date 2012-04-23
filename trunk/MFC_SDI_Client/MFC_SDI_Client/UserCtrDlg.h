#pragma once
#include "reportctrl.h"
#include "SetNewUser.h"

// CUserCtrDlg dialog

class CUserCtrDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserCtrDlg)

public:
	CUserCtrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserCtrDlg();
	virtual void OnCancel();
// Dialog Data
	enum { IDD = IDD_User_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PostNcDestroy();
	
public:
	virtual BOOL OnInitDialog();
	CReportCtrl m_List;
	void ShowUserInfo(int nItem, MUserInfo* pGetInfo);
	afx_msg void OnNMDblclkListUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton13();
	CString m_UserName;
	CString m_strOldP;
	CString m_new1;
	CString m_new2;
	CString m_phone;
	CString m_areaname;
	afx_msg void OnBnClickedM();
	afx_msg void OnBnClickedRenew();
	bool EnableWindow(UINT uID, bool bEnable);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	int GetChenkRadio(void);
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedDeleteuser();
};
