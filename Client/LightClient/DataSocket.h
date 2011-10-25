#if !defined(AFX_DATASOCKET_H__D049425B_ACB0_4A28_BD3C_6C038050FDAF__INCLUDED_)
#define AFX_DATASOCKET_H__D049425B_ACB0_4A28_BD3C_6C038050FDAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CDataSocket command target
class CChatCAsyncSocketDlg;
class CDataSocket : public CAsyncSocket
{
// Attributes
public:
	BOOL SendData(const char* buffer, int length);
	CChatCAsyncSocketDlg * m_pDlg;
	CDataSocket(CChatCAsyncSocketDlg* pDlg);
	virtual ~CDataSocket();
// Operations
private:
	CDataSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDataSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASOCKET_H__D049425B_ACB0_4A28_BD3C_6C038050FDAF__INCLUDED_)
