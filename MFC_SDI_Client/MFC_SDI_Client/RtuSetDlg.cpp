// RtuSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "RtuSetDlg.h"
 



// CRtuSetDlg dialog

IMPLEMENT_DYNAMIC(CRtuSetDlg, CDialog)

CRtuSetDlg::CRtuSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRtuSetDlg::IDD, pParent)
{
	  
}

CRtuSetDlg::~CRtuSetDlg()
{
}

void CRtuSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_RtuTab);
	DDX_Text(pDX, IDC_EDIT_BOXID, m_strTerminalID);
	DDX_Text(pDX, IDC_EDIT_BOXNAME, m_strTerminalName);
}

BEGIN_MESSAGE_MAP(CRtuSetDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CRtuSetDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CRtuSetDlg message handlers
BOOL CRtuSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ShowTerminalBoxInfo();

	m_RtuTab.InsertItem(0,_T("开关灯"),0);
	m_RtuTab.InsertItem(1,_T("经纬度设置"),1);
	m_RtuTab.InsertItem(2,_T("策略管理"),2);

	m_pLightDlg=new CLightOnOffDlg();
	m_pLightDlg->Create(IDD_LIGHTONOFF,&m_RtuTab);
	m_RtuTab.GetClientRect(&m_rect);
	m_rect.DeflateRect(0,20,0,0);
	m_pLightDlg->MoveWindow(m_rect);
	m_pLightDlg->ShowWindow(SW_SHOW);

	m_pJWDlg=new CJWSetDlg();
	m_pJWDlg->Create(IDD_JWDU_SET,&m_RtuTab);
	m_pJWDlg->MoveWindow(m_rect);
	m_pJWDlg->ShowWindow(SW_HIDE);
	
	m_pDecision=new CDecisionDlg();
	m_pDecision->Create(IDD_DECISION,&m_RtuTab);
	m_pDecision->MoveWindow(m_rect);
	m_pDecision->ShowWindow(SW_HIDE);
	 
	
	return true;
}

void CRtuSetDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sel=m_RtuTab.GetCurFocus();
	switch(sel)
	{

	   case 0:
			m_pLightDlg->ShowWindow(SW_SHOW);
			m_pJWDlg->ShowWindow(SW_HIDE);
			m_pDecision->ShowWindow(SW_HIDE); 
			break;
		case 1: 
			m_pJWDlg->ShowWindow(SW_SHOW);
			m_pLightDlg->ShowWindow(SW_HIDE);
			m_pDecision->ShowWindow(SW_HIDE);
			break;
		case 2:
			m_pDecision->ShowWindow(SW_SHOW);
			m_pJWDlg->ShowWindow(SW_HIDE);
			m_pLightDlg->ShowWindow(SW_HIDE);
			break;
	}
	*pResult = 0;
}
 
void CRtuSetDlg::ShowTerminalBoxInfo() 
{
	 m_pGPRS=theApp.m_pLocalInfoDlg;
	 SetDlgItemText(IDC_EDIT_BOXID,m_pGPRS->strID);
	 SetDlgItemText(IDC_EDIT_BOXNAME,m_pGPRS->strName);  
}




 

 