// SetNewUser.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "SetNewUser.h"


// CSetNewUser dialog

IMPLEMENT_DYNAMIC(CSetNewUser, CDialog)

CSetNewUser::CSetNewUser(CWnd* pParent /*=NULL*/)
	: CDialog(CSetNewUser::IDD, pParent)
{

}

CSetNewUser::~CSetNewUser()
{
}

void CSetNewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetNewUser, CDialog)
	ON_BN_CLICKED(IDOK, &CSetNewUser::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetNewUser message handlers

void CSetNewUser::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CMFC_SDI_ClientApp* pTheApp =(CMFC_SDI_ClientApp*)AfxGetApp();
	CString strName;
	CString strPassWord1;
	CString strPassWord2;
	CString strUserInfo;
	GetDlgItemText(IDC_UserName,strName);
	GetDlgItemText(IDC_FPassWord,strPassWord1);
	GetDlgItemText(IDC_SPassWord,strPassWord2);
	if (strcmp(strPassWord1,strPassWord2)!=0)
	{
		AfxMessageBox("2次密码不一样，请确认");
	}
	else
	{
		strUserInfo.Format(_T("C1%s+%s#"),strName,strPassWord1);
		int iSend =send(pTheApp->m_ConnectSock,(char*)strUserInfo.GetBuffer(),
										strUserInfo.GetLength()*sizeof(TCHAR),0);
		strUserInfo.ReleaseBuffer();
	}
}
