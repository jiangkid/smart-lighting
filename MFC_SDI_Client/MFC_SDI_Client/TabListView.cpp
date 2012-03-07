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
	// TODO: Add your specialized code here and/or call the base class
}

void CTabListView::UpDateMainFrame(void)
{
 	//theApp.m_pTabListView-> GetClientRect(&m_rect);
 	m_Tab.InsertItem(0,_T("单节点信息"),0);
 	m_Tab.InsertItem(1,_T("单路信息"),1);
	m_Tab.InsertItem(2,_T("区域信息"),2);
 	m_LightView = new CLightView();
 	m_LightView->Create(IDD_DIALOG3,&m_Tab);
 	CRect m_rect;
 	m_Tab.GetClientRect(m_rect);
	m_rect.DeflateRect(0,20,0,0);
 	m_LightView->MoveWindow(m_rect);
 	m_LightView->ShowWindow(SW_SHOW);

	m_RoadView = new CRoudInfoView();
	m_RoadView->Create(IDD_Roud_Info,&m_Tab);
	m_RoadView->MoveWindow(m_rect);
	m_RoadView->ShowWindow(SW_HIDE);

	m_GprsInfoView = new CGprsInfoCtrlDlg();
	m_GprsInfoView->Create(IDD_GPRS_INFO,&m_Tab);
	m_GprsInfoView->MoveWindow(m_rect);
	m_GprsInfoView->ShowWindow(SW_HIDE);

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
			m_RoadView->ShowWindow(SW_HIDE);
			m_GprsInfoView->ShowWindow(SW_HIDE);
		}
		break;
	case 1:
		{	
			theApp.TID[0]='R';
			theApp.TID[5]='#';
			send(theApp.m_ConnectSock,theApp.TID,6,0);
			Sleep(500);

			m_LightView->ShowWindow(SW_HIDE);
			m_RoadView->ShowWindow(SW_SHOW);
			m_GprsInfoView->ShowWindow(SW_HIDE);

  			ConTrlInfo* pGetInfo=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
  			ZeroMemory(pGetInfo,sizeof(ConTrlInfo));
  			pGetInfo->m_First[0]=0x2F;
  			pGetInfo->m_First[1]=0x43;
  			pGetInfo->m_First[2]=0x2F;
  			pGetInfo->m_First[3]=0x01;
  			memcpy(pGetInfo->m_ID,theApp.TID+1,4);
  			pGetInfo->m_OrderType[0]=0x1A;
  			pGetInfo->m_OrderObject[0]=0x32;
  			pGetInfo->m_ActiveType[0]=0xBD;
  			pGetInfo->m_EndBuffer[1]=0xCC;
  			SendContrlInfo(&hdr,pGetInfo);
			Sleep(500);
  			free(pGetInfo);
 			ConTrlInfo* pGetInfo1=(ConTrlInfo*)malloc(sizeof(ConTrlInfo));
 			ZeroMemory(pGetInfo1,sizeof(ConTrlInfo));
 			pGetInfo1->m_First[0]=0x2F;
 			pGetInfo1->m_First[1]=0x43;
 			pGetInfo1->m_First[2]=0x2F;
 			pGetInfo1->m_First[3]=0x06;
 			memcpy(pGetInfo1->m_ID,theApp.TID+1,4);
 			pGetInfo1->m_OrderType[0]=0x1A;
 			pGetInfo1->m_OrderObject[0]=0x33;
 			pGetInfo1->m_ActiveType[0]=0xBD;
 			pGetInfo1->m_CheckData[0]=0xA0;
 			pGetInfo1->m_EndBuffer[1]=0xCC;
 			SendContrlInfo(&hdr,pGetInfo1);
 			free(pGetInfo1);
		}
		break;
	case 2:
		m_LightView->ShowWindow(SW_HIDE);
		m_RoadView->ShowWindow(SW_HIDE);
		m_GprsInfoView->ShowWindow(SW_SHOW);
		break;
	}
	*pResult = 0;
}
