// NewSpcialDecisionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "NewSpcialDecisionDlg.h"


// CNewSpcialDecisionDlg dialog

IMPLEMENT_DYNAMIC(CNewSpcialDecisionDlg, CDialog)

CNewSpcialDecisionDlg::CNewSpcialDecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewSpcialDecisionDlg::IDD, pParent)
	, m_Type(_T(""))
{

}

CNewSpcialDecisionDlg::~CNewSpcialDecisionDlg()
{
}

void CNewSpcialDecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Type, m_Type);
	DDX_Control(pDX, IDC_SDATA, m_sd);
	DDX_Control(pDX, IDC_BATIME, m_ba);
	DDX_Control(pDX, IDC_BHTIME, m_bh);
	DDX_Control(pDX, IDC_EHTIME, m_eh);
	DDX_Control(pDX, IDC_EATIME, m_ea);
}


BEGIN_MESSAGE_MAP(CNewSpcialDecisionDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_QX, &CNewSpcialDecisionDlg::OnBnClickedBtnQx)
	ON_BN_CLICKED(IDC_BTN_BC, &CNewSpcialDecisionDlg::OnBnClickedBtnBc)
END_MESSAGE_MAP()


// CNewSpcialDecisionDlg message handlers

void CNewSpcialDecisionDlg::OnBnClickedBtnQx()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

BOOL CNewSpcialDecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Type=_T("ÌØÊâ²ßÂÔ");
	UpdateData(FALSE);
	m_sd.SetFormat("yyyy-M-d");
	m_ba.SetFormat("HH:mm");
	m_bh.SetFormat("HH:mm");
	m_eh.SetFormat("HH:mm");
	m_ea.SetFormat("HH:mm");
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewSpcialDecisionDlg::OnBnClickedBtnBc()
{
	// TODO: Add your control notification handler code here
	Decision*pGetInfo = (Decision*)malloc(DLONG);
	ZeroMemory(pGetInfo,DLONG);
	pGetInfo->strategyType[0]='T';
	UpdateData(true);
	CString strTime1=_T("");
	CString strTime11=_T("");
	CString strTime12=_T("");
	CString strTime13=_T("");
	CString strTime14=_T("");
	CTime m_GTimeBegin;
	DWORD dwResult1= m_sd.GetTime(m_GTimeBegin);
	if (dwResult1== GDT_VALID)
	{
		if ((m_sd.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime1 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime1.Format("%d-%d-%d",m_GTimeBegin.GetYear(),
			m_GTimeBegin.GetMonth(),m_GTimeBegin.GetDay());
	}
	memcpy(pGetInfo->day,strTime1.GetBuffer(),strTime1.GetLength()*sizeof(TCHAR));
	CTime m_GTimeBegin2;
	DWORD dwResult11= m_ba.GetTime(m_GTimeBegin2);
	if (dwResult11== GDT_VALID)
	{
		if ((m_ba.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin2.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin2.GetHour(),
			m_GTimeBegin2.GetMinute());
	}
	CTime m_GTimeEnd;
	DWORD dwResult12= m_ea.GetTime(m_GTimeEnd);
	if (dwResult12 == GDT_VALID)
	{
		if ((m_ea.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime12 = m_GTimeEnd.Format(_T("%X"));
		else
			strTime12.Format("%d:%d",m_GTimeEnd.GetHour(),
			m_GTimeEnd.GetMinute());
	}
	CTime m_GTimeBeginHalf;
	DWORD dwResult13= m_bh.GetTime(m_GTimeBeginHalf);
	if (dwResult13== GDT_VALID)
	{
		if ((m_bh.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
		else
			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
			m_GTimeBeginHalf.GetMinute());
	}
	CTime m_GTimeEndHalf;
	DWORD dwResult14= m_eh.GetTime(m_GTimeEndHalf);
	if (dwResult14 == GDT_VALID)
	{
		if ((m_eh.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
		else
			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
			m_GTimeEndHalf.GetMinute());
	}
	memcpy(pGetInfo->openTime,strTime11.GetBuffer(),5);
	memcpy(pGetInfo->closeTime,strTime12.GetBuffer(),5);
	memcpy(pGetInfo->Half_openTime,strTime13.GetBuffer(),5);
	memcpy(pGetInfo->Half_closeTime,strTime14.GetBuffer(),5);
	strTime11.ReleaseBuffer();
	strTime12.ReleaseBuffer();
	strTime13.ReleaseBuffer();
	strTime14.ReleaseBuffer();
	HDR2 hdr2;
	hdr2.dataCheck[0]='F';
	hdr2.nRet[0]=0x01;
	SendDecision(&hdr2,pGetInfo);
	free(pGetInfo);
}
