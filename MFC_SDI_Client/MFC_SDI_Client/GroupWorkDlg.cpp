// GroupWorkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "GroupWorkDlg.h"


// CGroupWorkDlg dialog

IMPLEMENT_DYNAMIC(CGroupWorkDlg, CDialog)

CGroupWorkDlg::CGroupWorkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupWorkDlg::IDD, pParent)
	, m_GroupNum(0)
{

}

CGroupWorkDlg::~CGroupWorkDlg()
{
}

void CGroupWorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GroupNum, m_GroupNum);
	DDV_MinMaxInt(pDX, m_GroupNum, 1, 255);
}


BEGIN_MESSAGE_MAP(CGroupWorkDlg, CDialog)
	ON_BN_CLICKED(IDC_GroupOK, &CGroupWorkDlg::OnBnClickedGroupok)
END_MESSAGE_MAP()


// CGroupWorkDlg message handlers

void CGroupWorkDlg::OnBnClickedGroupok()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
}
