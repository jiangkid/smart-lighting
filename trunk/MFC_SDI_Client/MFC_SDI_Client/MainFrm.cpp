
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "MainFrm.h"
#include "MFC_SDI_ClientView.h"
#include "CVListView.h"
#include "TabListView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_QUITEXE, &CMainFrame::OnQuitexe)
	ON_COMMAND(ID_OPENEXE, &CMainFrame::OnOpenexe)
	ON_COMMAND(ID_QUIT, &CMainFrame::OnQuit)
	ON_WM_TIMER()
	ON_COMMAND(ID_UserControl, &CMainFrame::OnUsercontrol)
	ON_COMMAND(ID_MainSet, &CMainFrame::OnMainset)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
: m_bSplitted(false)
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);
// 	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|CBRS_ALIGN_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS, CRect(4, 4, 0, 0))
// 		|| !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
// 	{
// 		return FALSE;
// 	}
// 	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
// 	m_wndToolBar.LoadTrueColorToolBar(116,IDB_BITMAP9);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);
	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	//����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}
	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	//CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	// ���ù�������ͣ�����ڲ˵��滻
//	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);
	// ���ÿ���(��ס Alt �϶�)�������Զ���
 	CMFCToolBar::EnableQuickCustomization();
	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\ToolbarNULL.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16,16), FALSE);
			CMFCToolBar::SetSizes(CSize(16,16), CSize(16,16));
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
	CSize btSize;
	btSize=CMFCToolBar::GetMenuButtonSize();
	CMFCToolBar::SetMenuSizes(CSize(32,24), CSize(7,7)); 

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	UINT IDArray[] ={ID_APP_ABOUT,0,ID_1,0,ID_2,0,ID_3,0,ID_4,0,ID_5,0,ID_6,0,ID_7,0,ID_8};
	m_wndToolBar.SetButtons(IDArray, 17); 
	theApp.nStatus[3]=0x01;
	TrayMessage(NIM_ADD,IDI_ICON4);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	//cs.hMenu = NULL;//ȥ���˵�
	cs.x=300;
	cs.y=150;
	cs.style&=~WS_MAXIMIZEBOX; //���ð�����󻯰�ť 
	cs.style&=~WS_THICKFRAME;//��ֹ�û��ı䴰�ڴ�С
	//cs.style&=~WS_MINIMIZEBOX;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	return TRUE;
}
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG
// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// Ϊ�����û������������Զ��尴ť
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWndEx::OnSize(nType, cx, cy);
	if (m_bSplitted && cx > 0 && cy > 0) {
		int height = cy/2;
		m_wndSplitter.SetRowInfo(0, cy-height, 0);
		m_wndSplitter.SetRowInfo(1, height, 0);
		m_wndSplitter.RecalcLayout();
	}
	// TODO: Add your message handler code here
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
// 	CString strClassView;
// 	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
// 	ASSERT(bNameValid);
// 	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
// 	{
// 		TRACE0("δ�ܴ���������ͼ������\n");
// 		return FALSE; // δ�ܴ���
// 	}

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

//	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
//	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	if (!m_wndSplitter.CreateStatic(this,2,1))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTabListView), CSize(0,260),  pContext) ||
		!m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CCVListView), CSize(0,0), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	m_bSplitted = TRUE;
	return TRUE; //ע��
	return CFrameWndEx::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (AfxMessageBox(_T("��ȷ��Ҫ�ر�����·�ƿ���ϵͳ��?"),MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
	{
		TrayMessage(NIM_DELETE,IDI_ICON4);
		CFrameWndEx::OnClose();
	}	
}
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x=1200;   
	lpMMI->ptMinTrackSize.y=800;   

	lpMMI->ptMaxTrackSize.x=1200;   
	lpMMI->ptMaxTrackSize.y=800;  
	CFrameWndEx::OnGetMinMaxInfo(lpMMI);
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case MYWM_NOTIFYICON:       
		//������û��������Ϣ
		if(lParam==WM_LBUTTONDBLCLK)
		{  //���˫��ʱ�����ڳ���
			if (theApp.nStatus[0]==0x01)
			{
				theApp.nStatus[0]=0x00;
				theApp.m_WaitDlg.DoModal();
				KillTimer(1);
				TrayMessage(NIM_MODIFY,IDI_ICON4);
			}
			else
				if (theApp.nStatus[3]==0x01)
				{
					AfxGetApp()->m_pMainWnd->
					ShowWindow(SW_SHOW);
				}
		
		}
		else if(lParam==WM_RBUTTONDOWN){ 
			//����Ҽ����������˵�
			CMenu menu;
			menu.LoadMenu(IDR_HIDEMENU);  //������ص��ǲ˵�����������Ե�����ϵͳ�Դ��Ĳ˵�����������Դ�����Լ�����Զ���Ĳ˵� 
			//�������ȶ���Ĳ˵�
			CMenu* pMenu=menu.GetSubMenu(0);
			CPoint pos;
			GetCursorPos(&pos);
			pMenu->TrackPopupMenu
				(TPM_LEFTALIGN|TPM_RIGHTBUTTON,
				pos.x,pos.y,AfxGetMainWnd());
		}
		break;
	case WM_SYSCOMMAND:     
		//�����ϵͳ��Ϣ
		if( wParam==SC_CLOSE )
		{ 
			//���յ���С����Ϣʱ����������
			AfxGetApp()->m_pMainWnd->
				ShowWindow(SW_HIDE);
			TrayMessage(NIM_ADD,IDI_ICON4);
			return 0;
		}
		if (wParam==SC_MINIMIZE)
		{
			AfxGetApp()->m_pMainWnd->
				ShowWindow(SW_HIDE);
			TrayMessage(NIM_ADD,IDI_ICON4);
			return 0;
		}
		break;
	}    
	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}
void CMainFrame::OnQuitexe()
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::OnOpenexe()
{
	// TODO: Add your command handler code here
	AfxGetApp()->m_pMainWnd->
		ShowWindow(SW_SHOW);
	//TrayMessage(NIM_DELETE);
}

void CMainFrame::OnQuit()
{
	// TODO: Add your command handler code here
	OnClose();
}

bool CMainFrame::TrayMessage(DWORD dwMessage,UINT nIDI_ICON)
{
	NOTIFYICONDATA m_tnid;
	m_tnid.cbSize=sizeof(NOTIFYICONDATA);    
	m_tnid.hWnd=this->m_hWnd;
	m_tnid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	m_tnid.uCallbackMessage=MYWM_NOTIFYICON;
	CString szToolTip;
	szToolTip=_T("����·�ƿ���ϵͳ");
	_tcscpy(m_tnid.szTip, szToolTip);
	m_tnid.uID=IDR_MAINFRAME;
	m_tnid.hBalloonIcon=AfxGetApp()->LoadIcon(IDI_ICON6);
	HICON hIcon;
	hIcon=AfxGetApp()->LoadIcon(nIDI_ICON);
	m_tnid.hIcon=hIcon;
	if(hIcon)
		::DestroyIcon(hIcon);
	return Shell_NotifyIcon(dwMessage,&m_tnid);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static bool bol=true;
	switch(nIDEvent)
	{
	case 1:
		if (bol)
		{
			TrayMessage(NIM_MODIFY,IDI_ICON6);
			bol=false;
		}
		else
		{
			TrayMessage(NIM_MODIFY);
			bol=true;
		}
		break;
// 	case 2:
// 		if (bol)
// 		{
// 			TrayMessage(NIM_ADD,IDI_ICON6);
// 			bol=false;
// 		}
// 		else
// 		{
// 			TrayMessage(NIM_DELETE,IDI_ICON6);
// 			bol=true;
// 		}
// 	case 3:
// 		if (bol)
// 		{
// 			TrayMessage(NIM_ADD,IDI_ICON6);
// 			bol=false;
// 		}
// 		else
// 		{
// 			TrayMessage(NIM_DELETE,IDI_ICON6);
// 			bol=true;
// 		}
		
	default:
		break;
		
	}
	CFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::StartTimer(UINT_PTR nIDEvent)
{
	SetTimer(nIDEvent,1000,0);
}

void CMainFrame::OnUsercontrol()
{
	// TODO: Add your command handler code here
	Userdlg = (CUserCtrDlg *)malloc(sizeof(CUserCtrDlg));
	Userdlg->CUserCtrDlg::CUserCtrDlg();
	Userdlg->Create(IDD_User_CTRL, this);
	Userdlg->CenterWindow();
	Userdlg->ShowWindow(SW_SHOW);
}

void CMainFrame::OnMainset()
{
	// TODO: Add your command handler code here
	Mdlg = (CMainSetDlg *)malloc(sizeof(CMainSetDlg));
	Mdlg->CMainSetDlg::CMainSetDlg();
	Mdlg->Create(IDD_MainSet, this);
	Mdlg->CenterWindow();
	Mdlg->ShowWindow(SW_SHOW);
}
