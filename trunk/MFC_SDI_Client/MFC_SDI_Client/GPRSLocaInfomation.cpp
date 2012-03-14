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
}


BEGIN_MESSAGE_MAP(CGPRSLocaInfomation, CDialog)
END_MESSAGE_MAP()


// CGPRSLocaInfomation message handlers

void CGPRSLocaInfomation::ShowLocationInfo(GPRSInfo* pGetInfo)
{
	m_gprsid=pGetInfo->gID;
	m_gprsarea=pGetInfo->gArea;
	m_gprslocation=pGetInfo->gLocation;
	m_gprsname=pGetInfo->gName;
	m_gprsphone=pGetInfo->gTelephone;
	m_gprstime=pGetInfo->gTime;
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
	
	// TODO:  Add extra initialization here
	theApp.m_pLocalInfoDlg=this;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
