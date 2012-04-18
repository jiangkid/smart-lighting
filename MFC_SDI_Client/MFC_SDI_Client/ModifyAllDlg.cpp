// ModifyAllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ModifyAllDlg.h"


// CModifyAllDlg dialog

IMPLEMENT_DYNAMIC(CModifyAllDlg, CDialog)

CModifyAllDlg::CModifyAllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyAllDlg::IDD, pParent)
{

}

CModifyAllDlg::~CModifyAllDlg()
{
}

void CModifyAllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CModifyAllDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CModifyAllDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CModifyAllDlg message handlers

BOOL CModifyAllDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Tab.InsertItem(0,_T("修改密码"),0);
	m_Tab.InsertItem(1,_T("修改用户名"),1);
	m_Tab.InsertItem(2,_T("修改电话号码"),2);
	m_pModifyPass = new CModify();
	m_pModifyPass->Create(IDD_Modify,&m_Tab);
	CRect m_rect;
	m_Tab.GetClientRect(m_rect);
	m_rect.DeflateRect(0,20,0,0);
	m_pModifyPass->MoveWindow(m_rect);
	m_pModifyPass->ShowWindow(SW_SHOW);

	m_pModifyName = new CModifyName();
	m_pModifyName->Create(IDD_MName,&m_Tab);
	m_pModifyName->MoveWindow(m_rect);
	m_pModifyName->ShowWindow(SW_HIDE);

	m_pMPhone = new CMphoneDlg();
	m_pMPhone->Create(IDD_MPhone,&m_Tab);
	m_pMPhone->MoveWindow(m_rect);
	m_pMPhone->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyAllDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_Tab.GetCurFocus();
	switch(sel)
	{
	case 0:
		m_pModifyPass->ShowWindow(SW_SHOW);
		m_pModifyName->ShowWindow(SW_HIDE);
		m_pMPhone->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pModifyPass->ShowWindow(SW_HIDE);
		m_pModifyName->ShowWindow(SW_SHOW);
		m_pMPhone->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pModifyPass->ShowWindow(SW_HIDE);
		m_pModifyName->ShowWindow(SW_HIDE);
		m_pMPhone->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
	*pResult = 0;
}
