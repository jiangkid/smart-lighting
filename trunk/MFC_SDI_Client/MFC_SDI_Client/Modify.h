#pragma once


// CModify dialog

class CModify : public CDialog
{
	DECLARE_DYNAMIC(CModify)

public:
	CModify(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModify();

// Dialog Data
	enum { IDD = IDD_Modify };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_UserName;
	CString m_OldPassWord;
	CString m_FNewPassWord;
	CString m_SNewPassWord;
	afx_msg void OnBnClickedOk();
	void ClearBoard(void);
	virtual BOOL OnInitDialog();
};
