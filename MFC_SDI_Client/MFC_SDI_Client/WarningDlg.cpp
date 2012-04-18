// WarningDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "WarningDlg.h"
#include "ReportCtrl.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "ControlWaringDlg.h"
// CWarningDlg dialog

IMPLEMENT_DYNAMIC(CWarningDlg, CDialog)

CWarningDlg::CWarningDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWarningDlg::IDD, pParent)
	, warningpackCount(0)
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
	DDX_Control(pDX, IDC_COM_GID, m_TheGID);
}


BEGIN_MESSAGE_MAP(CWarningDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_GET, &CWarningDlg::OnBnClickedBtnGet)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_WARNING, &CWarningDlg::OnNMDblclkWarningDlg)
END_MESSAGE_MAP()


// CWarningDlg message handlers

BOOL CWarningDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pWarningInfoView=this;
	for (int i(0);i<m_InitInfo.GNum;i++)
	{
		CString str=_T("");
		str+=m_InitInfo.m_InitGInfo->GID;
		m_TheGID.InsertString(i,str);
	}
	m_TheGID.SetCurSel(-1);
	theApp.m_pWarningInfoView=this;
	m_DataBegin.SetFormat("yyyy-M-d");
	m_DataEnd.SetFormat("yyyy-M-d");
	m_ListWarning.SetHeadings
		(_T("时间,120;对应故障ID,120;对应故障位置,120;对应名称,120;对应故障类型,120;对应故障具体值,120;处理情况，120"));
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
	CString strID=_T("");
	CString strTime1=_T("");
	CString strTime2=_T("");
	warningpackCount=0;
	int nIndex(-1);
	nIndex=m_TheGID.GetCurSel();
	if (nIndex==-1)
	{
		AfxMessageBox(_T("请选择区域ID"));
		return;
	}
	m_TheGID.GetLBText(nIndex,strID);
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
	str+='X';
	str+=strID;
	str+='+';
	str+=strTime1;
	str+='+';
	str+=strTime2;
	str+='#';
	send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
}

void CWarningDlg::OneWarningToShow(WarningInfo* pWarning, int nRow)
{
	CString strID=_T("");
	CString strArea=_T("");
	CString strName=_T("");
	CString strType=_T("");
	CString strTime=_T("");
	float     nValue(0);
	CString strValue=_T("");
	for (int i(0);i<16;i++)
	{
		strID+=pWarning->ID[i];
	}
	for (int m(0);m<50;m++)
	{
		strArea+=pWarning->InstallName[m];
	}
	for (int s(0);s<20;s++)
	{
		strName+=pWarning->Name[s];
	}
	for (int h(0);h<19;h++)
	{
		strTime+=pWarning->WarningTime[h];
	}
	switch(pWarning->WarningType)
	{
	case 0x01:
		m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,MAINC,_T("0"));
		break;
	case 0x02:
		m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,ASSISTC,_T("0"));
		break;
	case 0x03:
		if (pWarning->Value[0]==0x01)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,LOSTAV,_T("0"));
		}
		if (pWarning->Value[0]==0x02)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,LOSTBV,_T("0"));
		}
		if (pWarning->Value[0]==0x03)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,LOSTCV,_T("0"));
		}
		break;
	case 0x04:
		nValue=pWarning->Value[2]*256+pWarning->Value[3];
		strValue.Format("%0.3f",nValue/1000);
		if (pWarning->Value[0]==0x01)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,HAV,strValue);
		}
		if (pWarning->Value[0]==0x02)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,HBV,strValue);
		}
		if (pWarning->Value[0]==0x03)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,HBV,strValue);
		}
		break;
	case 0x05:
		nValue=pWarning->Value[2]*256+pWarning->Value[3];
		strValue.Format("%0.3f",nValue/1000);
		if (pWarning->Value[0]==0x01)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,LAV,strValue);
		}
		if (pWarning->Value[0]==0x02)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,LBV,strValue);
		}
		if (pWarning->Value[0]==0x03)
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,LCV,strValue);
		}
		break;
	case 0x33:
		nValue=pWarning->Value[2]*256+pWarning->Value[3];
		strValue.Format("%0.3f",nValue/1000);
		if (pWarning->Value[1]==0x01)
		{
			CString str=_T("第一支路");
			CString strName1=_T("");
			strName1.Format("%s%s",strName,str);
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName1,ROADHC,strValue);
		}
		if (pWarning->Value[1]==0x02)
		{
			CString str=_T("第二支路");
			CString strName1=_T("");
			strName1.Format("%s%s",strName,str);
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName1,ROADHC,strValue);
		}
		if (pWarning->Value[1]==0x03)
		{
			CString str=_T("第三支路");
			CString strName1=_T("");
			strName1.Format("%s%s",strName,str);
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName1,ROADHC,strValue);
		}
		break;
	case 0x39:
		{
			m_ListWarning.InsertItem(nRow,strTime,strID,strArea,strName,GPRSERROR,strValue);
			break;
		}

	default:
		break;
	}	
}
void CWarningDlg::AllWarningToShow(int nWarningCount)
{
	m_ListWarning.DeleteAllItems();
	for(int i(0);i<nWarningCount;i++)
	{
		OneWarningToShow(&theApp.m_WarningInfo[i],i);
	}
}
void CWarningDlg::OnNMDblclkWarningDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem == -1) 
	{ 
		//AfxMessageBox(_T("123"));
		return; 
	} 
	else 
	{ 
		//获取所点击的行和列的例子。 
		//strText.Format("你点击了第%d行", pNMItemActivate->iItem); 
		CControlWaringDlg dlg;
		dlg.DoModal();

	} 
	*pResult = 0;
}