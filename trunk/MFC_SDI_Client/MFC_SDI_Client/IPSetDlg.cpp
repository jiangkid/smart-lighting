// IPSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "IPSetDlg.h"


// CIPSetDlg dialog

IMPLEMENT_DYNAMIC(CIPSetDlg, CDialog)

CIPSetDlg::CIPSetDlg(CWnd* pParent /*=NULL*/)
: CDialog(CIPSetDlg::IDD, pParent)
, m_szIP(_T(""))
, m_szPort(0)
{

}

CIPSetDlg::~CIPSetDlg()
{
}

void CIPSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szIP);
	DDX_Text(pDX, IDC_EDIT2, m_szPort);
}


BEGIN_MESSAGE_MAP(CIPSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CIPSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CIPSetDlg message handlers

void CIPSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strIP,strPort;
	if (m_szIP=="" || m_szPort<=1024)
	{
		if (m_szIP=="")
		{
			AfxMessageBox("����IP��ַ����Ϊ�գ���ȷ��������Ϣ��");
		}
		if (m_szIP!=""&&m_szPort<=1024)
		{
			AfxMessageBox("�˿ں����������1024����ȷ�����������Ŷ˿ںţ�");
		}

	}
	else
	{
		strIP.Format("%s",m_szIP);
		strPort.Format("%d",m_szPort);
		WritePrivateProfileString("Server","serverip",strIP,"D:\\server.ini");
		WritePrivateProfileString("Server","serverport",strPort,"D:\\server.ini");
		AfxMessageBox("���óɹ���");
	}
	//OnOK();
}
