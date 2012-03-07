// MainSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MainSetDlg.h"

// CMainSetDlg dialog

IMPLEMENT_DYNAMIC(CMainSetDlg, CDialog)

CMainSetDlg::CMainSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainSetDlg::IDD, pParent)
	, m_GNum(0)
	, m_TNum(0)
	, m_RNum(0)
	, m_LNum(0)
	, m_NewID(_T(""))
{

}

CMainSetDlg::~CMainSetDlg()
{
}

void CMainSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GNum, m_GNum);
	DDX_Text(pDX, IDC_TNum, m_TNum);
	DDX_Text(pDX, IDC_RNum, m_RNum);
	DDX_Text(pDX, IDC_LNum, m_LNum);
	DDX_Text(pDX, IDC_EDIT7, m_NewID);
}


BEGIN_MESSAGE_MAP(CMainSetDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_GetID, &CMainSetDlg::OnBnClickedBtnGetid)
	ON_BN_CLICKED(IDC_BTN_Save, &CMainSetDlg::OnBnClickedBtnSave)
END_MESSAGE_MAP()


// CMainSetDlg message handlers

void CMainSetDlg::OnBnClickedBtnGetid()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	BGTrue = false;
	BTTrue = false;
	BRTrue = false;
	BLTrue = false;
	CEdit* pEdit =(CEdit*)GetDlgItem(IDC_EDIT7);
	pEdit->SetLimitText(-1);
	SetDlgItemText(IDC_EDIT7,"");
	CMFC_SDI_ClientApp* pTheApp = (CMFC_SDI_ClientApp*)AfxGetApp();
	unsigned char m_ID[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	unsigned char m_GID[1]={0x00};
	unsigned char m_TID[2]={0x00,0x00};
	unsigned char m_RID[3]={0x00,0x00,0x00};
	unsigned char m_LID[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	CString strIDInfo=_T("");
	int nGCount(0),nTCount(0),nRCount(0),nLCount(0);
	for (int i =0 ; i<m_GNum;i++)
	{
		m_ID[0]++;
		nGCount++;
		m_GID[0]=m_ID[0];
		CString m_strGID=_T(""),m_GCtoCS=_T("");
		m_GCtoCS=CharToCString(m_GID,1);
		m_strGID.Format(_T("BG%s#"),m_GCtoCS);
		send(pTheApp->m_ConnectSock,(char*)m_strGID.GetBuffer(),m_strGID.GetLength()*sizeof(TCHAR),0);
		strIDInfo.Format(_T("第%d的GPRSID"),nGCount);
		Sleep(500);
		ShowMessage(m_GID,1,strIDInfo);
		for (int n=0;n<m_TNum;n++)
		{
			if (n==0)
			{
				m_ID[1]=0x00;
				nTCount=0;
			}
			m_ID[1]++;
			nTCount++;
			m_TID[0]=m_GID[0];
			m_TID[1]=m_ID[1];
			CString  m_strTID=_T(""),m_TCtoCS=_T("");
			m_TCtoCS=CharToCString(m_TID,2);
			m_strTID.Format(_T("BT%s#"),m_TCtoCS);
			send(pTheApp->m_ConnectSock,(char*)m_strTID.GetBuffer(),
				m_strTID.GetLength()*sizeof(TCHAR),0);
			strIDInfo.Format(_T("第%d个GPRS下第%d终端ID"),nGCount,nTCount);		
			Sleep(500);
			ShowMessage(m_TID,2,strIDInfo);
			for(int m=0;m<m_RNum;m++)
			{
				if (m==0)
				{
					m_ID[2]=0x00;
					nRCount=0;
				}
				m_ID[2]++;
				nRCount++;
				m_RID[0]=m_TID[0];
				m_RID[1]=m_TID[1];
				m_RID[2]=m_ID[2];
				CString  m_strRID=_T(""),m_RCtoCS=_T("");
				m_RCtoCS=CharToCString(m_RID,3);
				m_strRID.Format(_T("BR%s#"),m_RCtoCS);
				send(pTheApp->m_ConnectSock,(char*)m_strRID.GetBuffer(),
					m_strRID.GetLength()*sizeof(TCHAR),0);
				strIDInfo.Format(_T("第%d个GPRS下第%d终端第%d路ID"),nGCount,nTCount,nRCount);
				Sleep(500);
				ShowMessage(m_RID,3,strIDInfo);
				for (int j=0;j<m_LNum;j++)
				{
					if (j==0)
					{
						m_ID[7]=0x00;
						m_ID[6]=0x00;
						m_ID[5]=0x00;
						m_ID[4]=0x00;
						m_ID[3]=0x00;
						nLCount=0;
					}
					m_ID[7]++;
					nLCount++;
					m_LID[0]=m_RID[0];
					m_LID[1]=m_RID[1];
					m_LID[2]=m_RID[2];
					m_LID[7]=m_ID[7];
					m_LID[6]=m_ID[6];
					m_LID[5]=m_ID[5];
					m_LID[4]=m_ID[4];
					m_LID[3]=m_ID[3];
					CString  m_strLID=_T(""),m_LCtoCS=_T("");
					m_LCtoCS=CharToCString(m_LID,8);
					m_strLID.Format(_T("BL%s#"),m_LCtoCS);
					send(pTheApp->m_ConnectSock,(char*)m_strLID.GetBuffer(),
						m_strLID.GetLength()*sizeof(TCHAR),0);
					strIDInfo.Format(_T("第%d个GPRS下第%d终端第%d路第%d盏ID"),nGCount,nTCount,nRCount,nLCount);
					Sleep(500);
					ShowMessage(m_LID,8,strIDInfo);
					if (m_ID[7]==0xFF)
					{
						m_ID[7]=0x00;
						m_ID[6]++;
						if (m_ID[6]==0xFF)
						{
							m_ID[6]=0x00;
							m_ID[5]++;
							if (m_ID[5]==0xFF)
							{
								m_ID[5]=0x00;
								m_ID[4]++;
								if (m_ID[4]==0x7F)
								{
									m_ID[4]=0x00;
									m_ID[3]++;
								}
							}
						}
					}
				}
			}	
		}
	}
}

void CMainSetDlg::ShowMessage(unsigned char* pszMessage, int nLength,CString str)
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
	strShow.Format(_T("%s:%s\r\n"),str, szText);

	CEdit* pEdit =(CEdit*)GetDlgItem(IDC_EDIT7);
	pEdit->SetFocus();
	pEdit->SetSel(-1, -1);
	pEdit->ReplaceSel(strShow);
	
}

void CMainSetDlg::OnBnClickedBtnSave()
{
	// TODO: Add your control notification handler code here
	CTime time=CTime::GetCurrentTime();
	CString str,strTime,str1;
	strTime=time.Format("Save%Y-%m-%d_%H-%M-%S.txt");
	CFile file(strTime,CFile::modeCreate | CFile::modeWrite);
	CArchive ar(&file,CArchive::store);

	UpdateData();
	str.Format("%s",m_NewID);
	ar<<str;
	str1.Format("数据保存在%s文件中",strTime);
	AfxMessageBox(str1);
}

CString CMainSetDlg::CharToCString(unsigned char* str, int nLength)
{
	CString strShow=_T("");
	LPTSTR p;
	TCHAR szText[300];
	ZeroMemory(szText, 300);
	p = szText;
	for (int i = 0; i< nLength; i++)
	{
		p+= wsprintf(p, "%.2X", str[i]);  //这部分为关键部分
	}
	strShow.Format(_T("%s"), szText);
	return strShow;
}

void CMainSetDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	DestroyWindow();
	//CDialog::OnCancel();
}
void CMainSetDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	this->~CMainSetDlg();
	free(this);
	CDialog::PostNcDestroy();
}