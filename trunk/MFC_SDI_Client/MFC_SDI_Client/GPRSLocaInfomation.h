#pragma once
#include "ReportCtrl.h"

// CGPRSLocaInfomation dialog

class CGPRSLocaInfomation : public CDialog
{
	DECLARE_DYNAMIC(CGPRSLocaInfomation)

public:
	CGPRSLocaInfomation(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGPRSLocaInfomation();

// Dialog Data
	enum { IDD = IDD_GPRSLocal };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void ShowLocationInfo(GPRSInfo* pGetINfo);
	virtual BOOL OnInitDialog();
	CString m_gprsid;
	CString m_gprsname;
	CString m_gprsphone;
	CString m_gprsarea;
	CString m_gprslocation;
	CString m_gprstime;
	int     m_TerminalCount;
	CReportCtrl m_List_Terminal;
	TerminalInfo* GetSelTerminalInfo(void);
	void ShowTerminalInfo(int nTcount);
};
