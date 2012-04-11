// NewWeekDecisionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "NewWeekDecisionDlg.h"


// CNewWeekDecisionDlg dialog

IMPLEMENT_DYNAMIC(CNewWeekDecisionDlg, CDialog)

CNewWeekDecisionDlg::CNewWeekDecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewWeekDecisionDlg::IDD, pParent)
	, m_weekName(_T(""))
{

}

CNewWeekDecisionDlg::~CNewWeekDecisionDlg()
{
}

void CNewWeekDecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_Type);
	DDX_Control(pDX, IDC_WEEK1BA, m_week1BA);
	DDX_Control(pDX, IDC_WEEK1EA, m_week1EA);
	DDX_Control(pDX, IDC_WEEK1BH, m_week1BH);
	DDX_Control(pDX, IDC_WEEK1EH, m_week1EH);
	DDX_Control(pDX, IDC_WEEK2BA, m_week2BA);
	DDX_Control(pDX, IDC_WEEK2EA, m_week2EA);
	DDX_Control(pDX, IDC_WEEK2BH, m_week2BH);
	DDX_Control(pDX, IDC_WEEK2EH, m_week2EH);
	DDX_Control(pDX, IDC_WEEK3BA, m_week3BA);
	DDX_Control(pDX, IDC_WEEK3EA, m_week3EA);
	DDX_Control(pDX, IDC_WEEK3BH, m_week3BH);
	DDX_Control(pDX, IDC_WEEK3EH, m_week3EH);
	DDX_Control(pDX, IDC_WEEK4BA, m_week4BA);
	DDX_Control(pDX, IDC_WEEK4EA, m_week4EA);
	DDX_Control(pDX, IDC_WEEK4BH, m_week4BH);
	DDX_Control(pDX, IDC_WEEK4EH, m_week4EH);
	DDX_Control(pDX, IDC_WEEK5BA, m_week5BA);
	DDX_Control(pDX, IDC_WEEK5EA, m_week5EA);
	DDX_Control(pDX, IDC_WEEK5BH, m_week5BH);
	DDX_Control(pDX, IDC_WEEK5EH, m_week5EH);
	DDX_Control(pDX, IDC_WEEK6BA, m_week6BA);
	DDX_Control(pDX, IDC_WEEK6EA, m_week6EA);
	DDX_Control(pDX, IDC_WEEK6BH, m_week6BH);
	DDX_Control(pDX, IDC_WEEK6EH, m_week6EH);
	DDX_Control(pDX, IDC_WEEK7BA, m_week7BA);
	DDX_Control(pDX, IDC_WEEK7EA, m_week7EA);
	DDX_Control(pDX, IDC_WEEK7BH, m_week7BH);
	DDX_Control(pDX, IDC_WEEK7EH, m_week7EH);
	DDX_Text(pDX, IDC_WeekName, m_weekName);
	//DDX_Control(pDX, IDC_WeekName, m_strName);
}


BEGIN_MESSAGE_MAP(CNewWeekDecisionDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CANCLE, &CNewWeekDecisionDlg::OnBnClickedBtnCancle)
	ON_BN_CLICKED(IDC_BTN_SURE, &CNewWeekDecisionDlg::OnBnClickedBtnSure)
END_MESSAGE_MAP()


// CNewWeekDecisionDlg message handlers

BOOL CNewWeekDecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Type.InsertString(0,_T("周策略一"));
	m_Type.InsertString(1,_T("周策略二"));
	m_Type.InsertString(2,_T("周策略三"));
	m_Type.InsertString(3,_T("周策略四"));
	m_Type.SetCurSel(0);
	m_week1BA.SetFormat("HH:mm");
	m_week1EA.SetFormat("HH:mm");
	m_week1BH.SetFormat("HH:mm");
	m_week1EH.SetFormat("HH:mm");
	m_week2BA.SetFormat("HH:mm");
	m_week2EA.SetFormat("HH:mm");
	m_week2BH.SetFormat("HH:mm");
	m_week2EH.SetFormat("HH:mm");
	m_week3BA.SetFormat("HH:mm");
	m_week3EA.SetFormat("HH:mm");
	m_week3BH.SetFormat("HH:mm");
	m_week3EH.SetFormat("HH:mm");
	m_week4BA.SetFormat("HH:mm");
	m_week4EA.SetFormat("HH:mm");
	m_week4BH.SetFormat("HH:mm");
	m_week4EH.SetFormat("HH:mm");
	m_week5BA.SetFormat("HH:mm");
	m_week5EA.SetFormat("HH:mm");
	m_week5BH.SetFormat("HH:mm");
	m_week5EH.SetFormat("HH:mm");
	m_week6BA.SetFormat("HH:mm");
	m_week6EA.SetFormat("HH:mm");
	m_week6BH.SetFormat("HH:mm");
	m_week6EH.SetFormat("HH:mm");
	m_week7BA.SetFormat("HH:mm");
	m_week7EA.SetFormat("HH:mm");
	m_week7BH.SetFormat("HH:mm");
	m_week7EH.SetFormat("HH:mm");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewWeekDecisionDlg::OnBnClickedBtnCancle()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CNewWeekDecisionDlg::OnBnClickedBtnSure()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_weekName==_T(""))
	{
		AfxMessageBox(_T("周策略名字不能为空"));
		return;
	}

	char* c=(char*)malloc(DLONG*7+2);
	ZeroMemory(c,DLONG*7+2);
	HDR2 hdr2;
	hdr2.dataCheck[0]='F';
	hdr2.nRet[0]=0x07;
	memcpy(c,&hdr2,2);

	GetInfo1();
	GetInfo2();
	GetInfo3();
	GetInfo4();
	GetInfo5();
	GetInfo6();
	GetInfo7();	
	int nRet(0);
	nRet=m_Type.GetCurSel();
	switch (nRet)
	{
	case 0:
		pGetInfo1->strategyType[0]='Z';
		pGetInfo1->strategyType[1]=0x31;
		pGetInfo2->strategyType[0]='Z';
		pGetInfo2->strategyType[1]=0x31;
		pGetInfo3->strategyType[0]='Z';
		pGetInfo3->strategyType[1]=0x31;
		pGetInfo4->strategyType[0]='Z';
		pGetInfo4->strategyType[1]=0x31;
		pGetInfo5->strategyType[0]='Z';
		pGetInfo5->strategyType[1]=0x31;
		pGetInfo6->strategyType[0]='Z';
		pGetInfo6->strategyType[1]=0x31;
		pGetInfo7->strategyType[0]='Z';
		pGetInfo7->strategyType[1]=0x31;
		break;
	case 1:
		pGetInfo1->strategyType[0]='Z';
		pGetInfo1->strategyType[1]=0x32;
		pGetInfo2->strategyType[0]='Z';
		pGetInfo2->strategyType[1]=0x32;
		pGetInfo3->strategyType[0]='Z';
		pGetInfo3->strategyType[1]=0x32;
		pGetInfo4->strategyType[0]='Z';
		pGetInfo4->strategyType[1]=0x32;
		pGetInfo5->strategyType[0]='Z';
		pGetInfo5->strategyType[1]=0x32;
		pGetInfo6->strategyType[0]='Z';
		pGetInfo6->strategyType[1]=0x32;
		pGetInfo7->strategyType[0]='Z';
		pGetInfo7->strategyType[1]=0x32;
		break;
	case 3:
		pGetInfo1->strategyType[0]='Z';
		pGetInfo1->strategyType[1]=0x33;
		pGetInfo2->strategyType[0]='Z';
		pGetInfo2->strategyType[1]=0x33;
		pGetInfo3->strategyType[0]='Z';
		pGetInfo3->strategyType[1]=0x33;
		pGetInfo4->strategyType[0]='Z';
		pGetInfo4->strategyType[1]=0x33;
		pGetInfo5->strategyType[0]='Z';
		pGetInfo5->strategyType[1]=0x33;
		pGetInfo6->strategyType[0]='Z';
		pGetInfo6->strategyType[1]=0x33;
		pGetInfo7->strategyType[0]='Z';
		pGetInfo7->strategyType[1]=0x33;
		break;
	case 4:
		pGetInfo1->strategyType[0]='Z';
		pGetInfo1->strategyType[1]=0x34;
		pGetInfo2->strategyType[0]='Z';
		pGetInfo2->strategyType[1]=0x34;
		pGetInfo3->strategyType[0]='Z';
		pGetInfo3->strategyType[1]=0x34;
		pGetInfo4->strategyType[0]='Z';
		pGetInfo4->strategyType[1]=0x34;
		pGetInfo5->strategyType[0]='Z';
		pGetInfo5->strategyType[1]=0x34;
		pGetInfo6->strategyType[0]='Z';
		pGetInfo6->strategyType[1]=0x34;
		pGetInfo7->strategyType[0]='Z';
		pGetInfo7->strategyType[1]=0x34;
		break;
	default:
		break;
	}
	memcpy(c+2,pGetInfo1,DLONG);
	memcpy(c+2+DLONG,pGetInfo2,DLONG);
	memcpy(c+2+DLONG*2,pGetInfo3,DLONG);
	memcpy(c+2+DLONG*3,pGetInfo4,DLONG);
	memcpy(c+2+DLONG*4,pGetInfo5,DLONG);
	memcpy(c+2+DLONG*5,pGetInfo6,DLONG);
	memcpy(c+2+DLONG*6,pGetInfo7,DLONG);
	send(theApp.m_ConnectSock,c,DLONG*7+2,0);
	free(c);
	free(pGetInfo1);
	free(pGetInfo2);
	free(pGetInfo3);
	free(pGetInfo4);
	free(pGetInfo5);
	free(pGetInfo6);
	free(pGetInfo7);
}

void CNewWeekDecisionDlg::GetInfo1(void)
{
	CString str1=_T("星期一");
	pGetInfo1 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo1,sizeof(Decision));	
	memcpy(pGetInfo1->day,str1.GetBuffer(),6);
	str1.ReleaseBuffer();
	memcpy(pGetInfo1->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult11= m_week1BA.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_week1BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_week1EA.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_week1EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_week1BH.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_week1BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_week1EH.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_week1EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo1->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo1->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo1->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo1->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
}
void CNewWeekDecisionDlg::GetInfo2(void)
{
	CString str2=_T("星期二");
	pGetInfo2 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo2,sizeof(Decision));
	memcpy(pGetInfo2->day,str2.GetBuffer(),6);
	str2.ReleaseBuffer();
	memcpy(pGetInfo2->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime21=_T("");
	CString strTime22=_T("");
	CString strTime23=_T("");
	CString strTime24=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult21= m_week2BA.GetTime(m_GTimeBegin);
	if (dwResult21== GDT_VALID)
	{
		if ((m_week2BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime21 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime21.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult22= m_week2EA.GetTime(m_GTimeEnd);
	if (dwResult22 == GDT_VALID)
	{
		if ((m_week2EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime22 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime22.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult23= m_week2BH.GetTime(m_GTimeBeginHalf);
	if (dwResult23== GDT_VALID)
	{
		if ((m_week2BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime23 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime23.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult24= m_week1EH.GetTime(m_GTimeEndHalf);
	if (dwResult24 == GDT_VALID)
	{
		if ((m_week2EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime24 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime24.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo2->openTime,strTime21.GetBuffer(),5);
	memcpy(pGetInfo2->closeTime,strTime22.GetBuffer(),5);
	memcpy(pGetInfo2->Half_openTime,strTime23.GetBuffer(),5);
	memcpy(pGetInfo2->Half_closeTime,strTime24.GetBuffer(),5);
	strTime21.ReleaseBuffer();
	strTime22.ReleaseBuffer();
	strTime23.ReleaseBuffer();
	strTime24.ReleaseBuffer();
}
void CNewWeekDecisionDlg::GetInfo3(void)
{
	CString str1=_T("星期三");
	pGetInfo3 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo3,sizeof(Decision));	
	memcpy(pGetInfo3->day,str1.GetBuffer(),6);
	str1.ReleaseBuffer();
	memcpy(pGetInfo3->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult11= m_week3BA.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_week3BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_week3EA.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_week3EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_week3BH.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_week3BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_week3EH.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_week1EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo3->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo3->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo3->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo3->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
}
void CNewWeekDecisionDlg::GetInfo4(void)
{
	CString str1=_T("星期四");
	pGetInfo4 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo4,sizeof(Decision));	
	memcpy(pGetInfo4->day,str1.GetBuffer(),6);
	str1.ReleaseBuffer();
	memcpy(pGetInfo4->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult11= m_week4BA.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_week4BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_week4EA.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_week4EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_week4BH.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_week4BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_week4EH.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_week4EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo4->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo4->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo4->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo4->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
}
void CNewWeekDecisionDlg::GetInfo5(void)
{
	CString str1=_T("星期五");
	pGetInfo5 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo5,sizeof(Decision));	
	memcpy(pGetInfo5->day,str1.GetBuffer(),6);
	str1.ReleaseBuffer();
	memcpy(pGetInfo5->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult11= m_week5BA.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_week5BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_week5EA.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_week5EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_week5BH.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_week5BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_week5EH.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_week5EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo5->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo5->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo5->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo5->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
}
void CNewWeekDecisionDlg::GetInfo6(void)
{
	CString str1=_T("星期六");
	pGetInfo6 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo6,sizeof(Decision));	
	memcpy(pGetInfo6->day,str1.GetBuffer(),6);
	str1.ReleaseBuffer();
	memcpy(pGetInfo6->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult11= m_week6BA.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_week6BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_week6EA.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_week6EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_week6BH.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_week6BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_week6EH.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_week6EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo6->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo6->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo6->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo6->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
}
void CNewWeekDecisionDlg::GetInfo7(void)
{
	CString str1=_T("星期日");
	pGetInfo7 = (Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo7,sizeof(Decision));	
	memcpy(pGetInfo7->day,str1.GetBuffer(),6);
	str1.ReleaseBuffer();
	memcpy(pGetInfo7->name,m_weekName.GetBuffer(),m_weekName.GetLength()*sizeof(TCHAR));
	m_weekName.ReleaseBuffer();
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult11= m_week7BA.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_week7BA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_week7EA.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_week7EA.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_week7BH.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_week7BH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_week7EH.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_week7EH.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo7->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo7->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo7->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo7->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
}