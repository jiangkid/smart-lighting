#if !defined(AFX_MYSOCKET_H__1ECE412B_54B3_4B5F_9DD0_781E353B8714__INCLUDED_)
#define AFX_MYSOCKET_H__1ECE412B_54B3_4B5F_9DD0_781E353B8714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mysocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMysocket command target
class CConnectDlg;
class CMysocket : public CAsyncSocket
{
// Attributes
public:

// Operations
public:
	CMysocket();
	CConnectDlg* m_pDlg;
	CMysocket(CConnectDlg* pDlg);
	virtual ~CMysocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMysocket)
	public:
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMysocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__1ECE412B_54B3_4B5F_9DD0_781E353B8714__INCLUDED_)
