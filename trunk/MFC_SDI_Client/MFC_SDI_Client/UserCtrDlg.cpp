// UserCtrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "UserCtrDlg.h"


// CUserCtrDlg dialog

IMPLEMENT_DYNAMIC(CUserCtrDlg, CDialog)

CUserCtrDlg::CUserCtrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserCtrDlg::IDD, pParent)
	, m_UserName(_T(""))
	, m_strOldP(_T(""))
	, m_new1(_T(""))
	, m_new2(_T(""))
	, m_phone(_T(""))
	, m_areaname(_T(""))
{

}

CUserCtrDlg::~CUserCtrDlg()
{
}

void CUserCtrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_List);
	DDX_Text(pDX, IDC_UESRNAME, m_UserName);
	DDX_Text(pDX, IDC_OLDP, m_strOldP);
	DDX_Text(pDX, IDC_NEWP1, m_new1);
	DDX_Text(pDX, IDC_NEWP2, m_new2);
	DDX_Text(pDX, IDC_PHONE, m_phone);
	DDX_Text(pDX, IDC_AREA, m_areaname);
}


BEGIN_MESSAGE_MAP(CUserCtrDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_USER, &CUserCtrDlg::OnNMDblclkListUser)
	ON_BN_CLICKED(IDC_BUTTON13, &CUserCtrDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_M, &CUserCtrDlg::OnBnClickedM)
	ON_BN_CLICKED(IDC_RENEW, &CUserCtrDlg::OnBnClickedRenew)
	ON_BN_CLICKED(IDC_RADIO1, &CUserCtrDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO4, &CUserCtrDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO5, &CUserCtrDlg::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_RADIO2, &CUserCtrDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CUserCtrDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO6, &CUserCtrDlg::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_DELETEUSER, &CUserCtrDlg::OnBnClickedDeleteuser)
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
// 	this->~CUserCtrDlg();
// 	free(this);
	CDialog::PostNcDestroy();
}

void CUserCtrDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//DestroyWindow();
	CDialog::OnCancel();
}

BOOL CUserCtrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pUserCtrl=this;
	m_List.SetHeadings("用户名,80;管理区域,80;手机号码,80;创建时间,120;");
	m_List.SetGridLines(TRUE);
	// TODO:  Add extra initialization here
	char c[2]={'M',0x31};
	send(theApp.m_ConnectSock,c,2,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CUserCtrDlg::ShowUserInfo(int nItem, MUserInfo* pGetInfo)
{
	CString strName=_T("");
	CString strArea=_T("");
	CString strPhone=_T("");
	CString strTime=_T("");
	for (int i(0);i<20;i++)
	{
		strTime+=pGetInfo->Time[i];
	}
	for (int n(0);n<20;n++)
	{
		strName+=pGetInfo->UserName[n];
	}
	for (int m(0);m<11;m++)
	{
		strPhone+=pGetInfo->PhoneNum[m];
	}
	for (int j(0);j<20;j++)
	{
		strArea+=pGetInfo->AreaName[j];
	}
	m_List.InsertItem(nItem,strName,strArea,strPhone,strTime);
}

void CUserCtrDlg::OnNMDblclkListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if (pNMItemActivate->iItem==-1)
	{
		return;
	}
	else
	{
		m_UserName=m_List.GetItemText(pNMItemActivate->iItem,0);
		m_phone=m_List.GetItemText(pNMItemActivate->iItem,2);
		m_areaname=m_List.GetItemText(pNMItemActivate->iItem,1);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CUserCtrDlg::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CUserCtrDlg::OnBnClickedM()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nRet = GetChenkRadio();
	switch(nRet)
	{
	case 1:
		{
			CString str=_T("");
			if (m_strOldP==_T(""))
			{
				AfxMessageBox(_T("密码不能为空"));
				return;
			}
			else
				if (strcmp(m_new1,m_new2)!=0)
				{
					AfxMessageBox(_T("2次密码不一样，请重新输入"));
					return;
				}
				else
					if (AfxMessageBox(_T("你确定要修改?"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
					{
						str.Format("M51%s+%s+%s#",m_UserName,m_strOldP,m_new1);
						send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
						str.ReleaseBuffer();
					}
		}
		break;
	case 2:
		{
			CString str=_T("");
			if (m_strOldP==_T(""))
			{
				AfxMessageBox(_T("密码不能为空"));
				return;
			}
			else
				if (AfxMessageBox(_T("你确定要修改?"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					str.Format("M61%s+%s+%s#",m_UserName,m_strOldP,m_phone);
					send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
					str.ReleaseBuffer();
				}
		}
		break;
	case 3:
		{
			CString str=_T("");
			if (m_strOldP==_T(""))
			{
				AfxMessageBox(_T("密码不能为空"));
				return;
			}
			else
				if (m_new1==_T("")||m_new2==_T("")||strcmp(m_new1,m_new2)!=0)
				{
					AfxMessageBox(_T("2次密码不一样，请重新输入"));
					return;
				}
				else 
					if (AfxMessageBox(_T("你确定要修改?"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
					{
						str.Format("M81%s+%s+%s+%s+%s#",m_UserName,m_strOldP,m_new1,m_phone,m_areaname);
						send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
						str.ReleaseBuffer();
					}
		}
		break;
	case 4:
		{
			CString str=_T("");
			if (m_strOldP==_T(""))
			{
				AfxMessageBox(_T("密码不能为空"));
				return;
			}
			else 
				if (AfxMessageBox(_T("你确定要修改?"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					str.Format("M71%s+%s+%s#",m_UserName,m_strOldP,m_areaname);
					send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
					str.ReleaseBuffer();
				}
		}
		break;
	default:
		AfxMessageBox(_T("选择操作类型"));
		break;
	}
}

void CUserCtrDlg::OnBnClickedRenew()
{
	// TODO: Add your control notification handler code here
	m_List.DeleteAllItems();
	char c[2]={'M',0x31};
	send(theApp.m_ConnectSock,c,2,0);

}

bool CUserCtrDlg::EnableWindow(UINT uID, bool bEnable)
{
	return GetDlgItem(uID)->EnableWindow(bEnable);
}

void CUserCtrDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO6,IDC_RADIO1);
	EnableWindow(IDC_PHONE,FALSE);
	EnableWindow(IDC_AREA,FALSE);
	EnableWindow(IDC_OLDP,true);
	EnableWindow(IDC_NEWP1,true);
	EnableWindow(IDC_NEWP2,true);
	EnableWindow(IDC_DELETEUSER,FALSE);
	EnableWindow(IDC_M,true);
}

void CUserCtrDlg::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO6,IDC_RADIO4);
	EnableWindow(IDC_PHONE,true);
	EnableWindow(IDC_AREA,FALSE);
	EnableWindow(IDC_OLDP,true);
	EnableWindow(IDC_NEWP1,FALSE);
	EnableWindow(IDC_NEWP2,FALSE);
	EnableWindow(IDC_DELETEUSER,FALSE);
	EnableWindow(IDC_M,true);
}

void CUserCtrDlg::OnBnClickedRadio5()
{
	// TODO: Add your control notification handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO6,IDC_RADIO5);
	EnableWindow(IDC_PHONE,true);
	EnableWindow(IDC_AREA,true);
	EnableWindow(IDC_OLDP,true);
	EnableWindow(IDC_NEWP1,true);
	EnableWindow(IDC_NEWP2,true);
	EnableWindow(IDC_DELETEUSER,FALSE);
	EnableWindow(IDC_M,true);
}

void CUserCtrDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO6,IDC_RADIO2);
	EnableWindow(IDC_PHONE,FALSE);
	EnableWindow(IDC_AREA,FALSE);
	EnableWindow(IDC_OLDP,true);
	EnableWindow(IDC_NEWP1,FALSE);
	EnableWindow(IDC_NEWP2,FALSE);
	EnableWindow(IDC_DELETEUSER,true);
	EnableWindow(IDC_M,FALSE);
}

void CUserCtrDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO6,IDC_RADIO3);
	CSetNewUser dlg;
	dlg.DoModal();
}

int CUserCtrDlg::GetChenkRadio(void)
{
	int nRet = GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO6);

	if (nRet==1084)
		return 1;
	if (nRet==1087)
		return 2;
	if (nRet==1088)
		return 3;
	if (nRet==1089)
		return 4;
	if (nRet==1085)
		return 5;
}

void CUserCtrDlg::OnBnClickedRadio6()
{
	// TODO: Add your control notification handler code here
	CheckRadioButton(IDC_RADIO1,IDC_RADIO6,IDC_RADIO6);
	EnableWindow(IDC_PHONE,FALSE);
	EnableWindow(IDC_AREA,true);
	EnableWindow(IDC_OLDP,true);
	EnableWindow(IDC_NEWP1,FALSE);
	EnableWindow(IDC_NEWP2,FALSE);
	EnableWindow(IDC_DELETEUSER,FALSE);
	EnableWindow(IDC_M,true);

}

void CUserCtrDlg::OnBnClickedDeleteuser()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nRet = GetChenkRadio();
	switch (nRet)
	{
	case 5:
		if (m_UserName==_T(""))
		{
			AfxMessageBox(_T("请选择要删除的用户"));
			return;
		}
		else 
			if(m_strOldP==_T(""))
			{
				AfxMessageBox(_T("请输入该用户的密码"));
			}
			else if (AfxMessageBox(_T("你确定要删除?"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{

				CString str=_T("");
				str.Format("D1%s+%s#",m_UserName,m_strOldP);
				send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
				str.ReleaseBuffer();
			}
			break;
	default:
		AfxMessageBox(_T("选择操作类型"));
		break;
	}
}
