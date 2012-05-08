// JWSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "JWSetDlg.h"


// CJWSetDlg dialog

IMPLEMENT_DYNAMIC(CJWSetDlg, CDialog)

CJWSetDlg::CJWSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJWSetDlg::IDD, pParent)
	, strLA(_T(""))
	, strLO(_T(""))
{

}

CJWSetDlg::~CJWSetDlg()
{
}

void CJWSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, strLA);
	DDX_Text(pDX, IDC_EDIT3, strLO);
}


BEGIN_MESSAGE_MAP(CJWSetDlg, CDialog)
	 
	ON_BN_CLICKED(IDOK, &CJWSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CJWSetDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CJWSetDlg message handlers

 

void CJWSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

BOOL CJWSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pJWSet = this;

	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CJWSetDlg::ShowJW(void)
{
	for (int n(0);n<theApp.nGeshu;n++)
	{
		CString str = _T("");
		for (int i(0);i<2;i++)
		{
			str+=theApp.m_MapInfo[n].GID[i];
		}
		for (int m(0);m<10;m++)
		{
			strLO+=theApp.m_MapInfo[n].Latitude[m];
			strLA+=theApp.m_MapInfo[n].Longtitude[m];
		}
		if (theApp.m_where)
		{
			if (strcmp(theApp.m_pMapCtrlDlg->m_GGGGID,str)==0)
			{
				UpdateData(FALSE);
			}			
		}
		else
		{
			if (strcmp(theApp.m_pLocalInfoDlg->m_GGGGGGID,str)==0)
			{
				UpdateData(FALSE);
			}
		}
	}
}

void CJWSetDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CString str1;
	CString str2;
	GetDlgItemText(IDC_EDIT2,str1);
	GetDlgItemText(IDC_EDIT3,str2);
	float a = (float)strtod(str1,NULL);
	float b = (float)strtod(str2,NULL);
	char* sendbuff = (char*)malloc(sizeof(PointInfo)+2);
	ZeroMemory(sendbuff,sizeof(PointInfo)+2);
	char c[2]={'S',0x43};
	memcpy(sendbuff,c,2);
	PointInfo* pGetInfo = (PointInfo*)malloc(sizeof(PointInfo));
	ZeroMemory(pGetInfo,sizeof(PointInfo));
	memcpy(&pGetInfo->GID[0],theApp.m_pMapCtrlDlg->strID.GetBuffer(2),2);
	//memcpy(&pGetInfo->GID[1],theApp.m_pMapCtrlDlg->strID.GetBuffer(1),1);
	theApp.m_pMapCtrlDlg->strID.ReleaseBuffer(4);
	//theApp.m_pMapCtrlDlg->strID.ReleaseBuffer(1);
// 	pGetInfo->GID[0]='0';
// 	pGetInfo->GID[1]='1';
	pGetInfo->Longtitude=a*100;
	pGetInfo->Latitude=b*100;
	memcpy(pGetInfo->LongtitudeSave,str1.GetBuffer(),10);
	memcpy(pGetInfo->LatitudeSave,str2.GetBuffer(),10);
	memcpy(sendbuff+2,pGetInfo,sizeof(PointInfo));
	send(theApp.m_ConnectSock,sendbuff,sizeof(PointInfo)+2,0);
	free(sendbuff);
	free(pGetInfo);
	AfxMessageBox(_T("信息已经发送"));
}

void CJWSetDlg::ShowDesitionStatus(StrategyInfo* nItem)
{
	if (nItem->sendtrue)
	{
		SetDlgItemText(IDC_EDIT1,_T("已应用"));
	}
	else
	{
		SetDlgItemText(IDC_EDIT1,_T("未应用"));
	}
}
