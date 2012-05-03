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
