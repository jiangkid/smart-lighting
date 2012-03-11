// LightCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "LightCtrlDlg.h"
#include "BtnST.h"

// CLightCtrlDlg dialog

IMPLEMENT_DYNAMIC(CLightCtrlDlg, CDialog)

CLightCtrlDlg::CLightCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightCtrlDlg::IDD, pParent)
	, m_GName(_T(""))
	, m_RName(_T(""))
	, m_LName(_T(""))
	, m_LID(_T(""))
{

}

CLightCtrlDlg::~CLightCtrlDlg()
{
}

void CLightCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GName, m_GName);
	DDX_Text(pDX, IDC_RoadName, m_RName);
	DDX_Text(pDX, IDC_LighteName, m_LName);
	DDX_Text(pDX, IDC_LightID, m_LID);
	DDX_Control(pDX, IDC_BTN_MAINStatus, m_MainStatus);
	DDX_Control(pDX, IDC_BTN_ASSISTStatus, m_AssistStatus);
}


BEGIN_MESSAGE_MAP(CLightCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_DON, &CLightCtrlDlg::OnBnClickedBtnDon)
	ON_BN_CLICKED(IDC_BTN_MAIN, &CLightCtrlDlg::OnBnClickedBtnMain)
	ON_BN_CLICKED(IDC_BTN_ASSIST, &CLightCtrlDlg::OnBnClickedBtnAssist)
	ON_BN_CLICKED(IDC_BTN_DOFF, &CLightCtrlDlg::OnBnClickedBtnDoff)
END_MESSAGE_MAP()


// CLightCtrlDlg message handlers

BOOL CLightCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_GName=theApp.m_pLightListView->m_szGName;
	m_RName=theApp.m_pLightListView->m_szRName;
	m_LName=theApp.m_pLightListView->m_szLName;
	m_LID=theApp.m_pLightListView->m_szLID;
	if (strcmp(theApp.m_pLightListView->m_szMainStatus,ON)==0)
	{
		m_MainStatus.SetIcon(IDI_LIGHT_ON);
		GetDlgItem(IDC_BTN_MAIN)->SetWindowText("主灯关"); 
	}
	else
	{
		m_MainStatus.SetIcon(IDI_LIGHT_OFF);
		GetDlgItem(IDC_BTN_MAIN)->SetWindowText("主灯开");
	}
		
	if (strcmp(theApp.m_pLightListView->m_szAssistStatus,ON)==0)
	{
		m_AssistStatus.SetIcon(IDI_LIGHT_ON);
		GetDlgItem(IDC_BTN_ASSIST)-> SetWindowText("辅灯关");
	}
	else
	{
		m_AssistStatus.SetIcon(IDI_LIGHT_OFF);
		GetDlgItem(IDC_BTN_ASSIST)-> SetWindowText("辅灯开");
	}
	UpdateData(FALSE);
	return TRUE; 
	// return TRUE unless you set the focus to a control	// EXCEPTION: OCX Property Pages should return FALSE
}
void CLightCtrlDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialog::OnCancel();
}
void CLightCtrlDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	this->~CLightCtrlDlg();
	free(this);
	CDialog::PostNcDestroy();
}
void CLightCtrlDlg::OnBnClickedBtnDon()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Progress)->SetWindowText("命令已经发出，请等待");
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
	pGetRInfo->m_First[0]=0x2F;
	pGetRInfo->m_First[1]=0x43;
	pGetRInfo->m_First[2]=0x2F;
	pGetRInfo->m_First[3]=0x01;
	memcpy(pGetRInfo->m_ID,m_LID.GetBuffer(),16);
	pGetRInfo->m_OrderType[0]=0x0A;
	pGetRInfo->m_OrderObject[0]=0xA3;
	pGetRInfo->m_ActiveType[0]=0xB1;
	pGetRInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetRInfo);
	Sleep(5000);
	if (theApp.m_DlgDoubleONStatus)
	{
		GetDlgItem(IDC_Progress)->SetWindowText("命令操作成功");
		m_MainStatus.SetIcon(IDI_LIGHT_ON);
		m_AssistStatus.SetIcon(IDI_LIGHT_ON);
		GetDlgItem(IDC_BTN_MAIN)->SetWindowText("主灯关"); 
		GetDlgItem(IDC_BTN_ASSIST)-> SetWindowText("辅灯关");
		theApp.m_DlgDoubleONStatus=false;
	}
	else
	{
		GetDlgItem(IDC_Progress)->SetWindowText("命令操作失败");
		return;
	}
}

void CLightCtrlDlg::OnBnClickedBtnMain()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Progress)->SetWindowText("命令已经发出，请等待");
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
	pGetRInfo->m_First[0]=0x2F;
	pGetRInfo->m_First[1]=0x43;
	pGetRInfo->m_First[2]=0x2F;
	pGetRInfo->m_First[3]=0x01;
	memcpy(pGetRInfo->m_ID,m_LID.GetBuffer(),16);
	pGetRInfo->m_OrderType[0]=0x0A;
	pGetRInfo->m_OrderObject[0]=0xA1;
	CString str;
	GetDlgItemText(IDC_BTN_MAIN,str);
	if (strcmp(str,"主灯开")==0)
	{
		pGetRInfo->m_ActiveType[0]=0xB1;
		pGetRInfo->m_EndBuffer[1]=0xCC;
		SendContrlInfo(&hdr,pGetRInfo);
		Sleep(5000);
		if (theApp.m_DlgMainONStatus)
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作成功");
			m_MainStatus.SetIcon(IDI_LIGHT_ON);
			GetDlgItem(IDC_BTN_MAIN)->SetWindowText("主灯关"); 
			theApp.m_DlgMainONStatus=false;
		}
		else
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作失败");
			return;
		}
	}
	else
	{
		pGetRInfo->m_ActiveType[0]=0xB2;
		pGetRInfo->m_EndBuffer[1]=0xCC;
		SendContrlInfo(&hdr,pGetRInfo);
		Sleep(5000);
		if (theApp.m_DlgMainOFFStatus)
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作成功");
			m_MainStatus.SetIcon(IDI_LIGHT_OFF);
			GetDlgItem(IDC_BTN_MAIN)->SetWindowText("主灯开"); 
			theApp.m_DlgMainOFFStatus=false;
		}
		else
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作失败");
			return;
		}
	}
}

void CLightCtrlDlg::OnBnClickedBtnAssist()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Progress)->SetWindowText("命令已经发出，请等待");
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
	pGetRInfo->m_First[0]=0x2F;
	pGetRInfo->m_First[1]=0x43;
	pGetRInfo->m_First[2]=0x2F;
	pGetRInfo->m_First[3]=0x01;
	memcpy(pGetRInfo->m_ID,m_LID.GetBuffer(),16);
	pGetRInfo->m_OrderType[0]=0x0A;
	pGetRInfo->m_OrderObject[0]=0xA2;
	CString str;
	GetDlgItemText(IDC_BTN_ASSIST,str);
	if (strcmp(str,"辅灯开")==0)
	{
		pGetRInfo->m_ActiveType[0]=0xB1;
		pGetRInfo->m_OrderObject[0]=0xA2;
		pGetRInfo->m_EndBuffer[1]=0xCC;
		SendContrlInfo(&hdr,pGetRInfo);
		Sleep(5000);
		if (theApp.m_DlgAssistONStatus)
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作成功");
			m_AssistStatus.SetIcon(IDI_LIGHT_ON);
			GetDlgItem(IDC_BTN_ASSIST)-> SetWindowText("辅灯关");
			theApp.m_DlgAssistONStatus=false;
		}
		else
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作失败");
			return;
		}
	}
	else
	{
		pGetRInfo->m_ActiveType[0]=0xB2;
		pGetRInfo->m_OrderObject[0]=0xA2;
		pGetRInfo->m_EndBuffer[1]=0xCC;
		SendContrlInfo(&hdr,pGetRInfo);
		Sleep(5000);
		if (theApp.m_DlgAssistOFFStatus)
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作成功");
			m_AssistStatus.SetIcon(IDI_LIGHT_OFF);
			GetDlgItem(IDC_BTN_ASSIST)-> SetWindowText("辅灯开");
			theApp.m_DlgAssistOFFStatus=false;
		}
		else
		{
			GetDlgItem(IDC_Progress)->SetWindowText("命令操作失败");
			return;
		}
	}
		
	

}

void CLightCtrlDlg::OnBnClickedBtnDoff()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_Progress)->SetWindowText("命令已经发出，请等待");
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
	pGetRInfo->m_First[0]=0x2F;
	pGetRInfo->m_First[1]=0x43;
	pGetRInfo->m_First[2]=0x2F;
	pGetRInfo->m_First[3]=0x01;
	memcpy(pGetRInfo->m_ID,m_LID.GetBuffer(),16);
	pGetRInfo->m_OrderType[0]=0x0A;
	pGetRInfo->m_OrderObject[0]=0xA3;
	pGetRInfo->m_ActiveType[0]=0xB2;
	pGetRInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetRInfo);
	Sleep(5000);
	if (theApp.m_DlgDoubleOFFStatus)
	{
		GetDlgItem(IDC_Progress)->SetWindowText("命令操作成功");
		m_MainStatus.SetIcon(IDI_LIGHT_OFF);
		m_AssistStatus.SetIcon(IDI_LIGHT_OFF);
		GetDlgItem(IDC_BTN_MAIN)->SetWindowText("主灯开"); 
		GetDlgItem(IDC_BTN_ASSIST)-> SetWindowText("辅灯开");
		theApp.m_DlgDoubleOFFStatus=false;
	}
	else
	{
		GetDlgItem(IDC_Progress)->SetWindowText("命令操作失败");
		return;
	}
}
