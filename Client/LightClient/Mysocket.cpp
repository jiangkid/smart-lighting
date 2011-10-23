// Mysocket.cpp : implementation file
//

#include "stdafx.h"
#include "LightClient.h"
#include "Mysocket.h"
#include "ConnectDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMysocket
CMysocket::CMysocket(CConnectDlg* pDlg):m_pDlg(pDlg)
{
}
CMysocket::CMysocket()
{
}

CMysocket::~CMysocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMysocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CMysocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMysocket member functions

void CMysocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CAsyncSocket::OnClose(nErrorCode);
}


