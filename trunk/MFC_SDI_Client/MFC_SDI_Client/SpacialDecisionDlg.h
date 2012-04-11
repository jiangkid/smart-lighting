#pragma once


// CSpacialDecisionDlg dialog

class CSpacialDecisionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSpacialDecisionDlg)

public:
	CSpacialDecisionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSpacialDecisionDlg();

// Dialog Data
	enum { IDD = IDD_Spcial_S };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
