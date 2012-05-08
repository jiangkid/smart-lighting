// AllDesitionCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "AllDesitionCtrl.h"


// CAllDesitionCtrl dialog

IMPLEMENT_DYNAMIC(CAllDesitionCtrl, CDialog)

CAllDesitionCtrl::CAllDesitionCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CAllDesitionCtrl::IDD, pParent)
{

}

CAllDesitionCtrl::~CAllDesitionCtrl()
{
}

void CAllDesitionCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CAllDesitionCtrl, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CAllDesitionCtrl::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON14, &CAllDesitionCtrl::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CAllDesitionCtrl::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CAllDesitionCtrl::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CAllDesitionCtrl::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CAllDesitionCtrl::OnBnClickedButton18)
END_MESSAGE_MAP()


// CAllDesitionCtrl message handlers

BOOL CAllDesitionCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  Add extra initialization here
	m_List.SetHeadings("日期,120;开灯时间,100;关灯时间,100");
	m_List.SetGridLines(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CAllDesitionCtrl::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	if (theApp.m_ZhouAll[0][0].sendtrue==false &&
		theApp.m_ZhouAll[0][1].sendtrue==false &&
		theApp.m_ZhouAll[0][2].sendtrue==false &&
		theApp.m_ZhouAll[0][3].sendtrue==false &&
		theApp.m_ZhouAll[0][4].sendtrue==false &&
		theApp.m_ZhouAll[0][5].sendtrue==false &&
		theApp.m_ZhouAll[0][6].sendtrue==false)
	{
		AfxMessageBox(_T("周策略一未设置"));
	}
	else
	{
		m_List.DeleteAllItems();
		for (int i(0);i<7;i++)
		{
			CString strTimeOn = _T("");
			CString strTimeOff = _T("");
			CString strTimeDate = _T("");
			for(int n(0);n<5;n++)
			{
				strTimeOn+=theApp.m_ZhouAll[0][i].OnTime[n];
				strTimeOff+=theApp.m_ZhouAll[0][i].OffTime[n];
			}
			for(int j(0);j<11;j++)
			{
				strTimeDate+=theApp.m_ZhouAll[0][i].time[j];
			}
			m_List.InsertItem(i,strTimeDate,strTimeOn,strTimeOff);
		}
	}

}

void CAllDesitionCtrl::OnBnClickedButton14()
{
	// TODO: Add your control notification handler code here
	if (theApp.m_ZhouAll[1][0].sendtrue==false &&
		theApp.m_ZhouAll[1][1].sendtrue==false &&
		theApp.m_ZhouAll[1][2].sendtrue==false &&
		theApp.m_ZhouAll[1][3].sendtrue==false &&
		theApp.m_ZhouAll[1][4].sendtrue==false &&
		theApp.m_ZhouAll[1][5].sendtrue==false &&
		theApp.m_ZhouAll[1][6].sendtrue==false)
	{
		AfxMessageBox(_T("周策略二未设置"));
	}
	else
	{
		m_List.DeleteAllItems();
		for (int i(0);i<7;i++)
		{
			CString strTimeOn = _T("");
			CString strTimeOff = _T("");
			CString strTimeDate = _T("");
			for(int n(0);n<5;n++)
			{
				strTimeOn+=theApp.m_ZhouAll[1][i].OnTime[n];
				strTimeOff+=theApp.m_ZhouAll[1][i].OffTime[n];
			}
			for(int j(0);j<11;j++)
			{
				strTimeDate+=theApp.m_ZhouAll[1][i].time[j];
			}
			m_List.InsertItem(i,strTimeDate,strTimeOn,strTimeOff);
		}
	}
}

void CAllDesitionCtrl::OnBnClickedButton15()
{
	// TODO: Add your control notification handler code here
	if (theApp.m_ZhouAll[2][0].sendtrue==false &&
		theApp.m_ZhouAll[2][1].sendtrue==false &&
		theApp.m_ZhouAll[2][2].sendtrue==false &&
		theApp.m_ZhouAll[2][3].sendtrue==false &&
		theApp.m_ZhouAll[2][4].sendtrue==false &&
		theApp.m_ZhouAll[2][5].sendtrue==false &&
		theApp.m_ZhouAll[2][6].sendtrue==false)
	{
		AfxMessageBox(_T("周策略三未设置"));
	}
	else
	{
		m_List.DeleteAllItems();
		for (int i(0);i<7;i++)
		{
			CString strTimeOn = _T("");
			CString strTimeOff = _T("");
			CString strTimeDate = _T("");
			for(int n(0);n<5;n++)
			{
				strTimeOn+=theApp.m_ZhouAll[2][i].OnTime[n];
				strTimeOff+=theApp.m_ZhouAll[2][i].OffTime[n];
			}
			for(int j(0);j<11;j++)
			{
				strTimeDate+=theApp.m_ZhouAll[2][i].time[j];
			}
			m_List.InsertItem(i,strTimeDate,strTimeOn,strTimeOff);
		}
	}
}

void CAllDesitionCtrl::OnBnClickedButton16()
{
	// TODO: Add your control notification handler code here
	if (theApp.m_ZhouAll[3][0].sendtrue==false &&
		theApp.m_ZhouAll[3][1].sendtrue==false &&
		theApp.m_ZhouAll[3][2].sendtrue==false &&
		theApp.m_ZhouAll[3][3].sendtrue==false &&
		theApp.m_ZhouAll[3][4].sendtrue==false &&
		theApp.m_ZhouAll[3][5].sendtrue==false &&
		theApp.m_ZhouAll[3][6].sendtrue==false)
	{
		AfxMessageBox(_T("周策略四未设置"));
	}
	else
	{
		m_List.DeleteAllItems();
		for (int i(0);i<7;i++)
		{
			CString strTimeOn = _T("");
			CString strTimeOff = _T("");
			CString strTimeDate = _T("");
			for(int n(0);n<5;n++)
			{
				strTimeOn+=theApp.m_ZhouAll[3][i].OnTime[n];
				strTimeOff+=theApp.m_ZhouAll[3][i].OffTime[n];
			}
			for(int j(0);j<11;j++)
			{
				strTimeDate+=theApp.m_ZhouAll[3][i].time[j];
			}
			m_List.InsertItem(i,strTimeDate,strTimeOn,strTimeOff);
		}
	}
}

void CAllDesitionCtrl::OnBnClickedButton17()
{
	// TODO: Add your control notification handler code here
	
	if (theApp.m_JieAll[0].sendtrue==false &&
		theApp.m_JieAll[1].sendtrue==false &&
		theApp.m_JieAll[2].sendtrue==false &&
		theApp.m_JieAll[3].sendtrue==false &&
		theApp.m_JieAll[4].sendtrue==false &&
		theApp.m_JieAll[5].sendtrue==false &&
		theApp.m_JieAll[6].sendtrue==false)
	{
		AfxMessageBox(_T("节假日策略未设置"));
	}
	else
	{
		m_List.DeleteAllItems();
		for (int i(0);i<7;i++)
		{
			if (theApp.m_JieAll[i].sendtrue)
			{
				CString strTimeOn = _T("");
				CString strTimeOff = _T("");
				CString strTimeDate = _T("");
				for(int n(0);n<5;n++)
				{
					strTimeOn+=theApp.m_JieAll[i].OnTime[n];
					strTimeOff+=theApp.m_JieAll[i].OffTime[n];
				}
				for(int j(0);j<11;j++)
				{
					strTimeDate+=theApp.m_JieAll[i].time[j];
				}
				m_List.InsertItem(i,strTimeDate,strTimeOn,strTimeOff);
			}
		}
	}
}

void CAllDesitionCtrl::OnBnClickedButton18()
{
	// TODO: Add your control notification handler code here
	if(theApp.m_TeshuAll[0].sendtrue==false)
	{
		AfxMessageBox(_T("特殊策略未设置"));
	}
	else
	{
		m_List.DeleteAllItems();
		CString strTimeOn = _T("");
		CString strTimeOff = _T("");
		CString strTimeDate = _T("");
		for(int n(0);n<5;n++)
		{
			strTimeOn+=theApp.m_TeshuAll[0].OnTime[n];
			strTimeOff+=theApp.m_TeshuAll[0].OffTime[n];
		}
		for(int j(0);j<11;j++)
		{
			strTimeDate+=theApp.m_TeshuAll[0].time[j];
		}
		m_List.InsertItem(0,strTimeDate,strTimeOn,strTimeOff);
	}
}
