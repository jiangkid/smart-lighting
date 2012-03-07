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
	afx_msg void OnBnClickedBtnSave();
	CString m_NewID;
	CString CharToCString(unsigned char* str, int nLength);
protected:
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
