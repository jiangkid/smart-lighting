#if !defined(AFX_ADDMINLOGINDLG_H__0BB986A1_BA3F_4EA6_A6D7_6F635D15ABB7__INCLUDED_)
#define AFX_ADDMINLOGINDLG_H__0BB986A1_BA3F_4EA6_A6D7_6F635D15ABB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddminLoginDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddminLoginDlg dialog

class CAddminLoginDlg : public CDialog
{
// Construction
public:
	CAddminLoginDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddminLoginDlg)
	enum { IDD = IDD_LOGIN_ADDMIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddminLoginDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddminLoginDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDMINLOGINDLG_H__0BB986A1_BA3F_4EA6_A6D7_6F635D15ABB7__INCLUDED_)
