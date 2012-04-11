#pragma once
//#include "ControlWaringDlg.h"
// CWarningDlg dialog

class CWarningDlg : public CDialog
{
	DECLARE_DYNAMIC(CWarningDlg)

public:
	CWarningDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWarningDlg();

// Dialog Data
	enum { IDD = IDD_WARNING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
public:
	CReportCtrl m_ListWarning;
	CDateTimeCtrl m_DataBegin;
	CDateTimeCtrl m_DataEnd;
	afx_msg void OnBnClickedBtnGet();
	void OneWarningToShow(WarningInfo* pWarning, int nRow);
	void AllWarningToShow(int nWarningCount);
	afx_msg void OnNMDblclkWarningDlg(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_TheGID;
	int warningpackCount;
	//CControlWaringDlg* m_pCtrlWarn;
};