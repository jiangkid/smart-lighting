#pragma once


// CMphoneDlg dialog

class CMphoneDlg : public CDialog
{
	DECLARE_DYNAMIC(CMphoneDlg)

public:
	CMphoneDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMphoneDlg();

// Dialog Data
	enum { IDD = IDD_MPhone };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_password;
	CString m_strphone;
	afx_msg void OnBnClickedBtnY();
	virtual BOOL OnInitDialog();
	bool EnableWindow(unsigned int uID, bool bEnable);
};
