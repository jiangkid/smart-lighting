#if !defined(AFX_LISTENSOCKET_H__CEA7B25B_50C1_48C7_8D77_AFD6F1CF455B__INCLUDED_)
#define AFX_LISTENSOCKET_H__CEA7B25B_50C1_48C7_8D77_AFD6F1CF455B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListenSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CListenSocket command target
class CChatCAsyncSocketDlg;
class CListenSocket : public CAsyncSocket
{
// Attributes
public:
	CListenSocket(CChatCAsyncSocketDlg *pDlg);
	CChatCAsyncSocketDlg *m_pDlg;
	virtual ~CListenSocket();
	
// Operations
private:
	CListenSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListenSocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CListenSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTENSOCKET_H__CEA7B25B_50C1_48C7_8D77_AFD6F1CF455B__INCLUDED_)
