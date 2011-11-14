// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "LightClient.h"
#include "ChatCAsyncSocketDlg.h"
#include "MainFrm.h"
#include "LoginDlg.h"
#include "LightClientView.h"
#include "LightClientView2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

extern CLightClientApp theApp; 
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_Connect, OnConnect)
	ON_COMMAND(ID_TEST, OnTest)
	ON_COMMAND(ID_Login, OnLogin)
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

		// TODO: Delete these three lines if you don't want the toolbar to
		//  be dockable
		m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
		EnableDocking(CBRS_ALIGN_ANY);
		DockControlBar(&m_wndToolBar);
		ShowControlBar(&m_wndToolBar,FALSE,FALSE);

		m_ImageList.Create(IDB_IMAGELIST, 13, 1, RGB(0,255,0));
		// CG: The following block was inserted by the 'Dialog Bar' component
		{
			m_wndBuilding.SetBtnImageList (&m_ImageList);
			//m_wndInformation.SetMenuID(IDR_POPUP);
			
			// Initialize dialog bar m_wndInformation
			if (!m_wndBuilding.Create(this, CG_ID_VIEW_BUILDING,
				_T("工程区"), CSize(200,200)))
			{
				TRACE0("Failed to create dialog bar m_wndInformation\n");
				return -1;		// fail to create
			}
			
			// allow bar to be resized when floating
			m_wndBuilding.SetBarStyle(m_wndBuilding.GetBarStyle() |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			
			m_wndBuilding.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockControlBar(&m_wndBuilding, AFX_IDW_DOCKBAR_LEFT);
		}
		// CG: The following block was inserted by the 'Dialog Bar' component
		{
			m_wndOutput.SetBtnImageList (&m_ImageList);
			//m_wndOutput.SetMenuID(IDR_POPUP);
			
			// Initialize dialog bar m_wndOutput
			if (!m_wndOutput.Create(this, CG_ID_VIEW_OUTPUT,
				_T("输出区"), CSize(200,195)))
			{
				TRACE0("Failed to create dialog bar m_wndOutput\n");
				return -1;		// fail to create
			}
			
			// Add the views to the tab control.
			m_wndOutput.AddView(_T("最近报警记录"), RUNTIME_CLASS(CLVErrorRecord));
			m_wndOutput.AddView(_T("最近操作记录"), RUNTIME_CLASS(CListOperatorRecord));
			
			// allow bar to be resized when floating
			m_wndOutput.SetBarStyle(m_wndOutput.GetBarStyle() |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			
			m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockControlBar(&m_wndOutput, AFX_IDW_DOCKBAR_BOTTOM);
		}
		// CG: The following block was inserted by the 'Dialog Bar' component
		{
			m_wndInformation.SetBtnImageList (&m_ImageList);
			//m_wndInformation.SetMenuID(IDR_LISTPOPUP);
			
			// Initialize dialog bar m_wndInformation
			if (!m_wndInformation.Create(this, CG_ID_VIEW_INFORMATION,
				_T("显示区"), CSize(GetSystemMetrics(SM_CXSCREEN)-491,200)))
			{
				TRACE0("Failed to create dialog bar m_wndInformation\n");
				return -1;		// fail to create
			}
			
			// allow bar to be resized when floating
			m_wndInformation.SetBarStyle(m_wndInformation.GetBarStyle() |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			
			m_wndInformation.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockControlBar(&m_wndInformation, AFX_IDW_DOCKBAR_RIGHT);
		}
		// CG: The following block was inserted by the 'Dialog Bar' component
		{
			m_wndMacInfo.SetBtnImageList (&m_ImageList);
			//m_wndInformation.SetMenuID(IDR_POPUP);
			
			// Initialize dialog bar m_wndInformation
			if (!m_wndMacInfo.Create(this, CG_ID_VIEW_MACINFO,
				_T("信息区"), CSize(300,200)))
			{     
				TRACE0("Failed to create dialog bar m_wndInformation\n");
				return -1;		// fail to create
			}
			
			// allow bar to be resized when floating
			m_wndMacInfo.SetBarStyle(m_wndMacInfo.GetBarStyle() |
				CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
			
			m_wndMacInfo.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockControlBar(&m_wndMacInfo, AFX_IDW_DOCKBAR_RIGHT);
	}
		return 0;


}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

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


void CMainFrame::OnConnect() 
{
	// TODO: Add your command handler code here
	CChatCAsyncSocketDlg dlg;
	dlg.DoModal();
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_wndSplitter.CreateStatic(this,1,2);
	m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CLightClientView2),
		CSize(100,200),pContext);
	m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CLightClientView),
		CSize(100,200),pContext);
	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnTest() 
{
	// TODO: Add your command handler code here
	CMDIChildWnd* pChildFrame = (CMDIChildWnd*)GetActiveFrame();
	if (pChildFrame == NULL)
	{
		return;
	}
	
	//获得当前视图
	CLightClientView* pView = (CLightClientView*)pChildFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	
	CRect rect;
	pView->GetClientRect(rect);
	CString strText = _T("");
	strText.Format(_T("在框架中获得当前视图指针：0x%08X"), pView);
	CDC* pDC = pView->GetDC();
	pDC->SetTextAlign(TA_CENTER);
	pDC->TextOut(rect.Width() / 2, rect.Height() / 2, strText);
}

void CMainFrame::OnLogin() 
{
	// TODO: Add your command handler code here
	CLoginDlg dlg;
	dlg.DoModal();
}

void CMainFrame::ReportToExcel(bool bType)
{
	//KillTimer(5);
	m_bChooseDB = TRUE;
	/*******************加载EXCEL表格******************/
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if(!m_ExlApp.CreateDispatch("Excel.Application",NULL)) //用m_ExlApp对象创建Excel2003进程
	{ 
		AfxMessageBox("创建Excel服务失败!"); 
		m_bChooseDB = FALSE;
		return; 
	} 
	m_ExlApp.SetVisible(TRUE); //设置为可见  
	m_ExlBooks.AttachDispatch(m_ExlApp.GetWorkbooks(),TRUE); 
	m_ExlBook.AttachDispatch(m_ExlBooks.Add(covOptional),TRUE);//加载EXCEL模板 
	m_ExlSheets.AttachDispatch(m_ExlBook.GetSheets(),TRUE);//加载Sheet页面 
	/*******************加载EXCEL表格******************/
	m_ExlSheet.AttachDispatch(m_ExlSheets.GetItem(_variant_t((long)1)),TRUE); 
	m_ExlSheet.SetName("AlarmReportSheet"); 
	/***************改变单元格宽度**************/
	if(!bType)
	{
		m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("E1"),_variant_t("E1")),TRUE); 
		m_ExlRge.SetColumnWidth(_variant_t((long)25));
		m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("G1"),_variant_t("G1")),TRUE); 
		m_ExlRge.SetColumnWidth(_variant_t((long)50));
	}
	else
	{
		m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("B1"),_variant_t("B1")),TRUE); 
		m_ExlRge.SetColumnWidth(_variant_t((long)19));
		m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("C1"),_variant_t("C1")),TRUE); 
		m_ExlRge.SetColumnWidth(_variant_t((long)15));
		m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("D1"),_variant_t("D1")),TRUE); 
		m_ExlRge.SetColumnWidth(_variant_t((long)25));
		m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("E1"),_variant_t("E1")),TRUE); 
		m_ExlRge.SetColumnWidth(_variant_t((long)50));
	}
	/**************合并第一行单元格*************/ 
	CString str;
	if(!bType) str = "G1";
	else       str = "E1";
	m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A1"),_variant_t(str)),TRUE); 
	m_ExlRge.Merge(_variant_t((long)0)); 
	/********************设置表格内容*******************/
	m_ExlRge.AttachDispatch(m_ExlSheet.GetCells(),TRUE);//加载所有单元格 
	long lNum = 2; 
	try
	{
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance(__uuidof(Recordset)); // 使用ADO创建数据库记录集
		if(!bType)
		{
			m_ExlRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t("报警记录")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)1),_variant_t("编号")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t("报警建筑")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)3),_variant_t("报警楼层")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)4),_variant_t("报警节点")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)5),_variant_t("报警时间")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)6),_variant_t("报警类型")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)7),_variant_t("详细信息"));
			pRecordset->Open("SELECT * FROM AlarmTab",                // 查询报警信息表中所有字段
							theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
			if(!pRecordset->BOF)
				pRecordset->MoveFirst();// 读入库中各字段并加入列表框中
			while(!pRecordset->adoEOF)
			{
				lNum++;
				long lBuf = lNum-2;
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)1),_variant_t(lBuf)); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)2),pRecordset->GetCollect("报警建筑")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)3),pRecordset->GetCollect("报警楼层")); 
				CString strBuf = (char*)(_bstr_t)pRecordset->GetCollect("报警节点");
				if(strBuf.Find('/') != -1)
					strBuf.Replace("/","_");
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)4),_variant_t(strBuf)); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)5),pRecordset->GetCollect("报警时间")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)6),pRecordset->GetCollect("报警类型")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)7),pRecordset->GetCollect("详细信息")); 
				pRecordset->MoveNext();
			}
		}
		else
		{
			m_ExlRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t("操作记录")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)1),_variant_t("编号")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t("操作对象")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)3),_variant_t("操作类型")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)4),_variant_t("操作时间")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)5),_variant_t("操作内容")); 
			pRecordset->Open("SELECT * FROM OperateTab",                // 查询报警信息表中所有字段
						theApp.m_pConnection.GetInterfacePtr(),	 // 获取库接库的IDispatch指针
						adOpenDynamic,
						adLockOptimistic,
						adCmdText);
			if(!pRecordset->BOF)
				pRecordset->MoveFirst();// 读入库中各字段并加入列表框中
			while(!pRecordset->adoEOF)
			{
				lNum++;
				long lBuf = lNum-2;
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)1),_variant_t(lBuf)); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)2),pRecordset->GetCollect("操作对象")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)3),pRecordset->GetCollect("操作类型")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)4),pRecordset->GetCollect("操作时间")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)5),pRecordset->GetCollect("操作内容")); 
				pRecordset->MoveNext();
			} 
		}
		pRecordset->Close();
		pRecordset = NULL;
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	/**************设置单元格格式************/
	m_ExlRge.AttachDispatch(m_ExlSheet.GetUsedRange());//加载已使用的单元格 
	m_ExlRge.SetWrapText(_variant_t((long)1));//设置单元格内的文本为自动换行 
	//********设置齐方式为水平垂直居中********/ 
	//水平对齐：默认＝1,居中＝-4108,左＝-4131,右＝-4152 
	//垂直对齐：默认＝2,居中＝-4108,左＝-4160,右＝-4107 
	m_ExlRge.SetHorizontalAlignment(_variant_t((long)-4108)); 
	m_ExlRge.SetVerticalAlignment(_variant_t((long)-4108)); 
	/********设置整体的字体、字号及颜色********/ 
	Font ft; 
	ft.AttachDispatch(m_ExlRge.GetFont()); 
	ft.SetName(_variant_t("宋体"));//字体 
	ft.SetColorIndex(_variant_t((long)11));//字的颜色 
	ft.SetSize(_variant_t((long)12));//字号 
	/**********设置标题字体及颜色*********/ 
	m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A1"),_variant_t("D1"))); 
	ft.AttachDispatch(m_ExlRge.GetFont()); 
	ft.SetBold(_variant_t((long)1));//粗体 
	ft.SetSize(_variant_t((long)13)); 
	ft.SetColorIndex(_variant_t((long)2)); 
	CellFormat cf; 
	cf.AttachDispatch(m_ExlRge.GetCells()); 
	/**************设置底色**************/ 
	Interior it; 
	it.AttachDispatch(m_ExlRge.GetInterior()); 
	it.SetColorIndex(_variant_t((long)11));//标题底色 
	/***********表格内容的底色***********/ 
	if(!bType) str.Format("G%d",lNum);
	else       str.Format("E%d",lNum);
	m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A2"),_variant_t(str))); 
	it.AttachDispatch(m_ExlRge.GetInterior()); 
	it.SetColorIndex(_variant_t((long)15)); 
	/*************为表格设置边框**********/
	//CString str; 
	Range UnitRge; 
	char cBuf;
	if(!bType) cBuf = 7;
	else       cBuf = 5;
	for(int i=1;i<=lNum-1;i++) 
	{ 
		for(int j=1;j<=cBuf;j++) 
		{     
			str.Format("%c%d",j+64,i);//单元格的名称 
			UnitRge.AttachDispatch(m_ExlRge.GetRange(_variant_t(str),_variant_t(str)));//加载单元格 
			//LineStyle=线型 Weight=线宽 ColorIndex=线的颜色(-4105为自动) 
			UnitRge.BorderAround(_variant_t((long)1),_variant_t((long)2),_variant_t((long)-4105),vtMissing);//设置边框 
		} 
	} 
	/********释放对象（相当重要！）***********/  
	m_ExlRge.ReleaseDispatch(); 
	m_ExlSheet.ReleaseDispatch(); 
	m_ExlSheets.ReleaseDispatch(); 
	m_ExlBook.ReleaseDispatch(); 
	m_ExlBooks.ReleaseDispatch(); 
	//m_ExlApp一定要释放，否则程序结束后还会有一个Excel进程驻留在内存中，而且程序重复运行的时候会出错 
	m_ExlApp.ReleaseDispatch(); 
	m_bChooseDB = FALSE;
	ShowWindow(SW_MINIMIZE);

	//m_ExlApp.Quit();  //退出程序
}
