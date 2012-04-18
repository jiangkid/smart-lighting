#pragma once


// CTerminalCtrlDlg dialog

class CTerminalCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CTerminalCtrlDlg)

public:
	CTerminalCtrlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTerminalCtrlDlg();
	
	int nRet;

// Dialog Data
	enum { IDD = IDD_TerminalDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void RoadInfoToView(int nCount);
};
