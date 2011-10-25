// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatCAsyncSocket.h"
#include "ListenSocket.h"
#include "ChatCAsyncSocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket(CChatCAsyncSocketDlg *pDlg):m_pDlg(pDlg)
{
}

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDlg->OnServerAccept();
	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CAsyncSocket::OnClose(nErrorCode);
}
