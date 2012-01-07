#include "StdAfx.h"
#include "MFC_SDI_Client.h"
#include "WaitApp.h"
#include "WaitDlg.h"


IMPLEMENT_DYNCREATE(CWaitApp, CWinThread)

CWaitApp::CWaitApp(void)
{
}

CWaitApp::~CWaitApp(void)
{
}

int CWaitApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinThread::ExitInstance();
}

BOOL CWaitApp::InitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
// 	CWaitDlg *pTestDlg = new CWaitDlg();
// 	pTestDlg->Create(IDD_DIALOG1);
// 	pTestDlg->ShowWindow(SW_SHOW);
// 
// 	pTestDlg->RunModalLoop();
// 	if (theApp.m_InitTrue)
// 	{
// 		pTestDlg->m_Sure.SetWindowText("È·¶¨");
// 	}
	dlg.DoModal();
	return 0;
	//return CWinThread::InitInstance();
}

BEGIN_MESSAGE_MAP(CWaitApp, CWinThread)
END_MESSAGE_MAP()


// CUIThreadApp message handlers