// MapViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MapViewDlg.h"


// CMapViewDlg dialog

IMPLEMENT_DYNAMIC(CMapViewDlg, CDialog)

CMapViewDlg::CMapViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapViewDlg::IDD, pParent)
	, m_lat(_T(""))
	, m_lng(_T(""))
{

}

CMapViewDlg::~CMapViewDlg()
{
}

void CMapViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	DDX_Control(pDX, IDC_EXPLORER1, m_Browser);
	DDX_Text(pDX, IDC_EDIT1, m_lat);
	DDX_Text(pDX, IDC_EDIT2, m_lng);
}


BEGIN_MESSAGE_MAP(CMapViewDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapViewDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMapViewDlg message handlers

BOOL CMapViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetClientRect(&rect);
	//m_Browser.ObjectForScripting = this;
	//m_Browser.SetWindowPos(NULL, rect.TopLeft().x, rect.top, rect.Width()-200, rect.Height(), NULL);
	COleVariant varEmpty;
	m_Browser.Navigate("C:\\HTMLPage1.htm",varEmpty,varEmpty,varEmpty,varEmpty);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMapViewDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	//m_Browser
	UpdateData(true);
	CWebPage m_be;
	CString str1;
	CString str2;
	GetDlgItemText(IDC_EDIT1,str1);
	GetDlgItemText(IDC_EDIT2,str2);
	m_be.SetDocument(m_Browser.get_Document());
	m_be.CallJScript("addMarker",str1,str2);
}
