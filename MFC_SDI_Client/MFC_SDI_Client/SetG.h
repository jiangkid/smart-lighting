#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CSetG dialog

class CSetG : public CDialog
{
	DECLARE_DYNAMIC(CSetG)

public:
	CSetG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetG();

// Dialog Data
	enum { IDD = IDD_Set };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_GName;

	bool EnableWindow(UINT uID, bool bEnable);
	afx_msg void OnBnClickedRadiog();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadiot();
	afx_msg void OnBnClickedRadior();
	afx_msg void OnBnClickedRadiol();
	CComboBox m_ComG;
	CComboBox m_ComT;
	CComboBox m_ComR;
	CComboBox m_ComL;
	CString ShowMessage(char* pszMessage, int nLength);
	afx_msg void OnBnClickedBtnGset();
	CString m_TName;
	CString m_RName;
	CString m_LName;
	CComboBox m_UserNameSet;
	afx_msg void OnBnClickedBtnTset();
	afx_msg void OnBnClickedBtnRset();
	afx_msg void OnBnClickedBtnLset();
	CString m_GPhoneNum;
	CString m_GArea;
	CDateTimeCtrl m_DataCtrl;
	CString m_GBusy;
	CDateTimeCtrl m_TTime;
	CString m_TArea;
	CString m_TBusy;
	afx_msg void OnCbnSelchangeComTid();
};
