#pragma once
#include "afxcmn.h"
#include "ReportCtrl.h"
#include "LightCtrlDlg.h"

class CLightView : public CDialog
{
	DECLARE_DYNAMIC(CLightView)

public:
	CString m_szRName;
	CString m_szLName;
	CString m_szLID;
	CString m_szLID1;
	CString m_szGName;
	CString m_szMainStatus;
	CString m_szAssistStatus;
	int     nCount;
	CLightView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightView();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CReportCtrl m_List;
	void LightToView(int nLCount);
	bool ItemdataProc(DWORD dwData, LPARAM lParam);
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnUpdata();
	//CLightCtrlDlg* pDlg;
	afx_msg void OnNMDblclkLightView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickLightView(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUpdataCurrent();
	afx_msg void OnUpdataStatus();
	void UpdataOneLight(ZigbeeInfo* pGetInfo);
	afx_msg void OnLvnItemchangedLightView(NMHDR *pNMHDR, LRESULT *pResult);
};
