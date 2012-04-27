#pragma once


// CJWSetDlg dialog

class CJWSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CJWSetDlg)

public:
	CJWSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJWSetDlg();

// Dialog Data
	enum { IDD = IDD_JWDU_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk2();
};
