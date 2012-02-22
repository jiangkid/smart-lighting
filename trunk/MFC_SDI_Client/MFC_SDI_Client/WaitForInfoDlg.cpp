// WaitForInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "WaitForInfoDlg.h"


// CWaitForInfoDlg dialog

IMPLEMENT_DYNAMIC(CWaitForInfoDlg, CDialog)

CWaitForInfoDlg::CWaitForInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitForInfoDlg::IDD, pParent)
{

}

CWaitForInfoDlg::~CWaitForInfoDlg()
{
}

void CWaitForInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaitForInfoDlg, CDialog)
END_MESSAGE_MAP()


// CWaitForInfoDlg message handlers

void CWaitForInfoDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	this->~CWaitForInfoDlg();
	free(this);
	CDialog::PostNcDestroy();
}

void CWaitForInfoDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	CDialog::OnCancel();
}

BOOL CWaitForInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	SetDlgItemText(IDC_STATIC_1,_T("正在获取信息，请稍后....."));
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
