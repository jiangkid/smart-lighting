#pragma once
#include "CWebBrowser.h"
#include "WebPage.h"

// CMapViewDlg dialog

class CMapViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CMapViewDlg)

public:
	CMapViewDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMapViewDlg();

// Dialog Data
	enum { IDD = IDD_MAP_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	CWebBrowser m_Browser;
	CString m_lat;
	CString m_lng;
	void ShowMessage(void);
	afx_msg void OnBnClickedButton2();
};
