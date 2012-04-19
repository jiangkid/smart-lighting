// DoRWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "DoRWorkDlg.h"


// CDoRWorkDlg dialog

IMPLEMENT_DYNAMIC(CDoRWorkDlg, CDialog)

CDoRWorkDlg::CDoRWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoRWorkDlg::IDD, pParent)
 	, m_RName(_T(""))
 	, RID(_T(""))
	, m_status(_T(""))
	, m_szLID1(_T(""))
{

}

CDoRWorkDlg::~CDoRWorkDlg()
{
}

void CDoRWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
 	DDX_Text(pDX, IDC_RName, m_RName);
 	DDX_Text(pDX, IDC_RID, RID);
	DDX_Text(pDX, IDC_RName2, m_status);
}


BEGIN_MESSAGE_MAP(CDoRWorkDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_Close, &CDoRWorkDlg::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_Open, &CDoRWorkDlg::OnBnClickedBtnAll11)
END_MESSAGE_MAP()


// CDoRWorkDlg message handlers

BOOL CDoRWorkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pDoRoadDld=this;
	// TODO:  Add extra initialization here
 	m_RName = theApp.m_pRoadView->m_szRName;
 	RID = theApp.m_pRoadView->m_szLID;
	m_status = theApp.m_pRoadView->m_szGName;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDoRWorkDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//DestroyWindow();
	CDialog::OnCancel();
}
void CDoRWorkDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//this->~CDoRWorkDlg();
	//free(this);
	CDialog::PostNcDestroy();
}
void CDoRWorkDlg::OnBnClickedBtnAll()
{
	ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
	pGetInfo->m_First[0]=0x2F;
	pGetInfo->m_First[1]=0x43;
	pGetInfo->m_First[2]=0x2F;
	pGetInfo->m_First[3]=0x01;
	
	memcpy(pGetInfo->m_ID,RID.GetBuffer(),4);
	pGetInfo->m_OrderType[0]=0x1A;
	pGetInfo->m_OrderObject[0]=0x32;
	pGetInfo->m_ActiveType[0]=0x12;
	RID.ReleaseBuffer();
	CString str=_T("");
	char c[6];
	ZeroMemory(c,6);
	memcpy(c,RID.GetBuffer(),6);
	str+=c[4];
	str+=c[5];
	if (strcmp(str,"01")==0)
	{
		pGetInfo->m_CheckData[3]=0x01;
	}
	if (strcmp(str,"02")==0)
	{
		pGetInfo->m_CheckData[3]=0x02;
	}
	if (strcmp(str,"03")==0)
	{
		pGetInfo->m_CheckData[3]=0x03;
	}
	if (strcmp(str,"04")==0)
	{
		pGetInfo->m_CheckData[3]=0x04;
	}
	if (strcmp(str,"05")==0)
	{
		pGetInfo->m_CheckData[3]=0x05;
	}
	if (strcmp(str,"06")==0)
	{
		pGetInfo->m_CheckData[3]=0x06;
	}
	if (strcmp(str,"07")==0)
	{
		pGetInfo->m_CheckData[3]=0x07;
	}
	if (strcmp(str,"08")==0)
	{
		pGetInfo->m_CheckData[3]=0x08;
	}
	pGetInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo);
	Sleep(500);
	m_status="¹Ø";
	UpdateData(false);
}
void CDoRWorkDlg::OnBnClickedBtnAll11()
{
	ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
	pGetInfo->m_First[0]=0x2F;
	pGetInfo->m_First[1]=0x43;
	pGetInfo->m_First[2]=0x2F;
	pGetInfo->m_First[3]=0x01;

	memcpy(pGetInfo->m_ID,RID.GetBuffer(),4);
	pGetInfo->m_OrderType[0]=0x1A;
	pGetInfo->m_OrderObject[0]=0x32;
	pGetInfo->m_ActiveType[0]=0x13;
	RID.ReleaseBuffer();
	CString str=_T("");
	char c[6];
	ZeroMemory(c,6);
	memcpy(c,RID.GetBuffer(),6);
	str+=c[4];
	str+=c[5];
	if (strcmp(str,"01")==0)
	{
		pGetInfo->m_CheckData[3]=0x01;
	}
	if (strcmp(str,"02")==0)
	{
		pGetInfo->m_CheckData[3]=0x02;
	}
	if (strcmp(str,"03")==0)
	{
		pGetInfo->m_CheckData[3]=0x03;
	}
	if (strcmp(str,"04")==0)
	{
		pGetInfo->m_CheckData[3]=0x04;
	}
	if (strcmp(str,"05")==0)
	{
		pGetInfo->m_CheckData[3]=0x05;
	}
	if (strcmp(str,"06")==0)
	{
		pGetInfo->m_CheckData[3]=0x06;
	}
	if (strcmp(str,"07")==0)
	{
		pGetInfo->m_CheckData[3]=0x07;
	}
	if (strcmp(str,"08")==0)
	{
		pGetInfo->m_CheckData[3]=0x08;
	}

	pGetInfo->m_EndBuffer[1]=0xCC;
	SendContrlInfo(&hdr,pGetInfo);
	Sleep(500);
	m_status="¿ª";
	UpdateData(false);
}