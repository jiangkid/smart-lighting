#pragma once
#include "JWSetDlg.h"
#include "LightOnOffDlg.h"
#include "afxwin.h"
#include "DecisionDlg.h"
 
 
 

// CRtuSetDlg dialog

class CRtuSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CRtuSetDlg)

public:
	CRtuSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRtuSetDlg();

// Dialog Data
	enum { IDD = IDD_RTU_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRect m_rect;
	CTabCtrl m_RtuTab;
	CJWSetDlg *m_pJWDlg;
	CLightOnOffDlg *m_pLightDlg;
	CGPRSLocaInfomation* m_pGPRS;
	CDecisionDlg *m_pDecision;
	CString m_strTerminalID;
	CString m_strTerminalName;
	CString m_BoxName;
	CString m_BoxID;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	void ShowTerminalBoxInfo();
	 
};
