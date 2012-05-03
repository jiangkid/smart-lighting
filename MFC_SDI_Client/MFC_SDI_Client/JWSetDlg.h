#pragma once


// CJWSetDlg dialog

class CJWSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CJWSetDlg)

public:
	CJWSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CJWSetDlg();
	int nGeshu;
// Dialog Data
	enum { IDD = IDD_JWDU_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString strLA;
	CString strLO;
	void ShowJW(void);
};
