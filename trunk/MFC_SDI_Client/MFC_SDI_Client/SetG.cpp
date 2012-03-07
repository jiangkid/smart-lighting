// SetG.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "SetG.h"


// CSetG dialog

IMPLEMENT_DYNAMIC(CSetG, CDialog)

CSetG::CSetG(CWnd* pParent /*=NULL*/)
	: CDialog(CSetG::IDD, pParent)
	, m_GName(_T(""))
	, m_TName(_T(""))
	, m_RName(_T(""))
	, m_LName(_T(""))
{

}

CSetG::~CSetG()
{
}

void CSetG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GName, m_GName);
	DDX_Control(pDX, IDC_COM_GID, m_ComG);
	DDX_Control(pDX, IDC_COM_TID, m_ComT);
	DDX_Control(pDX, IDC_COM_RID, m_ComR);
	DDX_Control(pDX, IDC_COM_LID, m_ComL);
	DDX_Text(pDX, IDC_TName, m_TName);
	DDX_Text(pDX, IDC_RName, m_RName);
	DDX_Text(pDX, IDC_LName, m_LName);
	DDX_Control(pDX, IDC_COM_UserName, m_UserNameSet);
}


BEGIN_MESSAGE_MAP(CSetG, CDialog)
	ON_BN_CLICKED(IDOK, &CSetG::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIOG, &CSetG::OnBnClickedRadiog)
	ON_BN_CLICKED(IDC_RADIOT, &CSetG::OnBnClickedRadiot)
	ON_BN_CLICKED(IDC_RADIOR, &CSetG::OnBnClickedRadior)
	ON_BN_CLICKED(IDC_RADIOL, &CSetG::OnBnClickedRadiol)
	ON_BN_CLICKED(IDC_BTN_GSET, &CSetG::OnBnClickedBtnGset)
	ON_BN_CLICKED(IDC_BTN_TSET, &CSetG::OnBnClickedBtnTset)
	ON_BN_CLICKED(IDC_BTN_RSET, &CSetG::OnBnClickedBtnRset)
	ON_BN_CLICKED(IDC_BTN_LSET, &CSetG::OnBnClickedBtnLset)
END_MESSAGE_MAP()


// CSetG message handlers

void CSetG::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();

}

bool CSetG::EnableWindow(UINT uID, bool bEnable)
{
	return GetDlgItem(uID)->EnableWindow(bEnable);
}

BOOL CSetG::OnInitDialog()
{
	CDialog::OnInitDialog();
	//CheckRadioButton(IDC_RADIOG,IDC_RADIOL,IDC_RADIOG);
	EnableWindow(IDC_GName,false);
	EnableWindow(IDC_COM_GID,false);
	EnableWindow(IDC_BTN_GSET,false);
	EnableWindow(IDC_TName,FALSE);
	EnableWindow(IDC_RName,FALSE);
	EnableWindow(IDC_LName,FALSE);
	EnableWindow(IDC_COM_TID,FALSE);
	EnableWindow(IDC_COM_RID,FALSE);
	EnableWindow(IDC_COM_LID,FALSE);
	EnableWindow(IDC_BTN_TSET,FALSE);
	EnableWindow(IDC_BTN_RSET,FALSE);
	EnableWindow(IDC_BTN_LSET,FALSE);
	m_ComG.SetCurSel(-1);
	m_ComL.SetCurSel(-1);
	m_ComR.SetCurSel(-1);
	m_ComT.SetCurSel(-1);
	m_UserNameSet.SetCurSel(-1);
	pTheApp =(CMFC_SDI_ClientApp*)AfxGetApp();
 	if (userInfo[0].Idetify==0x30)
 	{
 		EnableWindow(IDC_COM_UserName,TRUE);
 		char buff[2]={'S',0x29};
 		send(pTheApp->m_ConnectSock,buff,2,0);
 		//Sleep(100*pTheApp->m_NumInfo.UserNum);
		Sleep(1000);
		Sleep(100*pTheApp->m_NumInfo.UserNum);
 		for (int i=0;i<pTheApp->m_NumInfo.UserNum;i++)
 		{
			CString str1;
//			memcpy(str,userInfo[i+1].UserName+1,sizeof(userInfo[i+1].UserName)-1);
			str1.Format("%s",userInfo[i+1].UserName);
 			m_UserNameSet.InsertString(i,str1);
 		}
 	}
 	else if(userInfo[0].Idetify==0x31)
 	{
 		EnableWindow(IDC_COM_UserName,FALSE);
 	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CSetG::OnBnClickedRadiog()
{
	// TODO: Add your control notification handler code here
	m_ComG.ResetContent();
	CheckRadioButton(IDC_RADIOG,IDC_RADIOL,IDC_RADIOG);
/*	int nRetName=m_UserNameSet.GetCurSel();*/
// 	if(nRetName==-1)
// 	{
// 		AfxMessageBox("请选择要绑定的用户！");
// 		return;
// 	}
// 	CString strG=("");
// 	char buff[2]={'S',0x30};
// 	strG+=buff;
// 	strG+="+";
// 	strG+=userInfo[nRetName+1].UserName;
// 	strG+="#";
// 	send(pTheApp->m_ConnectSock,(char*)strG.GetBuffer(),strG.GetLength()*sizeof(TCHAR),0);
	char buff[2]={'S',0x30};
	send(pTheApp->m_ConnectSock,buff,2,0);
	Sleep(1000);
	for (int i=0;i<pTheApp->m_NumInfo.GNum;i++)
	{
		CString str;
		for (int n=0;n<2;n++)
		{
			str+=pTheApp->m_GInfo[i].GID[n];
		}
		
		m_ComG.InsertString(i,str);
		//m_ComG.InsertString(i,ShowMessage(pTheApp->m_GInfo[i].GID,8));
	}
	UpdateData(true);
	EnableWindow(IDC_GName,true);
	EnableWindow(IDC_BTN_GSET,true);
	EnableWindow(IDC_COM_GID,true);
	EnableWindow(IDC_TName,FALSE);
	EnableWindow(IDC_RName,FALSE);
	EnableWindow(IDC_LName,FALSE);
	EnableWindow(IDC_COM_TID,FALSE);
	EnableWindow(IDC_COM_RID,FALSE);
	EnableWindow(IDC_COM_LID,FALSE);
	EnableWindow(IDC_BTN_TSET,FALSE);
	EnableWindow(IDC_BTN_RSET,FALSE);
	EnableWindow(IDC_BTN_LSET,FALSE);

}
void CSetG::OnBnClickedRadiot()
{
	// TODO: Add your control notification handler code here
	m_ComT.ResetContent();
	CString strT=("");
	int nRetG=m_ComG.GetCurSel();
	if(nRetG==-1)
	{
		AfxMessageBox("请选择要绑定的区域！");
		((CButton*)GetDlgItem(IDC_RADIOT))->SetCheck(0);
		return;
	}
	char buff[3]={'S',0x31,'+'};
	strT+=buff;
	strT+=pTheApp->m_GInfo[nRetG].GID;
	strT+="#";
	send(pTheApp->m_ConnectSock,(char*)strT.GetBuffer(),strT.GetLength()*sizeof(TCHAR),0);
	//send(pTheApp->m_ConnectSock,buff,2,0);
	Sleep(1000);
	for (int i=0;i<pTheApp->m_NumInfo.TNum;i++)
	{
		CString str=_T("");
		for (int n=0;n<4;n++)
		{
			str+=pTheApp->m_TInfo[i].TID[n];
		}
		
		m_ComT.InsertString(i,str);
		//m_ComT.InsertString(i,ShowMessage(pTheApp->m_TInfo[i].TID,8));
	}
	CheckRadioButton(IDC_RADIOG,IDC_RADIOL,IDC_RADIOT);
	UpdateData(true);
	EnableWindow(IDC_GName,FALSE);
	EnableWindow(IDC_TName,true);
	EnableWindow(IDC_RName,FALSE);
	EnableWindow(IDC_LName,FALSE);	
	EnableWindow(IDC_COM_GID,FALSE);
	EnableWindow(IDC_COM_TID,TRUE);
	EnableWindow(IDC_COM_RID,FALSE);
	EnableWindow(IDC_COM_LID,FALSE);
	EnableWindow(IDC_BTN_GSET,FALSE);
	EnableWindow(IDC_BTN_TSET,TRUE);
	EnableWindow(IDC_BTN_RSET,FALSE);
	EnableWindow(IDC_BTN_LSET,FALSE);
	
}

void CSetG::OnBnClickedRadior()
{
	// TODO: Add your control notification handler code here
	m_ComR.ResetContent();
	int nRetT=m_ComT.GetCurSel();
	if(nRetT==-1)
	{
		AfxMessageBox("请选择要绑定的终端！");
		((CButton*)GetDlgItem(IDC_RADIOR))->SetCheck(0);
		return;
	}
	CString strR=("");
	char buff[3]={'S',0x32,'+'};
	strR+=buff;
	strR+=pTheApp->m_TInfo[nRetT].TID;
	strR+="#";
	send(pTheApp->m_ConnectSock,(char*)strR.GetBuffer(),strR.GetLength()*sizeof(TCHAR),0);
	Sleep(1000);
	for (int i=0;i<pTheApp->m_NumInfo.RNum;i++)
	{
		CString str=_T("");
		for (int n=0;n<6;n++)
		{
			str+=pTheApp->m_RInfo[i].RID[n];
		}
		m_ComR.InsertString(i,str);
	}
	CheckRadioButton(IDC_RADIOG,IDC_RADIOL,IDC_RADIOR);
	UpdateData(true);
	EnableWindow(IDC_GName,FALSE);
	EnableWindow(IDC_TName,FALSE);
	EnableWindow(IDC_RName,true);
	EnableWindow(IDC_LName,FALSE);	
	EnableWindow(IDC_COM_GID,FALSE);
	EnableWindow(IDC_COM_TID,FALSE);
	EnableWindow(IDC_COM_RID,true);
	EnableWindow(IDC_COM_LID,FALSE);
	EnableWindow(IDC_BTN_GSET,FALSE);
	EnableWindow(IDC_BTN_TSET,FALSE);
	EnableWindow(IDC_BTN_RSET,true);
	EnableWindow(IDC_BTN_LSET,FALSE);
}

void CSetG::OnBnClickedRadiol()
{
	// TODO: Add your control notification handler code here
	m_ComL.ResetContent();
	int nRetR = m_ComR.GetCurSel();
	if (nRetR==-1)
	{
		AfxMessageBox("请选择要绑定的支路");
		((CButton*)GetDlgItem(IDC_RADIOL))->SetCheck(0);
		return;
	}
	CString strL=("");
	char buff[3]={'S',0x33,'+'};
	strL+=buff;
	strL+=pTheApp->m_RInfo[nRetR].RID;
	strL+="#";
	send(pTheApp->m_ConnectSock,(char*)strL.GetBuffer(),strL.GetLength()*sizeof(TCHAR),0);
	Sleep(1000);
	for (int i=0;i<pTheApp->m_NumInfo.LNum;i++)
	{
		CString str=_T("");
		for (int n=0;n<16;n++)
		{
			str+=pTheApp->m_LInfo[i].LID[n];
		}
		m_ComL.InsertString(i,str);
	}
	CheckRadioButton(IDC_RADIOG,IDC_RADIOL,IDC_RADIOL);
	UpdateData(true);
	EnableWindow(IDC_GName,FALSE);
	EnableWindow(IDC_TName,FALSE);
	EnableWindow(IDC_RName,FALSE);
	EnableWindow(IDC_LName,true);	
	EnableWindow(IDC_COM_GID,FALSE);
	EnableWindow(IDC_COM_TID,FALSE);
	EnableWindow(IDC_COM_RID,FALSE);
	EnableWindow(IDC_COM_LID,true);
	EnableWindow(IDC_BTN_GSET,FALSE);
	EnableWindow(IDC_BTN_TSET,FALSE);
	EnableWindow(IDC_BTN_RSET,FALSE);
	EnableWindow(IDC_BTN_LSET,true);
}

CString CSetG::ShowMessage(char* pszMessage, int nLength)
{
	CString strShow=_T("");
	LPTSTR p;
	TCHAR szText[300];
	ZeroMemory(szText, 300);
	p = szText;
	for (int i = 0; i< nLength; i++)
	{
		p+= wsprintf(p, TEXT("%.2X "), pszMessage[i]);  //这部分为关键部分
	}
	strShow.Format(_T("%s"), szText);
	return strShow;
}

void CSetG::OnBnClickedBtnGset()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int nRet(-1),nRetUser(-1);
	CString str=_T("");
	char buff[2]={'S',0x39};
	str+=buff[0];
	str+=buff[1];
	nRet=m_ComG.GetCurSel();
	nRetUser=m_UserNameSet.GetCurSel();
	if (m_GName=="" || nRet==-1)
	{
		AfxMessageBox(_T("区域名或ID不能不能为空，请确认！"));
	}
	else 
	{
		str+=pTheApp->m_GInfo[nRet].GID[0];
		str+=pTheApp->m_GInfo[nRet].GID[1];
		str+="+";
		str+=m_GName;
		str+="+";
		str+=userInfo[nRetUser+1].UserName;
		str+="#";
		send(pTheApp->m_ConnectSock,(char*)str.GetBuffer(),str.GetLength()*sizeof(TCHAR),0);
		Sleep(1000);
	}
}

void CSetG::OnBnClickedBtnTset()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int nRetT(-1);
	CString str=_T("");
	char buff[2]={'S',0x38};
	str+=buff[0];
	str+=buff[1];
	nRetT=m_ComT.GetCurSel();
	//nRetUser=m_UserNameSet.GetCurSel();
	if (nRetT==-1 || m_TName=="")
	{
		AfxMessageBox(_T("输入的T名字和TID不能不能为空，请确认！"));
	}
	else 
	{
		str+=pTheApp->m_TInfo[nRetT].TID[0];
		str+=pTheApp->m_TInfo[nRetT].TID[1];
		str+=pTheApp->m_TInfo[nRetT].TID[2];
		str+=pTheApp->m_TInfo[nRetT].TID[3];
		str+="+";
		str+=m_TName;
		str+="#";
		send(pTheApp->m_ConnectSock,(char*)str.GetBuffer(),str.GetLength()*sizeof(TCHAR),0);
		Sleep(1000);
	}
}

void CSetG::OnBnClickedBtnRset()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int nRetR(-1);
	CString str=_T("");
	char buff[2]={'S',0x37};
	str+=buff[0];
	str+=buff[1];
	nRetR=m_ComR.GetCurSel();
	//nRetUser=m_UserNameSet.GetCurSel();
	if (nRetR==-1 || m_RName=="")
	{
		AfxMessageBox(_T("输入的R名字和RID不能不能为空，请确认！"));
	}
	else 
	{
		str+=pTheApp->m_RInfo[nRetR].RID[0];
		str+=pTheApp->m_RInfo[nRetR].RID[1];
		str+=pTheApp->m_RInfo[nRetR].RID[2];
		str+=pTheApp->m_RInfo[nRetR].RID[3];
		str+=pTheApp->m_RInfo[nRetR].RID[4];
		str+=pTheApp->m_RInfo[nRetR].RID[5];
		str+="+";
		str+=m_RName;
		str+="#";
		send(pTheApp->m_ConnectSock,(char*)str.GetBuffer(),str.GetLength()*sizeof(TCHAR),0);
		Sleep(1000);
	}
}

void CSetG::OnBnClickedBtnLset()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	int nRetL(-1);
	CString str=_T("");
	char buff[2]={'S',0x36};
	str+=buff[0];
	str+=buff[1];
	nRetL=m_ComL.GetCurSel();
	//nRetUser=m_UserNameSet.GetCurSel();
	if (nRetL==-1 || m_LName=="")
	{
		AfxMessageBox(_T("输入的灯名字和灯ID不能不能为空，请确认！"));
	}
	else 
	{
		for (int i=0;i<16;i++)
		{
			str+=pTheApp->m_LInfo[nRetL].LID[i];
		}
		str+="+";
		str+=m_LName;
		str+="#";
		send(pTheApp->m_ConnectSock,(char*)str.GetBuffer(),str.GetLength()*sizeof(TCHAR),0);
		Sleep(1000);
	}
}
