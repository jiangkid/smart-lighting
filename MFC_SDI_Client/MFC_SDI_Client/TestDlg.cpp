// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MainFrm.h"
#include "TestDlg.h"


// CTestDlg dialog

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTestDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestDlg::OnBnClickedButton8)
END_MESSAGE_MAP()


// CTestDlg message handlers
void CTestDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA1,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
	send(theApp.m_ConnectSock,(char*)buffer,30,0);
}

void CTestDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
// 	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
// 		'0','2','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
// 		0x0A,0xA2,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
// 	send(theApp.m_ConnectSock,(char*)buffer,30,0);
//  	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
//  	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
//  
//  	pGetRInfo->m_First[0]=0x2F;
//  	pGetRInfo->m_First[1]=0x43;
//  	pGetRInfo->m_First[2]=0x2F;
//  	pGetRInfo->m_First[3]=0x01;
//  	CString str;
//  	GetDlgItemText(IDC_ID,str);
// 		strcpy((char*)pGetRInfo->m_ID,str.GetBuffer(str.GetLength()));
//  	pGetRInfo->m_OrderType[0]=0x0A;
//  	pGetRInfo->m_OrderObject[0]=0xA3;
// 		pGetRInfo->m_ActiveType[0]=0xBD;
//  	pGetRInfo->m_EndBuffer[1]=0xCC;
//  	SendContrlInfo(&hdr,pGetRInfo);
}

void CTestDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA3,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
	send(theApp.m_ConnectSock,(char*)buffer,30,0);
}

void CTestDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA1,0xB2,0x00,0x00,0x00,0x00,0x00,0xCC};
	send(theApp.m_ConnectSock,(char*)buffer,30,0);
}

void CTestDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here
	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA2,0xB2,0x00,0x00,0x00,0x00,0x00,0xCC};
	send(theApp.m_ConnectSock,(char*)buffer,30,0);
}

void CTestDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here
	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA3,0xB2,0x00,0x00,0x00,0x00,0x00,0xCC};
	send(theApp.m_ConnectSock,(char*)buffer,30,0);
}
void CTestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
 	UpdateData(true);
	ConTrlInfo* pGetRInfo = (ConTrlInfo*)malloc(sizeof(ConTrlInfo));
	ZeroMemory(pGetRInfo,sizeof(ConTrlInfo));
 	CString str1=_T("");
 	CString str2=_T("");
	CString str3=_T("");
	CString str4=_T("");
	CString str5=_T("");
	CString str6=_T("");
 	GetDlgItemText(IDC_FIRST,str1);
 	GetDlgItemText(IDC_ID,str2);
 	GetDlgItemText(IDC_Type,str3);
 	GetDlgItemText(IDC_order,str4);
 	GetDlgItemText(IDC_active,str5);
 	GetDlgItemText(IDC_activeType,str6);
	char* aa=(char*)malloc(str1.GetLength()+1);
	char* bb=(char*)malloc(str2.GetLength()+1);
	char* cc=(char*)malloc(str3.GetLength()+1);
	char* dd=(char*)malloc(str4.GetLength()+1);
	char* ee=(char*)malloc(str5.GetLength()+1);
	char* ff=(char*)malloc(str6.GetLength()+1);
	char* gg=(char*)malloc(str6.GetLength()+1);
 	memcpy(aa,(char*)str1.GetBuffer(),str1.GetLength());
 	memcpy(bb,(char*)str2.GetBuffer(),str2.GetLength());
 	memcpy(cc,(char*)str3.GetBuffer(),str3.GetLength());
 	memcpy(dd,(char*)str4.GetBuffer(),str4.GetLength());
 	memcpy(ee,(char*)str5.GetBuffer(),str5.GetLength());
 	memcpy(ff,(char*)str6.GetBuffer(),str6.GetLength());
 	char* a_first;
 	char* b_id;
 	char* c_Type;
 	char* d_order;
 	char* e_active;
 	char* f_activeType;
 	a_first=Translation_ID(aa,8);
	memcpy((char*)pGetRInfo->m_First,a_first,4);
	memcpy((char*)pGetRInfo->m_ID,bb,16);
 	c_Type=Translation_ID(cc,2);
	memcpy((char*)pGetRInfo->m_OrderType,c_Type,1);
 	d_order=Translation_ID(dd,2);
	memcpy((char*)pGetRInfo->m_OrderObject,d_order,1);
 	e_active=Translation_ID(ee,2);
	memcpy((char*)pGetRInfo->m_ActiveType,e_active,1);
 	f_activeType=Translation_ID(ff,8);
 	memcpy((char*)pGetRInfo->m_CheckData,f_activeType,4);
 	pGetRInfo->m_EndBuffer[0]=0x00;
 	pGetRInfo->m_EndBuffer[1]=0xCC;
 	SendContrlInfo(&hdr,pGetRInfo);
	free(aa);
	free(bb);
	free(cc);
	free(dd);
	free(ee);
	free(ff);
	free(pGetRInfo);
}
char* CTestDlg::Translation_ID(char* buffer, int Length)
{
	char c[8];
	ZeroMemory(c,8);
	int nCount = 0;
	CString temp;
	int i,j;
	for (i=0;i<Length/2;i++)
	{
		CString temp="";
		for (j=nCount*2;j<nCount*2+2;j++)
		{
			temp+=buffer[j];
		}
		sscanf(temp,"%2x",&c[nCount]);
		nCount++;
	}
	 return c; 
}
void CTestDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	char buff[9000];
	memset(buff,'0',4096);
	memset(buff+4096,'1',4096);
// 	buff[0]=0x31;
// 	buff[8000]=0x31;
	char datacheck[2];
	char sendbuffer[4096];
	datacheck[0]='Z';
	datacheck[1]=0x30;
	memcpy(sendbuffer,datacheck,2);
	memcpy(sendbuffer+2,buff,4094);
	send(theApp.m_ConnectSock,sendbuffer,4096,0);
	Sleep(500);
	datacheck[0]='Z';
	datacheck[1]=0x31;
	memcpy(sendbuffer,datacheck,2);
	memcpy(sendbuffer+2,buff+4094,4094);
	send(theApp.m_ConnectSock,sendbuffer,4096,0);
}