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
	, m_status(_T(""))
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
	DDX_Text(pDX, IDC_RName2, m_status);
}


BEGIN_MESSAGE_MAP(CDoRWorkDlg, CDialog)
END_MESSAGE_MAP()


// CDoRWorkDlg message handlers

BOOL CDoRWorkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
 	m_RName = theApp.m_pRoadView->m_szRName;
 	RID = theApp.m_pRoadView->m_szLID;
	m_status = theApp.m_pRoadView->m_szGName;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDoRWorkDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialog::OnCancel();
}
void CDoRWorkDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	this->~CDoRWorkDlg();
	free(this);
	CDialog::PostNcDestroy();
}