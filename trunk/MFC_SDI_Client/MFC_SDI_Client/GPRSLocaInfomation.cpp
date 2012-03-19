// GPRSLocaInfomation.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "GPRSLocaInfomation.h"


// CGPRSLocaInfomation dialog

IMPLEMENT_DYNAMIC(CGPRSLocaInfomation, CDialog)

CGPRSLocaInfomation::CGPRSLocaInfomation(CWnd* pParent /*=NULL*/)
	: CDialog(CGPRSLocaInfomation::IDD, pParent)
	, m_gprsid(_T(""))
	, m_gprsname(_T(""))
	, m_gprsphone(_T(""))
	, m_gprsarea(_T(""))
	, m_gprslocation(_T(""))
	, m_gprstime(_T(""))
{

}

CGPRSLocaInfomation::~CGPRSLocaInfomation()
{
}

void CGPRSLocaInfomation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GID, m_gprsid);
	DDX_Text(pDX, IDC_EDIT_GNAME, m_gprsname);
	DDX_Text(pDX, IDC_EDIT_GPHONE, m_gprsphone);
	DDX_Text(pDX, IDC_EDIT_GAREA, m_gprsarea);
	DDX_Text(pDX, IDC_EDIT_GLOCATION, m_gprslocation);
	DDX_Text(pDX, IDC_EDIT_GTIME, m_gprstime);
	DDX_Control(pDX, IDC_LIST_TERMINAL, m_List_Terminal);
}


BEGIN_MESSAGE_MAP(CGPRSLocaInfomation, CDialog)
END_MESSAGE_MAP()


// CGPRSLocaInfomation message handlers

void CGPRSLocaInfomation::ShowLocationInfo(GPRSInfo* pGetInfo)
{
	m_gprsid=_T("");
	m_gprsarea=_T("");
	m_gprslocation=_T("");
	m_gprsname=_T("");
	m_gprsphone=_T("");
	m_gprstime=_T("");
	for (int i(0);i<2;i++)
	{
		m_gprsid+=pGetInfo->gID[i];
	}
	for (int n(0);n<20;n++)
	{
		m_gprsarea+=pGetInfo->gArea[n];
		m_gprslocation+=pGetInfo->gLocation[n];
		m_gprsname+=pGetInfo->gName[n];
	}
	for (int u(0);u<11;u++)
	{
		m_gprsphone+=pGetInfo->gTelephone[u];
	}
	for (int o(0);o<17;o++)
	{
		m_gprstime+=pGetInfo->gTime[o];
	}
	SetDlgItemText(IDC_EDIT_GID,m_gprsid);
	SetDlgItemText(IDC_EDIT_GLOCATION,m_gprslocation);
	SetDlgItemText(IDC_EDIT_GNAME,m_gprsname);
	SetDlgItemText(IDC_EDIT_GPHONE,m_gprsphone);
	SetDlgItemText(IDC_EDIT_GTIME,m_gprstime);
	SetDlgItemText(IDC_EDIT_GAREA,m_gprsarea);

	//UpdateData(true);
}

BOOL CGPRSLocaInfomation::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pLocalInfoDlg=this;
	m_List_Terminal.SetHeadings("终端箱ID,80;终端箱名字,120;终端箱安装地点,120;责任区,120;安装时间,120;");
	m_List_Terminal.SetGridLines(TRUE);
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}