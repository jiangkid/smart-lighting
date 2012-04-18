// DoGWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "DoGWorkDlg.h"
#include <string>

// CDoGWorkDlg dialog

IMPLEMENT_DYNAMIC(CDoGWorkDlg, CDialog)

CDoGWorkDlg::CDoGWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDoGWorkDlg::IDD, pParent)
	, m_GName(_T(""))
	, m_GID(_T(""))
{

}

CDoGWorkDlg::~CDoGWorkDlg()
{
}

void CDoGWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GName, m_GName);
	DDX_Text(pDX, IDC_GID, m_GID);
}


BEGIN_MESSAGE_MAP(CDoGWorkDlg, CDialog)
END_MESSAGE_MAP()


// CDoGWorkDlg message handlers

BOOL CDoGWorkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_GName = theApp.m_pFileView->m_szTreeName;
	m_GID= theApp.m_pFileView->m_szTreeID;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDoGWorkDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialog::OnCancel();
}

void CDoGWorkDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	this->~CDoGWorkDlg();
	free(this);
	CDialog::PostNcDestroy();
}
