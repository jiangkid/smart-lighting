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
	, str1(_T(""))
{

}

CSetNewUser::~CSetNewUser()
{
}

void CSetNewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AREA_NAME, m_strAreaName);
	DDX_Control(pDX, IDC_LIST_GLobal, m_List);
	DDX_Control(pDX, IDC_LIST2, m_List1);
}


BEGIN_MESSAGE_MAP(CSetNewUser, CDialog)
	ON_BN_CLICKED(IDOK, &CSetNewUser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetNewUser::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetNewUser::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON10, &CSetNewUser::OnBnClickedButton10)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_GLobal, &CSetNewUser::OnNMDblclkListGlobal)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CSetNewUser::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_RADIO1, &CSetNewUser::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CSetNewUser::OnBnClickedRadio2)
	ON_BN_CLICKED(IDOK2, &CSetNewUser::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK3, &CSetNewUser::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &CSetNewUser::OnBnClickedOk4)
	ON_BN_CLICKED(IDC_RADIO3, &CSetNewUser::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CSetNewUser message handlers
BOOL CSetNewUser::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pSetNewUserDlg=this;
	m_List.SetHeadings("已有用户区域名,150");
	m_List.SetGridLines(TRUE);
	m_List1.SetHeadings("GID,80;绑定的Location,150");
	m_List1.SetGridLines(TRUE);
	EnableWindow(IDC_NAME2,false);
	EnableWindow(IDC_NAME3,false);
	EnableWindow(IDC_NAME4,false);
	EnableWindow(IDC_NAME5,false);
	EnableWindow(IDC_NAME6,false);
	EnableWindow(IDOK2,false);
	EnableWindow(IDOK3,false);
	EnableWindow(IDOK4,false);
	char c[2]={'S',0x28};
	send(theApp.m_ConnectSock,c,2,0);
	m_List.DeleteAllItems();
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

void CSetNewUser::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	m_List.DeleteAllItems();	
	char c[2]={'S',0x28};
	send(theApp.m_ConnectSock,c,2,0);
	
}

void CSetNewUser::FullFillList(int nItem, USERINFO* strLocationName)
{
	CString str = _T("");
	str = strLocationName->AreaName;
	m_List.InsertItem(nItem,str);
}

void CSetNewUser::FullFillListBind(int nItem, BIND* pGetInfo)
{
	CString str1=_T("");
	CString str2=_T("");
	for(int i(0);i<2;i++)
	{
		str1+=pGetInfo->GID[i];
	}
	for (int n(0);n<20;n++)
	{
		str2+=pGetInfo->GlobalArea[n];
	}
	
	m_List1.InsertItem(nItem,str1,str2);
}

void CSetNewUser::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here
	m_List1.DeleteAllItems();
	char cc[2]={'M',0x39};
	send(theApp.m_ConnectSock,cc,2,0);
}

void CSetNewUser::OnNMDblclkListGlobal(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	CString str=m_List.GetItemText(pNMItemActivate->iItem,0);
	SetDlgItemText(IDC_AREA_NAME,_T(""));
	SetDlgItemText(IDC_AREA_NAME,str);
	SetDlgItemText(IDC_NAME2,_T(""));
	SetDlgItemText(IDC_NAME2,str);
	SetDlgItemText(IDC_NAME6,_T(""));
	SetDlgItemText(IDC_NAME6,str);
	*pResult = 0;
}

void CSetNewUser::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CString str=m_List1.GetItemText(pNMItemActivate->iItem,0);
	str1=m_List1.GetItemText(pNMItemActivate->iItem,1);
	SetDlgItemText(IDC_NAME3,_T(""));
	SetDlgItemText(IDC_NAME4,_T(""));
	SetDlgItemText(IDC_NAME3,str);
	SetDlgItemText(IDC_NAME5,_T(""));
	SetDlgItemText(IDC_NAME5,str);
	SetDlgItemText(IDC_NAME4,str1);
	*pResult = 0;
}

BOOL CSetNewUser::EnableWindow(UINT uID, bool bEnable)
{
	return GetDlgItem(uID)->EnableWindow(bEnable);
}

void CSetNewUser::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	EnableWindow(IDC_NAME2,true);
	EnableWindow(IDC_NAME3,true);
	EnableWindow(IDC_NAME4,false);
	EnableWindow(IDC_NAME5,false);
	EnableWindow(IDOK2,true);
	EnableWindow(IDOK3,false);
	EnableWindow(IDC_NAME6,false);
	EnableWindow(IDOK4,false);
}

void CSetNewUser::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	EnableWindow(IDC_NAME2,false);
	EnableWindow(IDC_NAME3,false);
	EnableWindow(IDC_NAME4,true);
	EnableWindow(IDC_NAME5,true);
	EnableWindow(IDOK2,false);
	EnableWindow(IDOK3,true);
	EnableWindow(IDC_NAME6,false);
	EnableWindow(IDOK4,false);
}

void CSetNewUser::OnBnClickedOk2()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strGID=_T("");
	CString strLocation=_T("");
	GetDlgItemText(IDC_NAME2,strLocation);
	GetDlgItemText(IDC_NAME3,strGID);
 	if (str1!=_T(""))
 	{
 		AfxMessageBox(_T("GPRS和区域已经绑定,请先解除绑定"));
 		return;
 	}
 	else
 	{
		char c[2]={'S',0x40};
		CString str=c;
		str+=strGID;
		str+='+';
		str+=strLocation;
		str+="#";
		send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
		str.ReleaseBuffer();
	}
}

void CSetNewUser::OnBnClickedOk3()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strGID=_T("");
	CString strLocation=_T("");
	GetDlgItemText(IDC_NAME4,strLocation);
	GetDlgItemText(IDC_NAME5,strGID);
	if (str1==_T(""))
	{
		AfxMessageBox(_T("GPRS还没有被绑定,请先绑定"));
		return;
	}
	else
	{
		char c[2]={'S',0x41};
		CString str=c;
		str+=strGID;
		str+='+';
		str+=strLocation;
		str+="#";
		send(theApp.m_ConnectSock,str.GetBuffer(),str.GetLength(),0);
		str.ReleaseBuffer();
	}
}

void CSetNewUser::OnBnClickedOk4()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str=_T("") ; 
	GetDlgItemText(IDC_NAME6,str);
	if (str==_T(""))
	{
		AfxMessageBox(_T("请选择要删除的区域"));
	}
	else
	{
		CString str2=_T("");
		str2+='D';
		str2+='2';
		str2+=str;
		send(theApp.m_ConnectSock,str2.GetBuffer(),str2.GetLength(),0);
		str2.ReleaseBuffer();
	}
}

void CSetNewUser::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	EnableWindow(IDC_NAME2,false);
	EnableWindow(IDC_NAME3,false);
	EnableWindow(IDC_NAME4,false);
	EnableWindow(IDC_NAME5,false);
	EnableWindow(IDOK2,false);
	EnableWindow(IDOK3,false);
	EnableWindow(IDC_NAME6,true);
	EnableWindow(IDOK4,true);
}
