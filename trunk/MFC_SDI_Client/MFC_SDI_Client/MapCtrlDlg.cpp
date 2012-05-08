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
	, m_later(false)
	, m_True(false)
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
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMapCtrlDlg message handlers

BOOL CMapCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	theApp.m_pMapCtrlDlg=this;
	m_MapList.SetHeadings("�ն���ID,80;�ն�������,120;�ն��䰲װ�ص�,120;������,120;��װʱ��,120;");
	//m_MapList.SetHeadings("�ն���ID,80;�ն�������,120;�ն��䰲װ�ص�,120;������,120;��װʱ��,120;A���ѹֵ,100;B���ѹֵ,100;C���ѹֵ,100;");
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
		char c[5] = {'G',0x36,0x00,0x00,'#'};
		memcpy(&c[2],strID.GetBuffer(4),2);
		strID.ReleaseBuffer(4);
		send(theApp.m_ConnectSock,c,5,0);
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
	m_True = true;
	CString str;
	str+='G';
	str+='3';
	str+=theApp.strGffID;
	str+='#';
	send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
	str.ReleaseBuffer();
}
void CMapCtrlDlg::SendTMessage(char* c)
{
	ConTrlInfo* pGteInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGteInfo,sizeof(ConTrlInfo));
	switch (c[0])
	{
	case 0x00:
		{
			pGteInfo->m_First[0]=0x2F;
			pGteInfo->m_First[1]=0x43;
			pGteInfo->m_First[2]=0x2F;
			pGteInfo->m_First[3]=0x06;
			pGteInfo->m_ID[0]=theApp.GID[2];
			pGteInfo->m_ID[1]=theApp.GID[3];
			pGteInfo->m_ID[2]=0x30;
			pGteInfo->m_ID[3]=0x31;
			pGteInfo->m_OrderType[0]=0x1A;
			pGteInfo->m_OrderObject[0]=0x34;
			pGteInfo->m_ActiveType[0]=0xBD;
			pGteInfo->m_CheckData[0]=0xA0;
			pGteInfo->m_EndBuffer[1]=0xCC;
			SendContrlInfo(&hdr,pGteInfo);
			m_later = false;
			SetTimer(1,5000,0);
		}
		break;
	case 0x01:
		switch(c[1])
		{
		case 0x00:
			{
				pGteInfo->m_First[0]=0x2F;
				pGteInfo->m_First[1]=0x43;
				pGteInfo->m_First[2]=0x2F;
				pGteInfo->m_First[3]=0x06;
				pGteInfo->m_ID[0]=theApp.GID[2];
				pGteInfo->m_ID[1]=theApp.GID[3];
				pGteInfo->m_ID[2]=0x30;
				pGteInfo->m_ID[3]=0x32;
				pGteInfo->m_OrderType[0]=0x1A;
				pGteInfo->m_OrderObject[0]=0x34;
				pGteInfo->m_ActiveType[0]=0xBD;
				pGteInfo->m_CheckData[0]=0xA0;
				pGteInfo->m_EndBuffer[1]=0xCC;
				SendContrlInfo(&hdr,pGteInfo);
			}
			break;
		case 0x01:
			switch(c[2])
			{
			case 0x00:
				{
					pGteInfo->m_First[0]=0x2F;
					pGteInfo->m_First[1]=0x43;
					pGteInfo->m_First[2]=0x2F;
					pGteInfo->m_First[3]=0x06;
					pGteInfo->m_ID[0]=theApp.GID[2];
					pGteInfo->m_ID[1]=theApp.GID[3];
					pGteInfo->m_ID[2]=0x30;
					pGteInfo->m_ID[3]=0x33;
					pGteInfo->m_OrderType[0]=0x1A;
					pGteInfo->m_OrderObject[0]=0x34;
					pGteInfo->m_ActiveType[0]=0xBD;
					pGteInfo->m_CheckData[0]=0xA0;
					pGteInfo->m_EndBuffer[1]=0xCC;
					SendContrlInfo(&hdr,pGteInfo);
				}
				break;
			case 0x01:
				switch (c[3])
				{
				case 0x00:
					{
						pGteInfo->m_First[0]=0x2F;
						pGteInfo->m_First[1]=0x43;
						pGteInfo->m_First[2]=0x2F;
						pGteInfo->m_First[3]=0x06;
						pGteInfo->m_ID[0]=theApp.GID[2];
						pGteInfo->m_ID[1]=theApp.GID[3];
						pGteInfo->m_ID[2]=0x30;
						pGteInfo->m_ID[3]=0x34;
						pGteInfo->m_OrderType[0]=0x1A;
						pGteInfo->m_OrderObject[0]=0x34;
						pGteInfo->m_ActiveType[0]=0xBD;
						pGteInfo->m_CheckData[0]=0xA0;
						pGteInfo->m_EndBuffer[1]=0xCC;
						SendContrlInfo(&hdr,pGteInfo);
					}
					break;
				case 0x01:
					ZeroMemory(theApp.sendTerminal,4);
					CString str;
					str+='G';
					str+='4';
					str+=theApp.strGffID;
					str+='#';
					send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
					send(theApp.m_ConnectSock,theApp.GID,5,0);
					break;
				}
				break;
			}
			break;
		}
		break;
	}

}
void CMapCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==1)
	{
		m_later = true;
		CString str;
		str+='G';
		str+='4';
		str+=theApp.strGffID;
		str+='#';
		send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
		str.ReleaseBuffer();
		KillTimer(1);
	}

	CDialog::OnTimer(nIDEvent);
}
