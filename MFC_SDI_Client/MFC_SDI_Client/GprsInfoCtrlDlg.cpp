// GprsInfoCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "GprsInfoCtrlDlg.h"


// CGprsInfoCtrlDlg dialog

IMPLEMENT_DYNAMIC(CGprsInfoCtrlDlg, CDialog)

CGprsInfoCtrlDlg::CGprsInfoCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGprsInfoCtrlDlg::IDD, pParent)
{

}

CGprsInfoCtrlDlg::~CGprsInfoCtrlDlg()
{
}

void CGprsInfoCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GINFO_TAB, m_Tab);
}
// CGprsInfoCtrlDlg message handlers
BEGIN_MESSAGE_MAP(CGprsInfoCtrlDlg, CDialog)
END_MESSAGE_MAP()
BOOL CGprsInfoCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("基本信息");
	m_Tab.InsertItem(0,&tcItem);
	tcItem.pszText = _T("开关信息");
	m_Tab.InsertItem(1,&tcItem);
	tcItem.pszText = _T("线缆信息");
	m_Tab.InsertItem(2,&tcItem);
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
