// TabListView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "TabListView.h"


// CTabListView

IMPLEMENT_DYNCREATE(CTabListView, CFormView)

CTabListView::CTabListView()
	: CFormView(CTabListView::IDD)
{

}

CTabListView::~CTabListView()
{
}

void CTabListView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEW_TAB, m_Tab);
	//DDX_Control(pDX, IDC_EXPLORER2, m_Browser1);
}

BEGIN_MESSAGE_MAP(CTabListView, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_VIEW_TAB, &CTabListView::OnTcnSelchangeViewTab)
END_MESSAGE_MAP()


// CTabListView diagnostics

#ifdef _DEBUG
void CTabListView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTabListView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTabListView message handlers

void CTabListView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	UpDateMainFrame();
// 	CRect rect;  
// 	GetClientRect(&rect);  
// 	m_Browser1.SetWindowPos(NULL, rect.TopLeft().x, rect.top, rect.Width(), rect.Height(), NULL);  
// 	COleVariant varEmpty;  
// 	CString str("www.baidu.com");  
// 	//COleVariant varUrl(str);  
// 	m_Browser1.Navigate(str, varEmpty, varEmpty, varEmpty, varEmpty);  

	// TODO: Add your specialized code here and/or call the base class
}

void CTabListView::UpDateMainFrame(void)
{
 	//theApp.m_pTabListView-> GetClientRect(&m_rect);
 	m_Tab.InsertItem(0,_T("单节点信息"),0);
	m_Tab.InsertItem(1,_T("区域信息"),1);
	m_Tab.InsertItem(2,_T("电表信息"),2);
	m_Tab.InsertItem(3,_T("报警信息"),3);
	m_Tab.InsertItem(4,_T("地图显示"),4);
 	m_LightView = new CLightView();
 	m_LightView->Create(IDD_DIALOG3,&m_Tab);
 	CRect m_rect;
 	m_Tab.GetClientRect(m_rect);
	m_rect.DeflateRect(0,20,0,0);
 	m_LightView->MoveWindow(m_rect);
 	m_LightView->ShowWindow(SW_SHOW);

	m_GprsInfoView = new CGprsInfoCtrlDlg();
	m_GprsInfoView->Create(IDD_GPRS_INFO,&m_Tab);
	m_GprsInfoView->MoveWindow(m_rect);
	m_GprsInfoView->ShowWindow(SW_HIDE);

	m_pMapView = new CMapViewDlg();
	m_pMapView->Create(IDD_MAP_VIEW,&m_Tab);
	m_pMapView->MoveWindow(m_rect);
	m_pMapView->ShowWindow(SW_HIDE);

	m_pWarningView = new CWarningDlg();
	m_pWarningView->Create(IDD_WARNING_DLG,&m_Tab);
	m_pWarningView->MoveWindow(m_rect);
	m_pWarningView->ShowWindow(SW_HIDE);


	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
}
void CTabListView::OnTcnSelchangeViewTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_Tab.GetCurFocus();
	switch(sel)
	{
	case 0:
		{
			m_LightView->ShowWindow(SW_SHOW);
			m_GprsInfoView->ShowWindow(SW_HIDE);
			m_pWarningView->ShowWindow(SW_HIDE);
			m_pMapView->ShowWindow(SW_HIDE);
		}
		break;
	case 1:
		{
			theApp.GID[0]='G';
			theApp.GID[1]=0x30;
			theApp.GID[2]=theApp.TID[1];
			theApp.GID[3]=theApp.TID[2];
			theApp.GID[4]='#';
			if (theApp.GID[2]==0x00&&theApp.GID[3]==0x00)
			{
				AfxMessageBox(_T("GID错误，请先打开路信息"));
				return;
			}
			else
			{
				send(theApp.m_ConnectSock,theApp.GID,5,0);
				Sleep(500);

				theApp.GID[1]=0x31;
				send(theApp.m_ConnectSock,theApp.GID,5,0);
			}
			m_LightView->ShowWindow(SW_HIDE);
			m_GprsInfoView->ShowWindow(SW_SHOW);
			m_pWarningView->ShowWindow(SW_HIDE);
			m_pMapView->ShowWindow(SW_HIDE);
		}
		break;
	case 2:
		{
			m_LightView->ShowWindow(SW_HIDE);
			m_GprsInfoView->ShowWindow(SW_HIDE);
			m_pWarningView->ShowWindow(SW_HIDE);
			m_pMapView->ShowWindow(SW_HIDE);
		}
		break;
	case 3:
		{
			m_LightView->ShowWindow(SW_HIDE);
			m_GprsInfoView->ShowWindow(SW_HIDE);
			m_pWarningView->ShowWindow(SW_SHOW);
			m_pMapView->ShowWindow(SW_HIDE);
		}
		break;
	case 4:
		{
			m_LightView->ShowWindow(SW_HIDE);
			m_GprsInfoView->ShowWindow(SW_HIDE);
			m_pWarningView->ShowWindow(SW_HIDE);
			m_pMapView->ShowWindow(SW_SHOW);
		}
		break;

	}
	*pResult = 0;
}
