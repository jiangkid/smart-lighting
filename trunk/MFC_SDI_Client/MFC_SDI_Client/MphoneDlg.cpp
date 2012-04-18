// MphoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MphoneDlg.h"


// CMphoneDlg dialog

IMPLEMENT_DYNAMIC(CMphoneDlg, CDialog)

CMphoneDlg::CMphoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMphoneDlg::IDD, pParent)
	, m_strName(_T(""))
	, m_password(_T(""))
	, m_strphone(_T(""))
{

}

CMphoneDlg::~CMphoneDlg()
{
}

void CMphoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OLDN, m_strName);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_phonenum, m_strphone);
}


BEGIN_MESSAGE_MAP(CMphoneDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_Y, &CMphoneDlg::OnBnClickedBtnY)
END_MESSAGE_MAP()


// CMphoneDlg message handlers

void CMphoneDlg::OnBnClickedBtnY()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CString str=_T("");
	if (m_password==_T(""))
	{
		AfxMessageBox(_T("密码不能为空"));
		return;
	}
	if (m_strphone==_T(""))
	{
		AfxMessageBox(_T("电话号码不能为空"));
		return;
	}
	else 
		if(userInfo[0].Idetify==0X30)
		{
			str.Format("M40%s+%s+%s#",m_strName,m_password,m_strphone);
		}
		else
			if (userInfo[0].Idetify==0X31)
			{
				str.Format("M41%s+%s+%s#",m_strName,m_password,m_strphone);
			}
		send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
		str.ReleaseBuffer();
}

BOOL CMphoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	for (int i(0);i<20;i++)
	{
		m_strName+=userInfo[0].UserName[i];
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
