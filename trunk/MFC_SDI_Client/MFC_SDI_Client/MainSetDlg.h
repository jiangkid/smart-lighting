#pragma once


// CMainSetDlg dialog

class CMainSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainSetDlg)

public:
	CMainSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMainSetDlg();

// Dialog Data
	enum { IDD = IDD_MainSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_GNum;
	int m_TNum;
	int m_RNum;
	int m_LNum;
	afx_msg void OnBnClickedBtnGetid();
	void ShowMessage(unsigned char* pszMessage, int nLength,CString str);
	void ShowUpdatatMessage(CString str);
	afx_msg void OnBnClickedBtnSave();
	CString m_NewID;
	CString CharToCString(unsigned char* str, int nLength);
protected:
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CString m_gprsid;
	CString m_tid;
	CString m_roudid;
	CString m_lightid;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton3();
	CString m_error;
	afx_msg void OnBnClickedCancel();
};
