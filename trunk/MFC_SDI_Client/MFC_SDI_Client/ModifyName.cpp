// ModifyName.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ModifyName.h"


// CModifyName dialog

IMPLEMENT_DYNAMIC(CModifyName, CDialog)

CModifyName::CModifyName(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyName::IDD, pParent)
	, m_strOldName(_T(""))
	, m_passwold(_T(""))
	, m_newName(_T(""))
{

}

CModifyName::~CModifyName()
{
}

void CModifyName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OldName, m_strOldName);
	DDX_Text(pDX, IDC_PASS, m_passwold);
	DDX_Text(pDX, IDC_NewName, m_newName);
}


BEGIN_MESSAGE_MAP(CModifyName, CDialog)
	ON_BN_CLICKED(IDC_BTN_YES, &CModifyName::OnBnClickedBtnYes)
END_MESSAGE_MAP()


// CModifyName message handlers

void CModifyName::OnBnClickedBtnYes()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CString str=_T("");
	if (m_newName==_T(""))
	{
		AfxMessageBox(_T("新用户名不能为空"));
		return;
	}
	if (m_passwold==_T(""))
	{
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	else 
		if(userInfo[0].Idetify==0X30)
		{
			str.Format("M30%s+%s+%s#",m_strOldName,m_passwold,m_newName);
		}
		else
			if (userInfo[0].Idetify==0X31)
			{
				str.Format("M31%s+%s+%s#",m_strOldName,m_passwold,m_newName);
			}
	send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
	str.ReleaseBuffer();
}

BOOL CModifyName::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pModifyNameDlg=this;
	// TODO:  Add extra initialization here
	for (int i(0);i<20;i++)
	{
		m_strOldName+=userInfo[0].UserName[i];
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CModifyName::ChangeName(void)
{
	ZeroMemory(userInfo[0].UserName,10);
	memcpy(userInfo[0].UserName,m_newName.GetBuffer(),10);
	ClearBroad();
}

void CModifyName::ClearBroad(void)
{
	//m_newName=_T("");
	//m_passwold=_T("");
	//UpdateData(FALSE);
	SetDlgItemText(IDC_PASS,_T(""));
	SetDlgItemText(IDC_NewName,_T(""));
}
