#pragma once
#include "PrintFrame.h"

// CControlWaringDlg dialog

class CControlWaringDlg : public CDialog
{
	DECLARE_DYNAMIC(CControlWaringDlg)
	void GetWindowGraph();
public:
	CControlWaringDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlWaringDlg();


	CPrintFrame*	m_pPrintFrame;
	CBitmap			m_bitmapPrint;


// Dialog Data
	enum { IDD = IDD_ControlWarning };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnPrintview();
	afx_msg LRESULT OnPrintWindow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
};