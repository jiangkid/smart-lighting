// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
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
	unsigned char buffer[30]={'N',0x2F,0x43,0x2F,0x01,
		'0','1','0','1','0','1','0','1','0','1','0','1','0','1','0','1',
		0x0A,0xA2,0xB1,0x00,0x00,0x00,0x00,0x00,0xCC};
	send(theApp.m_ConnectSock,(char*)buffer,30,0);
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
