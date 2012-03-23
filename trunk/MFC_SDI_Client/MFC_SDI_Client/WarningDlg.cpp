// WarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "WarningDlg.h"


// CWarningDlg dialog

IMPLEMENT_DYNAMIC(CWarningDlg, CDialog)

CWarningDlg::CWarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningDlg::IDD, pParent)
{

}

CWarningDlg::~CWarningDlg()
{
}

void CWarningDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WARNING, m_ListWarning);
	DDX_Control(pDX, IDC_BEGIN_DATA, m_DataBegin);
	DDX_Control(pDX, IDC_END_DATA, m_DataEnd);
}


BEGIN_MESSAGE_MAP(CWarningDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_GET, &CWarningDlg::OnBnClickedBtnGet)
END_MESSAGE_MAP()


// CWarningDlg message handlers

BOOL CWarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pWarningInfoView=this;
	m_DataBegin.SetFormat("yyyy-M-d");
	m_DataEnd.SetFormat("yyyy-M-d");
	m_ListWarning.SetHeadings("时间,120;对应故障ID,120;对应故障位置,120;对应故障类型,120;对应故障具体值,120;");
	m_ListWarning.SetGridLines(TRUE);
	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWarningDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//CDialog::OnOK();
}

void CWarningDlg::OnBnClickedBtnGet()
{
	// TODO: Add your control notification handler code here
	CString str=_T("");
	CString strTime1=_T("");
	CString strTime2=_T("");
	char a[2]={'X',0x31};
	for (int i(0);i<2;i++)
	{
		str+=a[i];
	}
	CTime m_GTimeBegin;
	DWORD dwResult1= m_DataBegin.GetTime(m_GTimeBegin);
	if (dwResult1== GDT_VALID)
	{
		if ((m_DataBegin.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime1 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime1.Format("%d-%d-%d",m_GTimeBegin.GetYear(),
			m_GTimeBegin.GetMonth(),m_GTimeBegin.GetDay());
	}
	CTime m_GTimeEnd;
	DWORD dwResult2= m_DataEnd.GetTime(m_GTimeEnd);
	if (dwResult2 == GDT_VALID)
	{
		if ((m_DataEnd.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime2 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime2.Format("%d-%d-%d",m_GTimeEnd.GetYear(),
			m_GTimeEnd.GetMonth(),m_GTimeEnd.GetDay());
	}
	str+=strTime1;
	str+='+';
	str+=strTime2;
	str+='#';
	send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
}
