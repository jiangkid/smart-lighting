#pragma once

#include "ReportCtrl.h"
#include "afxwin.h"
#include "AllDesitionCtrl.h"
// CDCTRLDlg dialog

class CDCTRLDlg : public CDialog
{
	DECLARE_DYNAMIC(CDCTRLDlg)

public:
	CDCTRLDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDCTRLDlg();

// Dialog Data
	enum { IDD = IDD_DCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	CReportCtrl m_List;
	void ShowBandDesition(void);
	CComboBox m_Zbox;
	CComboBox m_Jbox;
	CComboBox m_Tbox;
	CString strID;
	void InitialBox(void);
	void EnableButton2(CString str);
	void ShowBandInfoDate(StrategyInfo* pGetInfo);
	void ShowBandInfoWeek(StrategyInfo* pGetInfo);
	void ShowBandInfoJie(StrategyInfo* pGetInfo);
	void ShowBandInfoTe(StrategyInfo* pGetInfo);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
};
