#pragma once


// CGprsInfoCtrlDlg dialog

class CGprsInfoCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CGprsInfoCtrlDlg)

public:
	CGprsInfoCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGprsInfoCtrlDlg();

// Dialog Data
	enum { IDD = IDD_GPRS_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
