#pragma once


// CLightOnOffDlg dialog

class CLightOnOffDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightOnOffDlg)

public:
	CLightOnOffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLightOnOffDlg();

// Dialog Data
	enum { IDD = IDD_LIGHTONOFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel3();
};
