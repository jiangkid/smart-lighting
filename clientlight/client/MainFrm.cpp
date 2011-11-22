// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MFC_Client_Light.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_ADDLIST WM_USER+3
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_ADDLIST, OnAddMember)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
//有问题
	if (!m_statuslist.Create(this, IDD_DIALOG2, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM
		| CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED,IDD_ABOUTBOX))
	{
		TRACE0("Failed to create Dialog bar\n");
		return -1;      // fail to create
	}
	//m_statuslist.EnableDocking(CBRS_ALIGN_BOTTOM);
	//DockControlBar(&m_statuslist,AFX_IDW_DOCKBAR_BOTTOM);
	//m_statuslist.SetWindowText(_TEXT("在线列表"));


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	#define BUTTON_INDEX 1
	m_wndToolBar.SetButtonInfo(BUTTON_INDEX,ID_IP,TBBS_SEPARATOR,160);	
	CRect rect=NULL;
	m_wndToolBar.GetItemRect(BUTTON_INDEX,&rect);
	if(!m_ip1.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,rect,&m_wndToolBar,ID_IP))
		return -1;
	m_ip1.SetWindowText("127.0.0.1");	

	m_wndToolBar.SetButtonInfo(BUTTON_INDEX+2,ID_EDIT,TBBS_SEPARATOR,80);
	CRect rect1=NULL;
	m_wndToolBar.GetItemRect(BUTTON_INDEX+2,&rect1);
	if (!m_port.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,rect1,&m_wndToolBar,ID_EDIT))
		return -1;
	m_port.SetWindowText("5002");	
	
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);	

	m_statuslist.EnableDocking(CBRS_ALIGN_BOTTOM);
	DockControlBar(&m_statuslist, AFX_IDW_DOCKBAR_BOTTOM);
	m_statuslist.SetWindowText(_TEXT("路灯各支路状态"));
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;
	cs.cx=GetSystemMetrics(SM_CXSCREEN);
	cs.cy=GetSystemMetrics(SM_CYSCREEN);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



void CMainFrame::OnAddMember(WPARAM wParam, LPARAM lParam)
{
	roadinfo* pStr=(roadinfo*)wParam;
	m_statuslist.m_ListCtrl->AddItem(pStr);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}
