// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "LoginDlg.h"
#include "IPSetDlg.h"

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLoginDlg::IDD, pParent)
, m_szNamePassword(_T(""))
, m_bAuto(false)
, nIndex(0)
, m_UserName(_T(""))
, m_PassWord(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ID, m_ComboBox);
	DDX_Text(pDX, IDC_NAME, m_UserName);
	DDX_Text(pDX, IDC_PASSWORD, m_PassWord);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
	ON_CBN_EDITCHANGE(IDC_COMBO_ID, &CLoginDlg::OnCbnEditchangeComboId)
	ON_BN_CLICKED(IDC_BTN_SET, &CLoginDlg::OnBnClickedBtnSet)
END_MESSAGE_MAP()


// CLoginDlg message handlers

void CLoginDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CMFC_SDI_ClientApp *pTheApp =(CMFC_SDI_ClientApp*)AfxGetApp();	
	ZeroMemory(&userInfo[0],sizeof(USERINFO));
	int i = m_ComboBox.GetCurSel();	
	if (i==0)
		userInfo[0].Idetify=0x30;
	else if(i==1)
		userInfo[0].Idetify=0x31;
	if (m_PassWord==""||m_UserName=="")
	{
		AfxMessageBox(_T("用户名和密码不能为空，请确认！"));
	}	
	else 
		if (m_UserName.GetLength()*sizeof(TCHAR)>=11||m_PassWord.GetLength()>=13)
		{
			AfxMessageBox("输入的用户名或密码不合法，请重新输入。");
		}
		else
		{
			char * NameStr = (LPTSTR)(LPCTSTR)m_UserName;
			char * PWStr   = (LPTSTR)(LPCTSTR)m_PassWord;
			memcpy(userInfo[0].UserName,NameStr, m_UserName.GetLength()*sizeof(TCHAR));
			memcpy(userInfo[0].PassWord,PWStr,m_PassWord.GetLength()*sizeof(TCHAR));
			OnOK();
		}
}

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ComboBox.AddString(_T("管理员"));
	m_ComboBox.AddString(_T("普通用户"));
	m_ComboBox.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		int nChar = pMsg->wParam;
		if ((nChar == VK_DELETE) || (nChar == VK_BACK))
		{
			m_bAuto = false;
		}
		else
		{
			m_bAuto = true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::OnCbnEditchangeComboId()
{
	// TODO: Add your control notification handler code here
	if (!m_bAuto)
	{
		return;
	}

	CString strText = _T("");

	m_ComboBox.GetWindowText(strText);
	int nLength = strText.GetLength();

	nIndex = m_ComboBox.FindString(-1,strText);
	if (nIndex!=CB_ERR)
	{
		m_ComboBox.SetCurSel(nIndex);
		m_ComboBox.SetEditSel(nLength,-1);
	}
}


void CLoginDlg::OnBnClickedBtnSet()
{
	// TODO: Add your control notification handler code here
	CIPSetDlg dlg;
	dlg.DoModal();
}
