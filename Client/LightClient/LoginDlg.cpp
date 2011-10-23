// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LightClient.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	//}}AFX_DATA_INIT
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_EDIT2, m_Password);
	DDX_Control(pDX, IDC_EDIT1, m_Name);
	DDX_Control(pDX, IDC_CHECK1, m_Remember);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_BN_CLICKED(IDC_LOGIN, OnLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

void CLoginDlg::OnLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDialog::OnOK();
}
