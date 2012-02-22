#pragma once


// CWaitForInfoDlg dialog

class CWaitForInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CWaitForInfoDlg)

public:
	CWaitForInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWaitForInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
};
