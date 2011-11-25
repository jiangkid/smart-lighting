// AddminLoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Client_Light.h"
#include "AddminLoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddminLoginDlg dialog


CAddminLoginDlg::CAddminLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddminLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddminLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddminLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddminLoginDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddminLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CAddminLoginDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddminLoginDlg message handlers
