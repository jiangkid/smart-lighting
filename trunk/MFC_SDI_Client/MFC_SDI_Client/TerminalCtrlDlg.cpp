// TerminalCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "TerminalCtrlDlg.h"


// CTerminalCtrlDlg dialog

IMPLEMENT_DYNAMIC(CTerminalCtrlDlg, CDialog)

CTerminalCtrlDlg::CTerminalCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTerminalCtrlDlg::IDD, pParent)
	, nRet(0)
{

}

CTerminalCtrlDlg::~CTerminalCtrlDlg()
{
}

void CTerminalCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTerminalCtrlDlg, CDialog)
END_MESSAGE_MAP()


// CTerminalCtrlDlg message handlers

BOOL CTerminalCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pTerminalDlg=this;
	char c[6];
	ZeroMemory(c,6);
	c[0]='R';
	c[5]='#';
	memcpy(c+1,theApp.m_pLocalInfoDlg->strID.GetBuffer(),4);
	send(theApp.m_ConnectSock,c,6,0);
	theApp.m_pLocalInfoDlg->strID.ReleaseBuffer();
	Sleep(500);

	RoadInfoToView(theApp.nRCount);
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTerminalCtrlDlg::RoadInfoToView(int nCount)
{
	for (int i=0;i<nCount;i++)
	{
		CString strTName=_T("");
		CString strRName=_T("");
		CString strCurrent1=_T("");
		CString strCurrent2=_T("");
		CString strCurrent3=_T("");
		CString strRID  =_T("");
		strCurrent1.Format("%0.3fA",(float)theApp.m_RoadListInfo[i]->nCurrent1/1000);
		strCurrent2.Format("%0.3fA",(float)theApp.m_RoadListInfo[i]->nCurrent2/1000);
		strCurrent3.Format("%0.3fA",(float)theApp.m_RoadListInfo[i]->nCurrent3/1000);
		for (int n=0;n<20;n++)
		{
			strRName+=theApp.m_RoadListInfo[i]->m_RoadName[n];
		}
		for (int m=0;m<20;m++)
		{
			strTName+=theApp.m_RoadListInfo[i]->m_TernimalName[m];
		}
		for(int k=0;k<6;k++)
		{
			strRID+=theApp.m_RoadListInfo[i]->m_RoadID[k];
		}
		free(theApp.m_RoadListInfo[i]);
	}
}
