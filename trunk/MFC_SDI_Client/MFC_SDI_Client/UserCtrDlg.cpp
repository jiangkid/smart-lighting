// UserCtrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "UserCtrDlg.h"


// CUserCtrDlg dialog

IMPLEMENT_DYNAMIC(CUserCtrDlg, CDialog)

CUserCtrDlg::CUserCtrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserCtrDlg::IDD, pParent)
{

}

CUserCtrDlg::~CUserCtrDlg()
{
}

void CUserCtrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUserCtrDlg, CDialog)
END_MESSAGE_MAP()


// CUserCtrDlg message handlers

void CUserCtrDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//CDialog::OnOK();
}
