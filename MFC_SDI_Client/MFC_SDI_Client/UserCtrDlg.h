#pragma once


// CUserCtrDlg dialog

class CUserCtrDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserCtrDlg)

public:
	CUserCtrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserCtrDlg();

// Dialog Data
	enum { IDD = IDD_User_CTRL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();
};
