#pragma once
#include "afxcmn.h"
#include "SetG.h"
#include "RtuSetDlg.h"
// CMapCtrlDlg dialog

class CMapCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapCtrlDlg)

public:
	CMapCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapCtrlDlg();

// Dialog Data
	enum { IDD = IDD_MAPCTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CReportCtrl m_MapList;
	CString strID;
	CString strName;
	CString id;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkListTerminal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	void ShowLocalInfo(GPRSInfo* pGetInfo);
	void ShowTerminalInfo(UINT nItem);
	void SendGMessage(void);
	void SendTMessage(char* c);
	CString m_GGGGID;
	bool m_later;
	bool m_True;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
