// DecisionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "DecisionDlg.h"


// CDecisionDlg dialog

IMPLEMENT_DYNAMIC(CDecisionDlg, CDialog)

CDecisionDlg::CDecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDecisionDlg::IDD, pParent)
{

}

CDecisionDlg::~CDecisionDlg()
{
}

void CDecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CDecisionDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CDecisionDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CDecisionDlg message handlers

BOOL CDecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Tab.InsertItem(0,_T("日策略"),0);
	m_Tab.InsertItem(1,_T("周策略"),1);
	m_Tab.InsertItem(2,_T("节假日策略"),2);
	m_Tab.InsertItem(3,_T("特殊策略"),3);
	m_pDayDlg = new CDayDecisionDlg();
	m_pDayDlg->Create(IDD_Day_S,&m_Tab);
	CRect m_rect;
	m_Tab.GetClientRect(m_rect);
	m_rect.DeflateRect(0,20,0,0);
	m_pDayDlg->MoveWindow(m_rect);
	m_pDayDlg->ShowWindow(SW_SHOW);

	m_pWeekDlg = new CWeekDecisionDlg();
	m_pWeekDlg->Create(IDD_Week_S,&m_Tab);
	m_pWeekDlg->MoveWindow(m_rect);
	m_pWeekDlg->ShowWindow(SW_HIDE);

	m_pJieDlg = new CJieDecisionDlg();
	m_pJieDlg->Create(IDD_Jie_S,&m_Tab);
	m_pJieDlg->MoveWindow(m_rect);
	m_pJieDlg->ShowWindow(SW_HIDE);

	m_pSpacialDlg = new CSpacialDecisionDlg();
	m_pSpacialDlg->Create(IDD_Spcial_S,&m_Tab);
	m_pSpacialDlg->MoveWindow(m_rect);
	m_pSpacialDlg->ShowWindow(SW_HIDE);
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDecisionDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_Tab.GetCurFocus();
	switch(sel)
	{
	case 0:
		m_pDayDlg->ShowWindow(SW_SHOW);
		m_pWeekDlg->ShowWindow(SW_HIDE);
		m_pJieDlg->ShowWindow(SW_HIDE);
		m_pSpacialDlg->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pDayDlg->ShowWindow(SW_HIDE);
		m_pWeekDlg->ShowWindow(SW_SHOW);
		m_pJieDlg->ShowWindow(SW_HIDE);
		m_pSpacialDlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pDayDlg->ShowWindow(SW_HIDE);
		m_pWeekDlg->ShowWindow(SW_HIDE);
		m_pJieDlg->ShowWindow(SW_SHOW);
		m_pSpacialDlg->ShowWindow(SW_HIDE);
		break;
	case 3:
		m_pDayDlg->ShowWindow(SW_HIDE);
		m_pWeekDlg->ShowWindow(SW_HIDE);
		m_pJieDlg->ShowWindow(SW_HIDE);
		m_pSpacialDlg->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
