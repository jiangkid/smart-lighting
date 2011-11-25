// UserLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Client_Light.h"
#include "UserLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg dialog


CUserLoginDlg::CUserLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUserLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserLoginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CUserLoginDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserLoginDlg message handlers
