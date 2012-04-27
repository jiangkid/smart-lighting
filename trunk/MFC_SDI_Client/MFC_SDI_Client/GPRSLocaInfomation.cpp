// GPRSLocaInfomation.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "GPRSLocaInfomation.h"
#include "RtuSetDlg.h"


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
	, m_TerminalCount(0)
	, strID(_T(""))
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
	ON_BN_CLICKED(IDC_BTN_GMODIFY, &CGPRSLocaInfomation::OnBnClickedBtnGmodify)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_TERMINAL, &CGPRSLocaInfomation::OnNMDblclkListTerminal)
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
TerminalInfo* CGPRSLocaInfomation::GetSelTerminalInfo(void)
{
	POSITION pos_s;
	CString str1=_T("");
	pos_s=m_List_Terminal.GetFirstSelectedItemPosition();
	if(!pos_s)
	{
		return NULL;
	}
	int item=m_List_Terminal.GetNextSelectedItem(pos_s);
	str1=m_List_Terminal.GetItemText(item,0);
	for(int i(0);i<m_TerminalCount;i++)
	{
		CString str2=_T("");
		for (int m(0);m<4;m++)
		{
			str2+=theApp.m_TerminalInfo[i].TID[m];
		}
		if (strcmp(str1,str2)==0)
		{
			return &theApp.m_TerminalInfo[i];
		}		
	}
	return NULL;
}

void CGPRSLocaInfomation::ShowTerminalInfo(int nTcount)
{
	m_List_Terminal.DeleteAllItems();
	m_TerminalCount=nTcount;
	for (int i(0);i<nTcount;i++)
	{
		CString strTerminalName=_T("");
		CString strTerminalID=_T("");
		CString strTerminalTime=_T("");
		CString strTerminalLocation=_T("");
		CString strTerminalArea=_T("");
		for (int m(0);m<20;m++)
		{
			strTerminalName+=theApp.m_TerminalInfo[i].TerminalName[m];
			strTerminalArea+=theApp.m_TerminalInfo[i].TerminalArea[m];
			strTerminalLocation+=theApp.m_TerminalInfo[i].TerminalLocation[m];
		}
		for (int n(0);n<4;n++)
		{
			strTerminalID+=theApp.m_TerminalInfo[i].TID[n];
		}
		for (int j(0);j<17;j++)
		{
			strTerminalTime+=theApp.m_TerminalInfo[i].TerminalTime[j];
		}
		m_List_Terminal.InsertItem(i,strTerminalID,strTerminalName,strTerminalLocation,strTerminalArea,strTerminalTime);
	}
}

void CGPRSLocaInfomation::OnBnClickedBtnGmodify()
{
	// TODO: Add your control notification handler code here
	CSetG dlg;
	dlg.DoModal();
}

void CGPRSLocaInfomation::OnNMDblclkListTerminal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	strID=_T("");
	strName=_T("");
	if(pNMItemActivate->iItem==-1)
	{
		return;
	}
	else
	{	
		strID=m_List_Terminal.GetItemText(pNMItemActivate->iItem,0);
		strName=m_List_Terminal.GetItemText(pNMItemActivate->iItem,1);  
		CRtuSetDlg dlg;
		dlg.DoModal();
	}

	*pResult = 0;
}

 
