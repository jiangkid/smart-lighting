#pragma once
#include "afxcmn.h"
#include "ReportCtrl.h"
#include "DoRWorkDlg.h"
// CRoudInfoView dialog

class CRoudInfoView : public CDialog
{
	DECLARE_DYNAMIC(CRoudInfoView)

public:
	CRoudInfoView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRoudInfoView();
	int     nCount;
	CString m_szRName;
	CString m_szLName;
	CString m_szLID;
	CString m_szLID1;
	CString m_szGName;
	//CDoRWorkDlg* pDlg;
// Dialog Data
	enum { IDD = IDD_Roud_Info };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CReportCtrl m_List;
	void RoadInfoToView(int nRCount);
	void UpdataOneRoad(RoadListViewInfo* pGetInfo);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedUpdataAll();
	U8* GPRSTranslationID(U8* buffer, int Length);
	afx_msg void OnNMDblclkRList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickRList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRcUpdata();
	afx_msg void OnRsUpdata();
	afx_msg void OnRc2Updata();
	afx_msg void OnRc3Updata();
};
