#if !defined(AFX_CLIENTSOCKET_H__C0121B5D_7195_4D0F_BEAA_E6A96B24020B__INCLUDED_)
#define AFX_CLIENTSOCKET_H__C0121B5D_7195_4D0F_BEAA_E6A96B24020B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target
class CMFC_Client_LightView;
class CClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket(CMFC_Client_LightView* view2);
	virtual ~CClientSocket();

// Overrides
public:
	CMFC_Client_LightView* view;
	bool m_bClose;
	bool m_bInit;
	CSocketFile* m_sfSocketFile;
	CArchive* m_aSessionOut;
	CArchive* m_aSessionIn;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__C0121B5D_7195_4D0F_BEAA_E6A96B24020B__INCLUDED_)
