// Modify.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "Modify.h"


// CModify dialog

IMPLEMENT_DYNAMIC(CModify, CDialog)

CModify::CModify(CWnd* pParent /*=NULL*/)
	: CDialog(CModify::IDD, pParent)
	, m_UserName(_T(""))
	, m_OldPassWord(_T(""))
	, m_FNewPassWord(_T(""))
	, m_SNewPassWord(_T(""))
{

}

CModify::~CModify()
{
}

void CModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CModify, CDialog)
	ON_BN_CLICKED(IDOK, &CModify::OnBnClickedOk)
END_MESSAGE_MAP()


// CModify message handlers

void CModify::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	UpdateData(true);
	CMFC_SDI_ClientApp* pTheApp =(CMFC_SDI_ClientApp*)AfxGetApp();
	CString str=_T("");

//	GetDlgItem(IDC_UserName)->GetWindowText(m_UserName);
	GetDlgItemText(IDC_UserName,m_UserName);
	GetDlgItemText(IDC_OLD_PW,m_OldPassWord);
	GetDlgItemText(IDC_FPassWord,m_FNewPassWord);
	GetDlgItemText(IDC_SPassWord,m_SNewPassWord);
	if (m_UserName==_T(""))
	{
		AfxMessageBox(_T("请输入正确用户名"));
		return;
	}
	else if (strcmp(m_FNewPassWord,m_SNewPassWord)!=0)
	{
		AfxMessageBox(_T("2次密码不一样，请重新输入"));
	}
	str.Format(_T("M1%s+%s+%s#"),m_UserName,m_OldPassWord,m_SNewPassWord);
	send(pTheApp->m_ConnectSock,(char*)str.GetBuffer(),str.GetLength()*sizeof(TCHAR),0);
	ClearBoard();
}

void CModify::ClearBoard(void)
{
	SetDlgItemText(IDC_FPassWord,_T(""));
	SetDlgItemText(IDC_OLD_PW,_T(""));
	SetDlgItemText(IDC_SPassWord,_T(""));
	SetDlgItemText(IDC_UserName,_T(""));
}
