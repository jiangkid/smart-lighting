#pragma once


// CCVSetDlg dialog

class CCVSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CCVSetDlg)

public:
	CCVSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCVSetDlg();

// Dialog Data
	enum { IDD = IDD_CV_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
