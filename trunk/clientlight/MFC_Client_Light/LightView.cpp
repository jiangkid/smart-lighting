// LightView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Client_Light.h"
#include "LightView.h"
#include "BtnST.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightView

IMPLEMENT_DYNCREATE(CLightView, CFormView)

CLightView::CLightView()
	: CFormView(CLightView::IDD)
{
	//{{AFX_DATA_INIT(CLightView)
	//}}AFX_DATA_INIT
	m_id3true=TRUE;
	m_id2true=TRUE;
	m_id1true=TRUE;

}

CLightView::~CLightView()
{
}

void CLightView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightView)
	DDX_Control(pDX, IDC_BUTTON3, m_btnID33);
	DDX_Control(pDX, IDC_BUTTON2, m_btnID22);
	DDX_Control(pDX, IDC_BUTTON1, m_btnID11);
	DDX_Control(pDX, IDC_BTN_LIGHT1, m_btnID1);
	DDX_Control(pDX, IDC_BTN_LIGHT3, m_btnID3);
	DDX_Control(pDX, IDC_BTN_LIGHT2, m_btnID2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightView, CFormView)
	//{{AFX_MSG_MAP(CLightView)
	ON_BN_CLICKED(IDC_BTN_LIGHT1, OnBtnLight1)
	ON_BN_CLICKED(IDC_BTN_LIGHT2, OnBtnLight2)
	ON_BN_CLICKED(IDC_BTN_LIGHT3, OnBtnLight3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightView diagnostics

#ifdef _DEBUG
void CLightView::AssertValid() const
{
	CFormView::AssertValid();

}

void CLightView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLightView message handlers

void CLightView::OnInitialUpdate() 
{
	//UpdateData(true);
	m_btnID1.SubclassDlgItem(IDC_BTN_LIGHT1,this);
	m_btnID1.SetIcon(IDI_ICON_ON);
	m_btnID11.SubclassDlgItem(IDC_BUTTON1,this);
	m_btnID11.SetIcon(IDI_ICON_OFF);

	m_btnID2.SubclassDlgItem(IDC_BTN_LIGHT2,this);
	m_btnID2.SetIcon(IDI_ICON_ON);
	m_btnID22.SubclassDlgItem(IDC_BUTTON2,this);
	m_btnID22.SetIcon(IDI_ICON_OFF);

	m_btnID3.SubclassDlgItem(IDC_BTN_LIGHT3,this);
	m_btnID3.SetIcon(IDI_ICON_ON);
	m_btnID33.SubclassDlgItem(IDC_BUTTON3,this);
	m_btnID33.SetIcon(IDI_ICON_OFF);
	
	m_btnID11.ShowWindow(SW_HIDE);
	m_btnID22.ShowWindow(SW_HIDE);
	m_btnID33.ShowWindow(SW_HIDE);
	//m_btnID1.SubclassDlgItem(IDC_BTN_LIGHT1,this);
	//m_btnID1.ModifyStyle(0,BS_OWNERDRAW);
	//m_btnID1.MoveWindow(10,10,68,56);
	//m_btnID1.LoadBitmaps(IDI_ICON_ON,IDI_ICON_OFF,0,0);

	//m_btnID2.SubclassDlgItem(IDC_BTN_LIGHT2,this);
	//m_btnID2.SetIcon(IDI_ICON_ON);
	//m_btnID3.SetFlat(FALSE);
	//CFormView::OnInitialUpdate();

	//TODO: Add your specialized code here and/or call the base class

}

void CLightView::OnBtnLight1() 
{
	// TODO: Add your control notification handler code here
	//UpdateData(true);
	/*CString str;
	GetDlgItemText(IDC_BTN_LIGHT1, str);
	if(str.Compare("开") == 0)
	{	
		SetDlgItemText(IDC_BTN_LIGHT1, "关");

	}
	if (str.Compare("关") == 0)
	{
		SetDlgItemText(IDC_BTN_LIGHT1, "开");
	}*/

	m_btnID1.ShowWindow(SW_HIDE);
	m_btnID11.ShowWindow(SW_SHOW);
}


void CLightView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class


}

void CLightView::OnBtnLight2() 
{
	// TODO: Add your control notification handler code here
	m_btnID2.ShowWindow(SW_HIDE);
	m_btnID22.ShowWindow(SW_SHOW);
}

void CLightView::OnBtnLight3() 
{
	m_btnID3.ShowWindow(SW_HIDE);
	m_btnID33.ShowWindow(SW_SHOW);
}

void CLightView::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_btnID11.ShowWindow(SW_HIDE);
	m_btnID1.ShowWindow(SW_SHOW);
}

void CLightView::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_btnID22.ShowWindow(SW_HIDE);
	m_btnID2.ShowWindow(SW_SHOW);
}

void CLightView::OnButton3() 
{
	// TODO: Add your control notification handler code here
	m_btnID33.ShowWindow(SW_HIDE);
	m_btnID3.ShowWindow(SW_SHOW);
}
