#if !defined(AFX_USERLOGINDLG_H__1F7FFB77_F194_4D02_9438_11BA37F2A46F__INCLUDED_)
#define AFX_USERLOGINDLG_H__1F7FFB77_F194_4D02_9438_11BA37F2A46F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog

class CUserLoginDlg : public CDialog
{
// Construction
public:
	CUserLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUserLoginDlg)
	enum { IDD = IDD_LOGIN_USER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserLoginDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERLOGINDLG_H__1F7FFB77_F194_4D02_9438_11BA37F2A46F__INCLUDED_)
