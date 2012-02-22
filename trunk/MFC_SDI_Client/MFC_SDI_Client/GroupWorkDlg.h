#pragma once


// CGroupWorkDlg dialog

class CGroupWorkDlg : public CDialog
{
	DECLARE_DYNAMIC(CGroupWorkDlg)

public:
	CGroupWorkDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGroupWorkDlg();

// Dialog Data
	enum { IDD = IDD_GroupWork };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_GroupNum;
	afx_msg void OnBnClickedGroupok();
};
