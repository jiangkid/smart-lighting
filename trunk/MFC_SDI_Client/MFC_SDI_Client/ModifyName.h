#pragma once


// CModifyName dialog

class CModifyName : public CDialog
{
	DECLARE_DYNAMIC(CModifyName)

public:
	CModifyName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyName();

// Dialog Data
	enum { IDD = IDD_MName };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnYes();
	CString m_strOldName;
	CString m_passwold;
	CString m_newName;
	virtual BOOL OnInitDialog();
};
