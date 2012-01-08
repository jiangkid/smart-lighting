// WaitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "WaitDlg.h"


// CWaitDlg dialog

IMPLEMENT_DYNAMIC(CWaitDlg, CDialog)

CWaitDlg::CWaitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWaitDlg::IDD, pParent)
	, m_InitStatus(_T(""))
{

}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Sure);
	DDX_Text(pDX, IDC_STATIC_INIT, m_InitStatus);
}


BEGIN_MESSAGE_MAP(CWaitDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CWaitDlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWaitDlg message handlers

BOOL CWaitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_STATIC_INIT,"登入成功，正在初始化。。。。。。。");
	SetDlgItemText(IDC_BUTTON1,"取消");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWaitDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CWaitDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

void CWaitDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
// 	this->~CWaitDlg();
// 	free(this);
	CDialog::PostNcDestroy();
}
