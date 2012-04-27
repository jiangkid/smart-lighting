// MapViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MapViewDlg.h"

// CMapViewDlg dialog
#include <atlbase.h>

IMPLEMENT_DYNAMIC(CMapViewDlg, CDialog)

CMapViewDlg::CMapViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapViewDlg::IDD, pParent)
	, m_lat(_T(""))
	, m_lng(_T(""))
{
}

CMapViewDlg::~CMapViewDlg()
{
}

void CMapViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	//DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	DDX_Text(pDX, IDC_EDIT1, m_lat);
	DDX_Text(pDX, IDC_EDIT2, m_lng);
	DDX_Control(pDX, IDC_LIST1, m_List);
}

BEGIN_MESSAGE_MAP(CMapViewDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapViewDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapViewDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapViewDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapViewDlg::OnBnClickedButton4)
END_MESSAGE_MAP()



BOOL CMapViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pMapViewDlg=this;
	// TODO:  Add extra initialization here
	m_List.SetHeadings("������ID,100;�����侭��,120;������γ��,120;");
	m_List.SetGridLines(true);
	CRect rect;
	GetClientRect(&rect);

	m_pMapInfo	= new CMapInfomation();
	rect.DeflateRect(10,10,400,5);
	m_pMapInfo->Create(IDD_HTMLMAP,this);
	m_pMapInfo->MoveWindow(rect);
	m_pMapInfo->ShowWindow(SW_SHOW);

	return TRUE;  
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMapViewDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
// 	UpdateData(TRUE);
// 	CString str1;
// 	CString str2;
// 	GetDlgItemText(IDC_EDIT1,str1);
// 	GetDlgItemText(IDC_EDIT2,str2);
// 	theApp.m_pMapInfoDlg->CallJScript("addMarker",str1,str2,NULL);
	
}

void CMapViewDlg::ShowMessage(const CString str1,const CString str2)
{
	//SetDlgItemText(IDC_EDIT1,str1);
	//SetDlgItemText(IDC_EDIT2,str2);
	//CString str3=str1;
	//CString str4=str2;
	//theApp.m_pMapInfoDlg->CallJScript("addMarker",str3,str4,NULL);
// 	int nCount = m_List.GetItemCount();
// 	for (int i(0);i<nCount;i++)
// 	{
// 		CString str1 = m_List.GetItemText(i,1);
// 		CString str2 = m_List.GetItemText(i,2);
// 		theApp.m_pMapInfoDlg->CallJScript("addMarker",str1,str2,NULL);
// 	}
	
}

void CMapViewDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CString str1;
	CString str2;
	GetDlgItemText(IDC_EDIT1,str1);
	GetDlgItemText(IDC_EDIT2,str2);
	float a = (float)strtod(str1,NULL);
	float b = (float)strtod(str2,NULL);
	char* sendbuff = (char*)malloc(sizeof(PointInfo)+2);
	ZeroMemory(sendbuff,sizeof(PointInfo)+2);
	char c[2]={'S',0x42};
	memcpy(sendbuff,c,2);
	PointInfo* pGetInfo = (PointInfo*)malloc(sizeof(PointInfo));
	ZeroMemory(pGetInfo,sizeof(PointInfo));
	pGetInfo->GID[0]='0';
	pGetInfo->GID[1]='1';
	pGetInfo->Longtitude=a*100;
	pGetInfo->Latitude=b*100;
	memcpy(pGetInfo->LongtitudeSave,str1.GetBuffer(),str1.GetLength());
	memcpy(pGetInfo->LatitudeSave,str2.GetBuffer(),str2.GetLength());
	memcpy(sendbuff+2,pGetInfo,sizeof(PointInfo));
	send(theApp.m_ConnectSock,sendbuff,sizeof(PointInfo)+2,0);
	free(sendbuff);
	free(pGetInfo);
}
void CMapViewDlg::ShowInfomation(int nItem, MAPInfo* pGetInfo)
{
	CString strID;
	CString strLatitude;
	CString strLongtitude;
	CString strTerminal[4];
	CString strIntallTime;
	CString strUserName;
	CString strName;
	for (int i(0);i<2;i++)
	{
		strID+=pGetInfo->GID[i];
	}
	for (int m(0);m<10;m++)
	{
		strLatitude+=pGetInfo->Latitude[m];
		strLongtitude+=pGetInfo->Longtitude[m];
	}
	for (int j(0);j<4;j++)
	{
		strIntallTime=_T("");
		strUserName=_T("");
		strName=_T("");
		for(int k(0);k<20;k++)
		{
			strTerminal[j]+=pGetInfo->TeminalName[j][k];
			strIntallTime+=pGetInfo->Time[k];
			strUserName+=pGetInfo->UserName[k];
			strName+=pGetInfo->GlableAreaName[k];
		}
	}
	if(strLongtitude==""||strLatitude=="")
	{
		m_List.InsertItem(nItem,strID,strLongtitude,strLatitude);
		m_List.SetItemData(nItem,(DWORD)&theApp.m_MapInfo[nItem]);
	}
	else
	{
		m_List.InsertItem(nItem,strID,strLongtitude,strLatitude);
		m_List.SetItemData(nItem,(DWORD)&theApp.m_MapInfo[nItem]);
		//ShowMessage(strLongtitude,strLatitude);
		//theApp.m_pMapInfoDlg->CallJScript("addMarker",strLongtitude,strLatitude,NULL);
	}
	
}

void CMapViewDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	//theApp.m_pMapInfoDlg->CallJScript("clearOverlays",NULL);
	m_List.DeleteAllItems();
	if(userInfo[0].Idetify==0x30)
	{
		char c[3]={'G',0x32,0x30};
		CString str = c;
		str+=userInfo[0].UserName;
		str+='#';
		send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
		str.ReleaseBuffer();
	}
	else
	{
		char c[3]={'G',0x32,0x31};
		CString str = c;
		str+=userInfo[0].UserName;
		str+='#';
		send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
		str.ReleaseBuffer();
	}
}

void CMapViewDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	int nCount = m_List.GetItemCount();
	for (int i(0);i<nCount;i++)
	{
		CString strTerminal[4];
		CString strIntallTime;
		CString strUserName;
		CString strName;
		CString str1 = m_List.GetItemText(i,1);
		CString str2 = m_List.GetItemText(i,2);
		MAPInfo* pGetInfo = (MAPInfo*)m_List.GetItemData(i);
		for (int n(0);n<20;n++)
		{
			strIntallTime+=pGetInfo->Time[n];
			strName+=pGetInfo->GlableAreaName[n];
			strUserName+=pGetInfo->UserName[n];
		}
		for (int j(0);j<4;j++)
		{
			for (int m(0);m<20;m++)
			{
				strTerminal[j]+=pGetInfo->TeminalName[j][m];
			}
		}
		theApp.m_pMapInfoDlg->CallJScript("addMarker",str1,str2,NULL);
	}
}