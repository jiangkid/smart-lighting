// DoRWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "DoRWorkDlg.h"


// CDoRWorkDlg dialog

IMPLEMENT_DYNAMIC(CDoRWorkDlg, CDialog)

CDoRWorkDlg::CDoRWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoRWorkDlg::IDD, pParent)
	, m_RName(_T(""))
	, RID(_T(""))
{

}

CDoRWorkDlg::~CDoRWorkDlg()
{
}

void CDoRWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RName, m_RName);
	DDX_Text(pDX, IDC_RID, RID);
}


BEGIN_MESSAGE_MAP(CDoRWorkDlg, CDialog)
END_MESSAGE_MAP()


// CDoRWorkDlg message handlers

BOOL CDoRWorkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_RName = theApp.m_pFileView->m_szTreeName;
	RID = theApp.m_pFileView->m_szTreeID;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
