// SpacialDecisionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "SpacialDecisionDlg.h"


// CSpacialDecisionDlg dialog

IMPLEMENT_DYNAMIC(CSpacialDecisionDlg, CDialog)

CSpacialDecisionDlg::CSpacialDecisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpacialDecisionDlg::IDD, pParent)
{

}

CSpacialDecisionDlg::~CSpacialDecisionDlg()
{
}

void CSpacialDecisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSpacialDecisionDlg, CDialog)
END_MESSAGE_MAP()


// CSpacialDecisionDlg message handlers

BOOL CSpacialDecisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
