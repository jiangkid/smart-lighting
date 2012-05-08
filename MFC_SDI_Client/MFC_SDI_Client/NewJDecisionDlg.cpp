// NewJDecisionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "NewJDecisionDlg.h"
#include "ReportCtrl.h"

// CNewJDecisionDlg dialog

IMPLEMENT_DYNAMIC(CNewJDecisionDlg, CDialog)

CNewJDecisionDlg::CNewJDecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewJDecisionDlg::IDD, pParent)
	, m_Tyepe(_T(""))
	, m_NItem(0)
	, m_JName(_T(""))
{

}

CNewJDecisionDlg::~CNewJDecisionDlg()
{
}

void CNewJDecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Type, m_Tyepe);
	DDX_Control(pDX, IDC_DATA, m_Data);
	DDX_Control(pDX, IDC_LIST_DATA, m_List);
	DDX_Control(pDX, IDC_DATABA, m_ba);
	DDX_Control(pDX, IDC_DATAEA, m_ea);
	//DDX_Control(pDX, IDC_DATABH, m_bh);
	//DDX_Control(pDX, IDC_DATAEH, m_eh);
	DDX_Text(pDX, IDC_JName, m_JName);
}


BEGIN_MESSAGE_MAP(CNewJDecisionDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_D, &CNewJDecisionDlg::OnBnClickedBtnD)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DATA, &CNewJDecisionDlg::OnNMRClickListData)
	ON_COMMAND(ID_32896, &CNewJDecisionDlg::On32896)
	ON_BN_CLICKED(IDC_BTNC, &CNewJDecisionDlg::OnBnClickedBtnc)
	ON_BN_CLICKED(IDC_BTNSS, &CNewJDecisionDlg::OnBnClickedBtnss)
END_MESSAGE_MAP()


// CNewJDecisionDlg message handlers
static char nCount[1];
void CNewJDecisionDlg::OnBnClickedBtnD()
{
	// TODO: Add your control notification handler code here
 	UpdateData(true);
 	CString strTime1=_T("");
 	CTime m_GTimeBegin;
 	DWORD dwResult1= m_Data.GetTime(m_GTimeBegin);
 	if (dwResult1== GDT_VALID)
 	{
 		if ((m_Data.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
 			strTime1 = m_GTimeBegin.Format(_T("%X"));
 		else
 			strTime1.Format("%d-%d-%d",m_GTimeBegin.GetYear(),
 			m_GTimeBegin.GetMonth(),m_GTimeBegin.GetDay());
 	}
 	nCount[0]++;
 	int i=nCount[0];
 	m_List.InsertItem(i-1,strTime1);
}

BOOL CNewJDecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Tyepe=_T("节假日策略");
	UpdateData(FALSE);
	// TODO:  Add extra initialization here
	m_Data.SetFormat("yyyy-M-d");
	m_List.SetHeadings("已选日期,120");
	m_List.SetGridLines(TRUE);
	nCount[0]=0x00;
	m_ba.SetFormat("HH:mm");
	//m_bh.SetFormat("HH:mm");
	m_ea.SetFormat("HH:mm");
	//m_eh.SetFormat("HH:mm");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNewJDecisionDlg::OnNMRClickListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_NItem=pNMItemActivate->iItem;
	CMenu menu, *pSubMenu; //定义下面要用到的cmenu对象
	menu.LoadMenu(IDR_POPMENU); //装载自定义的右键菜单
	pSubMenu = menu.GetSubMenu(1); //获取第一个弹出菜单 
	CPoint oPoint; //定义一个用于确定光标位置的位置
	GetCursorPos(&oPoint); //获取当前光标的位置 
	//在指定位置显示弹出菜单
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this); 
	*pResult = 0;
}

void CNewJDecisionDlg::On32896()
{
	// TODO: Add your command handler code here
	m_List.DeleteItem(m_NItem);
	nCount[0]--;
}

void CNewJDecisionDlg::OnBnClickedBtnc()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CNewJDecisionDlg::OnBnClickedBtnss()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if (nCount[0]==0x00)
	{
		AfxMessageBox(_T("请选择节假日日期"));
		return;
	}
	if (m_JName==_T(""))
	{
		AfxMessageBox(_T("名称不能为空"));
		return;
	}
	int i = nCount[0];
	CString strTime11=_T("");
	CString strTime12=_T("");
	//CString strTime13=_T("");
	//CString strTime14=_T("");
	char* c=(char*)malloc(DLONG*i+2);
	ZeroMemory(c,DLONG*i+2);
	HDR2 hdr2;
	hdr2.dataCheck[0]='F';
	hdr2.nRet[0]=nCount[0];
	memcpy(c,&hdr2,2);
	CTime m_GTimeBegin;
	DWORD dwResult11= m_ba.GetTime(m_GTimeBegin);
	if (dwResult11== GDT_VALID)
	{
		if ((m_ba.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
			strTime11 = m_GTimeBegin.Format(_T("%X"));
		else
			strTime11.Format("%d:%d",m_GTimeBegin.GetHour(),
			m_GTimeBegin.GetMinute());
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
// 	CTime m_GTimeBeginHalf;
// 	DWORD dwResult13= m_bh.GetTime(m_GTimeBeginHalf);
// 	if (dwResult13== GDT_VALID)
// 	{
// 		if ((m_bh.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
// 			strTime13 = m_GTimeBeginHalf.Format(_T("%X"));
// 		else
// 			strTime13.Format("%d:%d",m_GTimeBeginHalf.GetHour(),
// 			m_GTimeBeginHalf.GetMinute());
// 	}
// 	CTime m_GTimeEndHalf;
// 	DWORD dwResult14= m_eh.GetTime(m_GTimeEndHalf);
// 	if (dwResult14 == GDT_VALID)
// 	{
// 		if ((m_eh.GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
// 			strTime14 = m_GTimeEndHalf.Format(_T("%X"));
// 		else
// 			strTime14.Format("%d:%d",m_GTimeEndHalf.GetHour(),
// 			m_GTimeEndHalf.GetMinute());
// 	}
	for (int n(0);n<i;n++)
	{
		Decision* pGetInfo = (Decision*)malloc(DLONG);
		ZeroMemory(pGetInfo,DLONG);
		CString str=_T("");
		str=m_List.GetItemText(n,0);
		memcpy(pGetInfo->day,str.GetBuffer(),str.GetLength()*sizeof(TCHAR));
		str.ReleaseBuffer();
		memcpy(pGetInfo->name,m_JName.GetBuffer(),m_JName.GetLength()*sizeof(TCHAR));
		m_JName.ReleaseBuffer();
		memcpy(pGetInfo->openTime,strTime11.GetBuffer(),5);
		memcpy(pGetInfo->closeTime,strTime12.GetBuffer(),5);
		//memcpy(pGetInfo->Half_openTime,strTime13.GetBuffer(),5);
		//memcpy(pGetInfo->Half_closeTime,strTime14.GetBuffer(),5);
		strTime11.ReleaseBuffer();
		strTime12.ReleaseBuffer();
		//strTime13.ReleaseBuffer();
		//strTime14.ReleaseBuffer();
		pGetInfo->strategyType[0]='J';
		memcpy(c+2+DLONG*n,pGetInfo,DLONG);
		free(pGetInfo);
	}
	send(theApp.m_ConnectSock,c,DLONG*i+2,0);
	free(c);
}