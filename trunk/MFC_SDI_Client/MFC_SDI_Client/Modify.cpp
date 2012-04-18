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
	DDX_Text(pDX, IDC_UserName, m_UserName);
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
		AfxMessageBox(_T("��������ȷ�û���"));
		return;
	}
	else 
		if (m_FNewPassWord==_T("")||m_SNewPassWord==_T(""))
		{
			AfxMessageBox(_T("���벻��Ϊ��"));
			return;
		}
		else
			if (strcmp(m_FNewPassWord,m_SNewPassWord)!=0)
			{
				AfxMessageBox(_T("2�����벻һ��������������"));
				return;
			}
	if (userInfo[0].Idetify==0x30)
	{
		str.Format(_T("M20%s+%s+%s#"),m_UserName,m_OldPassWord,m_SNewPassWord);
	}
	else
	{
		str.Format(_T("M21%s+%s+%s#"),m_UserName,m_OldPassWord,m_SNewPassWord);
	}
	send(pTheApp->m_ConnectSock,(char*)str.GetBuffer(),str.GetLength()*sizeof(TCHAR),0);
	AfxMessageBox(_T("��Ϣ���ͳɹ�"));
	ClearBoard();
}

void CModify::ClearBoard(void)
{
	SetDlgItemText(IDC_FPassWord,_T(""));
	SetDlgItemText(IDC_OLD_PW,_T(""));
	SetDlgItemText(IDC_SPassWord,_T(""));
	//SetDlgItemText(IDC_UserName,_T(""));
}

BOOL CModify::OnInitDialog()
{
	CDialog::OnInitDialog();
	for (int i(0);i<20;i++)
	{
		m_UserName+=userInfo[0].UserName[i];
	}
	UpdateData(FALSE);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
