// MapCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MapCtrlDlg.h"


// CMapCtrlDlg dialog

IMPLEMENT_DYNAMIC(CMapCtrlDlg, CDialog)

CMapCtrlDlg::CMapCtrlDlg(CWnd* pParent /*=NULL*/ )
	: CDialog(CMapCtrlDlg::IDD, pParent)
	,id(_T(""))
	,strID(_T(""))
	,strName(_T(""))
	, m_GGGGID(_T(""))
{
}

CMapCtrlDlg::~CMapCtrlDlg()
{
}

void CMapCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_MapList);
}


BEGIN_MESSAGE_MAP(CMapCtrlDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMapCtrlDlg::OnNMDblclkListTerminal)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapCtrlDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMapCtrlDlg message handlers

BOOL CMapCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	theApp.m_pMapCtrlDlg=this;
	m_MapList.SetHeadings("终端箱ID,80;终端箱名字,120;终端箱安装地点,120;责任区,120;安装时间,120;");
	m_MapList.SetGridLines(true);
	SendGMessage();
	return TRUE;  
	 
}
void CMapCtrlDlg::OnNMDblclkListTerminal(NMHDR *pNMHDR, LRESULT *pResult)
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
		theApp.m_where = true;
		strID=m_MapList.GetItemText(pNMItemActivate->iItem,0);
		strName=m_MapList.GetItemText(pNMItemActivate->iItem,1);  
		CRtuSetDlg dlg;
		dlg.DoModal();
	}

	*pResult = 0;
}

void CMapCtrlDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CSetG dlg;
	dlg.DoModal();
}

void CMapCtrlDlg::ShowLocalInfo(GPRSInfo* pGetInfo)
{
	CString m_gprsid=_T("");
	CString m_gprsarea=_T("");
	CString m_gprslocation=_T("");
	CString m_gprsname=_T("");
	CString m_gprsphone=_T("");
	CString m_gprstime=_T("");
	m_GGGGID=_T("");
	for (int i(0);i<2;i++)
	{
		m_gprsid+=pGetInfo->gID[i];
		m_GGGGID+=pGetInfo->gID[i];
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
	SetDlgItemText(IDC_EDIT1,m_gprsid);
	SetDlgItemText(IDC_EDIT5,m_gprslocation);
	SetDlgItemText(IDC_EDIT2,m_gprsname);
	SetDlgItemText(IDC_EDIT4,m_gprsphone);
	SetDlgItemText(IDC_EDIT6,m_gprstime);
	SetDlgItemText(IDC_EDIT3,m_gprsarea);
}

void CMapCtrlDlg::ShowTerminalInfo(UINT nItem)
{
	m_MapList.DeleteAllItems();
	for (int i(0);i<nItem;i++)
	{
		CString strTerminalName=_T("");
		CString strTerminalID=_T("");
		CString strTerminalTime=_T("");
		CString strTerminalLocation=_T("");
		CString strTerminalArea=_T("");
		for (int m(0);m<20;m++)
		{
			strTerminalName+=theApp.m_TerminalInfoMap[i].TerminalName[m];
			strTerminalArea+=theApp.m_TerminalInfoMap[i].TerminalArea[m];
			strTerminalLocation+=theApp.m_TerminalInfoMap[i].TerminalLocation[m];
		}
		for (int n(0);n<4;n++)
		{
			strTerminalID+=theApp.m_TerminalInfoMap[i].TID[n];
		}
		for (int j(0);j<17;j++)
		{
			strTerminalTime+=theApp.m_TerminalInfoMap[i].TerminalTime[j];
		}
		m_MapList.InsertItem(i,strTerminalID,strTerminalName,strTerminalLocation,strTerminalArea,strTerminalTime);
	}
}

void CMapCtrlDlg::SendGMessage(void)
{
	CString str;
	str+='G';
	str+='3';
	str+=theApp.strGffID;
	str+='#';
	send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
	str.ReleaseBuffer();
}
void CMapCtrlDlg::SendTMessage(void)
{
	CString str;
	str+='G';
	str+='4';
	str+=theApp.strGffID;
	str+='#';
	send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
	str.ReleaseBuffer();
}