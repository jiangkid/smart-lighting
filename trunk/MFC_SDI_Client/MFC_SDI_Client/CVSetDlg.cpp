// CVSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "CVSetDlg.h"


// CCVSetDlg dialog

IMPLEMENT_DYNAMIC(CCVSetDlg, CDialog)

CCVSetDlg::CCVSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCVSetDlg::IDD, pParent)
{

}

CCVSetDlg::~CCVSetDlg()
{
}

void CCVSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCVSetDlg, CDialog)
END_MESSAGE_MAP()


// CCVSetDlg message handlers
