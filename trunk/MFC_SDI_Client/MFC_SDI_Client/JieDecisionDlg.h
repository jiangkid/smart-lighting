#pragma once


// CJieDecisionDlg dialog

class CJieDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CJieDecisionDlg)

public:
	CJieDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJieDecisionDlg();

// Dialog Data
	enum { IDD = IDD_Jie_S };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
