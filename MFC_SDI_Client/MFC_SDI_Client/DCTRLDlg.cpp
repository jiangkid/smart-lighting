// DCTRLDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "DCTRLDlg.h"


// CDCTRLDlg dialog

IMPLEMENT_DYNAMIC(CDCTRLDlg, CDialog)

CDCTRLDlg::CDCTRLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDCTRLDlg::IDD, pParent)
{

}

CDCTRLDlg::~CDCTRLDlg()
{
}

void CDCTRLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO1, m_Zbox);
	DDX_Control(pDX, IDC_COMBO3, m_Jbox);
	DDX_Control(pDX, IDC_COMBO4, m_Tbox);
}


BEGIN_MESSAGE_MAP(CDCTRLDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CDCTRLDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDCTRLDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CDCTRLDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDCTRLDlg message handlers

void CDCTRLDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	int nZ = m_Zbox.GetCurSel();
	int nJ = m_Jbox.GetCurSel();
	int nT = m_Tbox.GetCurSel();
	if (nZ == 0 && nJ == 0 && nT==0)
	{
		AfxMessageBox(_T("没有选择一个策略"));
		return;
	}
	char cZ[10] = {'S',0x44,0x30,0x00,0x00,0x00,0x00,'Z',0x00,'#'};
	char cJ[10] = {'S',0x44,0x30,0x00,0x00,0x00,0x00,'J',0x00,'#'};
	char cT[10] = {'S',0x44,0x30,0x00,0x00,0x00,0x00,'T',0x00,'#'};
	switch (nZ)
	{
	case 1:
		cZ[8]=0x31;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	case 2:
		cZ[8]=0x32;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	case 3:
		cZ[8]=0x33;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	case 4:
		cZ[8]=0x34;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	default:
		break;
	}
	Sleep(500);

	switch (nJ)
	{
	case 1:
		memcpy(&cJ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cJ,10,0);
		strID.ReleaseBuffer();
		break;
	default:
		break;
	}
	Sleep(500);
	switch (nT)
	{
	case 1:
		memcpy(&cT[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cT,10,0);
		strID.ReleaseBuffer();
		break;
	default:
		break;
	}

}

BOOL CDCTRLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_DCtrlDlg=this;
	// TODO:  Add extra initialization here
	m_List.SetHeadings("策略,180;是否绑定,120");
	m_List.SetGridLines(true);
	ShowBandDesition();
	InitialBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDCTRLDlg::ShowBandDesition(void)
{
	CString str=_T("基本策略");
	CString str1=_T("周策略");
	CString str2=_T("节假日策略");
	CString str3=_T("特殊策略");
	m_List.InsertItem(0,str,UNBAND);
	m_List.InsertItem(1,str1,UNBAND);
	m_List.InsertItem(2,str2,UNBAND);
	m_List.InsertItem(3,str3,UNBAND);
}

void CDCTRLDlg::InitialBox(void)
{	
	m_Zbox.InsertString(0,_T("未选择"));
	m_Jbox.InsertString(0,_T("未选择"));
	m_Tbox.InsertString(0,_T("未选择"));
	m_Zbox.InsertString(1,_T("周策略一"));
	m_Zbox.InsertString(2,_T("周策略二"));
	m_Zbox.InsertString(3,_T("周策略三"));
	m_Zbox.InsertString(4,_T("周策略四"));
	m_Jbox.InsertString(1,_T("节假日策略"));
	m_Tbox.InsertString(1,_T("特殊策略"));
	m_Zbox.SetCurSel(0);
	m_Jbox.SetCurSel(0);
	m_Tbox.SetCurSel(0);
}

void CDCTRLDlg::EnableButton2(CString str)
{
	strID = str;
	CWnd* pWnd = this->GetDlgItem(IDC_BUTTON2);
	char c[4] = {0x00,0x00,0x00,0x00};
	memcpy(c,str.GetBuffer(),4);
	str.ReleaseBuffer();
	if (c[3]==0x31)
	{
		pWnd->EnableWindow(true);
	}
	else
	{
		pWnd->EnableWindow(false);
	}
}

void CDCTRLDlg::ShowBandInfoDate(StrategyInfo* pGetInfo)
{
	if (pGetInfo->sendtrue)
	{
		CString str1=_T("基本策略");
		CString str2=_T("已设置");
		m_List.DeleteItem(0);
		m_List.InsertItem(0,str1,str2);
	}
}

void CDCTRLDlg::ShowBandInfoWeek(StrategyInfo* pGetInfo)
{
	if (pGetInfo->sendtrue)
	{
		CString str1=_T("周策略");
		CString str2=_T("周策略1");
		CString str3=_T("周策略2");
		CString str4=_T("周策略3");
		CString str5=_T("周策略4");
		switch (pGetInfo->strategyType)
		{
		case 0x02:
			m_List.DeleteItem(1);
			m_List.InsertItem(1,str1,str2);
			break;
		case 0x03:
			m_List.DeleteItem(1);
			m_List.InsertItem(1,str1,str3);
			break;
		case 0x04:
			m_List.DeleteItem(1);
			m_List.InsertItem(1,str1,str4);
			break;
		case 0x05:
			m_List.DeleteItem(1);
			m_List.InsertItem(1,str1,str5);
			break;
		default:
			break;
		}
	}
}
void CDCTRLDlg::ShowBandInfoJie(StrategyInfo* pGetInfo)
{
	if (pGetInfo->sendtrue)
	{
		CString str1=_T("节假日策略");
		CString str2=_T("已设置");
		m_List.DeleteItem(2);
		m_List.InsertItem(2,str1,str2);
	}
}
void CDCTRLDlg::ShowBandInfoTe(StrategyInfo* pGetInfo)
{
	if (pGetInfo->sendtrue)
	{
		CString str1=_T("特殊策略");
		CString str2=_T("已设置");
		m_List.DeleteItem(3);
		m_List.InsertItem(3,str1,str2);
	}
}
void CDCTRLDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	int nZ = m_Zbox.GetCurSel();
	int nJ = m_Jbox.GetCurSel();
	int nT = m_Tbox.GetCurSel();
	if (nZ == 0 && nJ == 0 && nT==0)
	{
		AfxMessageBox(_T("没有选择一个策略"));
		return;
	}
	char cZ[10] = {'S',0x44,0x31,0x00,0x00,0x00,0x00,'Z',0x00,'#'};
	char cJ[10] = {'S',0x44,0x31,0x00,0x00,0x00,0x00,'J',0x00,'#'};
	char cT[10] = {'S',0x44,0x31,0x00,0x00,0x00,0x00,'T',0x00,'#'};
	switch (nZ)
	{
	case 1:
		cZ[8]=0x31;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	case 2:
		cZ[8]=0x32;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	case 3:
		cZ[8]=0x33;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	case 4:
		cZ[8]=0x34;
		memcpy(&cZ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cZ,10,0);
		strID.ReleaseBuffer();
		break;
	default:
		break;
	}
	Sleep(500);

	switch (nJ)
	{
	case 1:
		memcpy(&cJ[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cJ,10,0);
		strID.ReleaseBuffer();
		break;
	default:
		break;
	}
	Sleep(500);
	switch (nT)
	{
	case 1:
		memcpy(&cT[3],strID.GetBuffer(),4);
		send(theApp.m_ConnectSock,cT,10,0);
		strID.ReleaseBuffer();
		break;
	default:
		break;
	}
}

void CDCTRLDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CAllDesitionCtrl dlg;
	dlg.DoModal();
}
