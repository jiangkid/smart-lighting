// NewDayDecisionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "NewDayDecisionDlg.h"
#include "NewJDecisionDlg.h"

// CNewDayDecisionDlg dialog

IMPLEMENT_DYNAMIC(CNewDayDecisionDlg, CDialog)

CNewDayDecisionDlg::CNewDayDecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDayDecisionDlg::IDD, pParent)
	, m_Type(_T(""))
	, strName(_T(""))
{

}

CNewDayDecisionDlg::~CNewDayDecisionDlg()
{
}

void CNewDayDecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Type, m_Type);
	DDX_Control(pDX, IDC_BEGINOPEN, m_BeginOpenAll);
	DDX_Control(pDX, IDC_ENDOPEN, m_EndOpenAll);
	DDX_Control(pDX, IDC_BEGINHALF, m_BeginHalf);
	DDX_Control(pDX, IDC_ENDHALF, m_EndHalf);
	DDX_Text(pDX, IDC_EDIT2, strName);
}


BEGIN_MESSAGE_MAP(CNewDayDecisionDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CNewDayDecisionDlg::OnBnClickedBtnCancel)
	ON_BN_CLICKED(IDC_BTN_SURE, &CNewDayDecisionDlg::OnBnClickedBtnSure)
END_MESSAGE_MAP()

// CNewDayDecisionDlg message handlers
BOOL CNewDayDecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_Type=_T("日策略");
	UpdateData(FALSE);
	m_BeginOpenAll.SetFormat("HH:mm");
	m_EndOpenAll.SetFormat("HH:mm");
	m_BeginHalf.SetFormat("HH:mm");
	m_EndHalf.SetFormat("HH:mm");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewDayDecisionDlg::OnBnClickedBtnCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CNewDayDecisionDlg::OnBnClickedBtnSure()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strTime1=_T("");
	CString strTime2=_T("");
	CString strTime3=_T("");
	CString strTime4=_T("");
	if (strName==_T(""))
	{
		AfxMessageBox(_T("决策名不能为空！"));
		return;
	}
	Decision* pGetInfo=(Decision*)malloc(sizeof(Decision));
	ZeroMemory(pGetInfo,sizeof(Decision));
	pGetInfo->strategyType[0]='R';
	memcpy(pGetInfo->name,strName.GetBuffer(),strName.GetLength()*sizeof(TCHAR));
	strName.ReleaseBuffer();
	CTime m_GTimeBegin;
	DWORD dwResult1= m_BeginOpenAll.GetTime(m_GTimeBegin);
	if (dwResult1==GDT_VALID)
	{
		if ((m_BeginOpenAll.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime1 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime1.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult2=m_EndOpenAll.GetTime(m_GTimeEnd);
	if (dwResult2 ==GDT_VALID)
	{
		if ((m_EndOpenAll.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime2 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime2.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult3= m_BeginHalf.GetTime(m_GTimeBeginHalf);
	if (dwResult3== GDT_VALID)
	{
		if ((m_BeginHalf.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime3 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime3.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult4= m_EndHalf.GetTime(m_GTimeEndHalf);
	if (dwResult4 == GDT_VALID)
	{
		if ((m_EndHalf.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime4 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime4.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo->openTime,strTime1.GetBuffer(),5);
	memcpy(pGetInfo->closeTime,strTime2.GetBuffer(),5);
	memcpy(pGetInfo->Half_openTime,strTime3.GetBuffer(),5);
	memcpy(pGetInfo->Half_closeTime,strTime4.GetBuffer(),5);
	strTime1.ReleaseBuffer();
	strTime2.ReleaseBuffer();
	strTime3.ReleaseBuffer();
	strTime4.ReleaseBuffer();
	HDR2 hdr2;
	SendDecision(&hdr2,pGetInfo);
	free(pGetInfo);
}
