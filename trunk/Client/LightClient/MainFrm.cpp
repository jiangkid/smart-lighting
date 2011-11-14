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
				_T("������"), CSize(200,200)))
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
				_T("�����"), CSize(200,195)))
			{
				TRACE0("Failed to create dialog bar m_wndOutput\n");
				return -1;		// fail to create
			}
			
			// Add the views to the tab control.
			m_wndOutput.AddView(_T("���������¼"), RUNTIME_CLASS(CLVErrorRecord));
			m_wndOutput.AddView(_T("���������¼"), RUNTIME_CLASS(CListOperatorRecord));
			
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
				_T("��ʾ��"), CSize(GetSystemMetrics(SM_CXSCREEN)-491,200)))
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
				_T("��Ϣ��"), CSize(300,200)))
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
	
	//��õ�ǰ��ͼ
	CLightClientView* pView = (CLightClientView*)pChildFrame->GetActiveView();
	if (pView == NULL)
	{
		return;
	}
	
	CRect rect;
	pView->GetClientRect(rect);
	CString strText = _T("");
	strText.Format(_T("�ڿ���л�õ�ǰ��ͼָ�룺0x%08X"), pView);
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
	/*******************����EXCEL���******************/
	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if(!m_ExlApp.CreateDispatch("Excel.Application",NULL)) //��m_ExlApp���󴴽�Excel2003����
	{ 
		AfxMessageBox("����Excel����ʧ��!"); 
		m_bChooseDB = FALSE;
		return; 
	} 
	m_ExlApp.SetVisible(TRUE); //����Ϊ�ɼ�  
	m_ExlBooks.AttachDispatch(m_ExlApp.GetWorkbooks(),TRUE); 
	m_ExlBook.AttachDispatch(m_ExlBooks.Add(covOptional),TRUE);//����EXCELģ�� 
	m_ExlSheets.AttachDispatch(m_ExlBook.GetSheets(),TRUE);//����Sheetҳ�� 
	/*******************����EXCEL���******************/
	m_ExlSheet.AttachDispatch(m_ExlSheets.GetItem(_variant_t((long)1)),TRUE); 
	m_ExlSheet.SetName("AlarmReportSheet"); 
	/***************�ı䵥Ԫ����**************/
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
	/**************�ϲ���һ�е�Ԫ��*************/ 
	CString str;
	if(!bType) str = "G1";
	else       str = "E1";
	m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A1"),_variant_t(str)),TRUE); 
	m_ExlRge.Merge(_variant_t((long)0)); 
	/********************���ñ������*******************/
	m_ExlRge.AttachDispatch(m_ExlSheet.GetCells(),TRUE);//�������е�Ԫ�� 
	long lNum = 2; 
	try
	{
		_RecordsetPtr pRecordset;
		pRecordset.CreateInstance(__uuidof(Recordset)); // ʹ��ADO�������ݿ��¼��
		if(!bType)
		{
			m_ExlRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t("������¼")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)1),_variant_t("���")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t("��������")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)3),_variant_t("����¥��")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)4),_variant_t("�����ڵ�")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)5),_variant_t("����ʱ��")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)6),_variant_t("��������")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)7),_variant_t("��ϸ��Ϣ"));
			pRecordset->Open("SELECT * FROM AlarmTab",                // ��ѯ������Ϣ���������ֶ�
							theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
							adOpenDynamic,
							adLockOptimistic,
							adCmdText);
			if(!pRecordset->BOF)
				pRecordset->MoveFirst();// ������и��ֶβ������б����
			while(!pRecordset->adoEOF)
			{
				lNum++;
				long lBuf = lNum-2;
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)1),_variant_t(lBuf)); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)2),pRecordset->GetCollect("��������")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)3),pRecordset->GetCollect("����¥��")); 
				CString strBuf = (char*)(_bstr_t)pRecordset->GetCollect("�����ڵ�");
				if(strBuf.Find('/') != -1)
					strBuf.Replace("/","_");
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)4),_variant_t(strBuf)); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)5),pRecordset->GetCollect("����ʱ��")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)6),pRecordset->GetCollect("��������")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)7),pRecordset->GetCollect("��ϸ��Ϣ")); 
				pRecordset->MoveNext();
			}
		}
		else
		{
			m_ExlRge.SetItem(_variant_t((long)1),_variant_t((long)1),_variant_t("������¼")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)1),_variant_t("���")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)2),_variant_t("��������")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)3),_variant_t("��������")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)4),_variant_t("����ʱ��")); 
			m_ExlRge.SetItem(_variant_t((long)2),_variant_t((long)5),_variant_t("��������")); 
			pRecordset->Open("SELECT * FROM OperateTab",                // ��ѯ������Ϣ���������ֶ�
						theApp.m_pConnection.GetInterfacePtr(),	 // ��ȡ��ӿ��IDispatchָ��
						adOpenDynamic,
						adLockOptimistic,
						adCmdText);
			if(!pRecordset->BOF)
				pRecordset->MoveFirst();// ������и��ֶβ������б����
			while(!pRecordset->adoEOF)
			{
				lNum++;
				long lBuf = lNum-2;
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)1),_variant_t(lBuf)); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)2),pRecordset->GetCollect("��������")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)3),pRecordset->GetCollect("��������")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)4),pRecordset->GetCollect("����ʱ��")); 
				m_ExlRge.SetItem(_variant_t(lNum),_variant_t((long)5),pRecordset->GetCollect("��������")); 
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
	/**************���õ�Ԫ���ʽ************/
	m_ExlRge.AttachDispatch(m_ExlSheet.GetUsedRange());//������ʹ�õĵ�Ԫ�� 
	m_ExlRge.SetWrapText(_variant_t((long)1));//���õ�Ԫ���ڵ��ı�Ϊ�Զ����� 
	//********�����뷽ʽΪˮƽ��ֱ����********/ 
	//ˮƽ���룺Ĭ�ϣ�1,���У�-4108,��-4131,�ң�-4152 
	//��ֱ���룺Ĭ�ϣ�2,���У�-4108,��-4160,�ң�-4107 
	m_ExlRge.SetHorizontalAlignment(_variant_t((long)-4108)); 
	m_ExlRge.SetVerticalAlignment(_variant_t((long)-4108)); 
	/********������������塢�ֺż���ɫ********/ 
	Font ft; 
	ft.AttachDispatch(m_ExlRge.GetFont()); 
	ft.SetName(_variant_t("����"));//���� 
	ft.SetColorIndex(_variant_t((long)11));//�ֵ���ɫ 
	ft.SetSize(_variant_t((long)12));//�ֺ� 
	/**********���ñ������弰��ɫ*********/ 
	m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A1"),_variant_t("D1"))); 
	ft.AttachDispatch(m_ExlRge.GetFont()); 
	ft.SetBold(_variant_t((long)1));//���� 
	ft.SetSize(_variant_t((long)13)); 
	ft.SetColorIndex(_variant_t((long)2)); 
	CellFormat cf; 
	cf.AttachDispatch(m_ExlRge.GetCells()); 
	/**************���õ�ɫ**************/ 
	Interior it; 
	it.AttachDispatch(m_ExlRge.GetInterior()); 
	it.SetColorIndex(_variant_t((long)11));//�����ɫ 
	/***********������ݵĵ�ɫ***********/ 
	if(!bType) str.Format("G%d",lNum);
	else       str.Format("E%d",lNum);
	m_ExlRge.AttachDispatch(m_ExlSheet.GetRange(_variant_t("A2"),_variant_t(str))); 
	it.AttachDispatch(m_ExlRge.GetInterior()); 
	it.SetColorIndex(_variant_t((long)15)); 
	/*************Ϊ������ñ߿�**********/
	//CString str; 
	Range UnitRge; 
	char cBuf;
	if(!bType) cBuf = 7;
	else       cBuf = 5;
	for(int i=1;i<=lNum-1;i++) 
	{ 
		for(int j=1;j<=cBuf;j++) 
		{     
			str.Format("%c%d",j+64,i);//��Ԫ������� 
			UnitRge.AttachDispatch(m_ExlRge.GetRange(_variant_t(str),_variant_t(str)));//���ص�Ԫ�� 
			//LineStyle=���� Weight=�߿� ColorIndex=�ߵ���ɫ(-4105Ϊ�Զ�) 
			UnitRge.BorderAround(_variant_t((long)1),_variant_t((long)2),_variant_t((long)-4105),vtMissing);//���ñ߿� 
		} 
	} 
	/********�ͷŶ����൱��Ҫ����***********/  
	m_ExlRge.ReleaseDispatch(); 
	m_ExlSheet.ReleaseDispatch(); 
	m_ExlSheets.ReleaseDispatch(); 
	m_ExlBook.ReleaseDispatch(); 
	m_ExlBooks.ReleaseDispatch(); 
	//m_ExlAppһ��Ҫ�ͷţ������������󻹻���һ��Excel����פ�����ڴ��У����ҳ����ظ����е�ʱ������ 
	m_ExlApp.ReleaseDispatch(); 
	m_bChooseDB = FALSE;
	ShowWindow(SW_MINIMIZE);

	//m_ExlApp.Quit();  //�˳�����
}
