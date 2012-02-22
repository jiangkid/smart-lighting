#pragma once


// CDoGWorkDlg dialog

class CDoGWorkDlg : public CDialog
{
	DECLARE_DYNAMIC(CDoGWorkDlg)

public:
	CDoGWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDoGWorkDlg();

// Dialog Data
	enum { IDD = IDD_GWork };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_GName;
	CString m_GID;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
};
