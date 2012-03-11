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
BOOL CUserCtrDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE||pMsg->wParam==VK_RETURN)
		{
			return false;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CUserCtrDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	this->~CUserCtrDlg();
	free(this);
	CDialog::PostNcDestroy();
}

void CUserCtrDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialog::OnCancel();
}
