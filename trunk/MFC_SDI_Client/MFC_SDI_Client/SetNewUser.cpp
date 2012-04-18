// SetNewUser.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "SetNewUser.h"


// CSetNewUser dialog

IMPLEMENT_DYNAMIC(CSetNewUser, CDialog)

CSetNewUser::CSetNewUser(CWnd* pParent /*=NULL*/)
	: CDialog(CSetNewUser::IDD, pParent)
	, m_strAreaName(_T(""))
{

}

CSetNewUser::~CSetNewUser()
{
}

void CSetNewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AREA_NAME, m_strAreaName);
}


BEGIN_MESSAGE_MAP(CSetNewUser, CDialog)
	ON_BN_CLICKED(IDOK, &CSetNewUser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetNewUser::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSetNewUser message handlers
BOOL CSetNewUser::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;
}
void CSetNewUser::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CMFC_SDI_ClientApp* pTheApp =(CMFC_SDI_ClientApp*)AfxGetApp();
	CString strName;
	CString strPassWord1;
	CString strPassWord2;
	CString strUserInfo;
	CString strTelNum;
	char stridentify[3]="C";
	GetDlgItemText(IDC_UserName,strName);
	GetDlgItemText(IDC_FPassWord,strPassWord1);
	GetDlgItemText(IDC_SPassWord,strPassWord2);
	GetDlgItemText(IDC_TelNum,strTelNum);
	if (strcmp(strPassWord1,strPassWord2)!=0)
	{
		AfxMessageBox("2次密码不一样，请确认");
	}
	else
	{
		if (strTelNum.GetLength()<7||strTelNum.GetLength()>11)
		{
			AfxMessageBox(_T("您输入的电话号码格式不正确，请重新输入！"));
		}
		else
		{
			char *a="1";
			strcat_s(stridentify,a);		 
			strUserInfo.Format(_T("%s+%s+%s+%s+%s#"),stridentify,strName,strPassWord1,strTelNum,m_strAreaName);
			int iSend =send(pTheApp->m_ConnectSock,(char*)strUserInfo.GetBuffer(),
				strUserInfo.GetLength()*sizeof(TCHAR),0);
			strUserInfo.ReleaseBuffer();
			AfxMessageBox(_T("信息已发送"));
		}
	}
}
void CSetNewUser::OnCbnSelchangeCombo1()
{
	 if (!IsBegin)
	 {
		 return;
	 }
}

void CSetNewUser::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
