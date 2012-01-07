#pragma once


// CIPSetDlg dialog

class CIPSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CIPSetDlg)

public:
	CIPSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIPSetDlg();

	// Dialog Data
	enum { IDD = IDD_Config };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_szIP;
	UINT m_szPort;
};
