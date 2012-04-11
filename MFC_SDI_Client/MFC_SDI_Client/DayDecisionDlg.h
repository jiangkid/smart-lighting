#pragma once


// CDayDecisionDlg dialog

class CDayDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CDayDecisionDlg)

public:
	CDayDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDayDecisionDlg();

// Dialog Data
	enum { IDD = IDD_Day_S };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
