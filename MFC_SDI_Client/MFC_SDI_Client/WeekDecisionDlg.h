#pragma once


// CWeekDecisionDlg dialog

class CWeekDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CWeekDecisionDlg)

public:
	CWeekDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWeekDecisionDlg();

// Dialog Data
	enum { IDD = IDD_Week_S };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
