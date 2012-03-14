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
	ON_NOTIFY(TCN_SELCHANGE, IDC_GINFO_TAB, &CGprsInfoCtrlDlg::OnTcnSelchangeGinfoTab)
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

	m_pGLocalVIew = new CGPRSLocaInfomation();
	m_pGLocalVIew->Create(IDD_GPRSLocal,&m_Tab);
	CRect m_rect;
	m_Tab.GetClientRect(m_rect);
	m_rect.DeflateRect(0,20,0,0);
	m_pGLocalVIew->MoveWindow(m_rect);
	m_pGLocalVIew->ShowWindow(SW_SHOW);
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CGprsInfoCtrlDlg::OnTcnSelchangeGinfoTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_Tab.GetCurFocus();
	switch(sel)
	{
	case 1 :
		break;
	case 2:
		break;
	case 3:
		break;
	}
	*pResult = 0;
}
