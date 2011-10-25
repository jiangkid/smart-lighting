// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ChatCAsyncSocket.h"
#include "DataSocket.h"
#include "ChatCAsyncSocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataSocket

CDataSocket::CDataSocket(CChatCAsyncSocketDlg* pDlg):m_pDlg(pDlg)
{
}

CDataSocket::CDataSocket()
{
}

CDataSocket::~CDataSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDataSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CDataSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDataSocket member functions

void CDataSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(nErrorCode!=0)
	{
		m_pDlg->OnSocketClose(this);
		return;
	}
	m_pDlg->OnSocketReceive(this);
	CAsyncSocket::OnReceive(nErrorCode);
}

void CDataSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDlg->OnSocketClose(this);
	CAsyncSocket::OnClose(nErrorCode);
}

BOOL CDataSocket::SendData(const char* buffer, int length)
{
	int ret,i;
	i=0;
	while(length>0)
	{
		ret=Send(&(buffer[i]),length,0);
		if(ret==0)
			break;
		else if(ret == SOCKET_ERROR)
			return FALSE;
		length-=ret;
		i+=ret;
	}
	return TRUE;
}

void CDataSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pDlg->OnClientConnect(nErrorCode);
	CAsyncSocket::OnConnect(nErrorCode);
}
