// MapCtrlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MapCtrlDlg.h"


// CMapCtrlDlg dialog

IMPLEMENT_DYNAMIC(CMapCtrlDlg, CDialog)

CMapCtrlDlg::CMapCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapCtrlDlg::IDD, pParent)
{

}

CMapCtrlDlg::~CMapCtrlDlg()
{
}

void CMapCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_MapList);
}


BEGIN_MESSAGE_MAP(CMapCtrlDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CGPRSLocaInfomation::OnNMDblclkListTerminal)
END_MESSAGE_MAP()


// CMapCtrlDlg message handlers

BOOL CMapCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	theApp.m_pMapCtrlDlg=this;
	m_MapList.SetHeadings("�ն���ID,80;�ն�������,120;�ն��䰲װ�ص�,120;������,120;��װʱ��,120;");
	m_MapList.SetGridLines(true);
	return TRUE;  
	 
}
void CMapCtrlDlg::OnNMDblclkListTerminal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	strID=_T("");
	strName=_T("");
	if(pNMItemActivate->iItem==-1)
	{
		return;
	}
	else
	{	
		strID=m_MapList.GetItemText(pNMItemActivate->iItem,0);
		strName=m_MapList.GetItemText(pNMItemActivate->iItem,1);  
		CMapViewDlg dlg;
		dlg.DoModal();
	}

	*pResult = 0;
}
