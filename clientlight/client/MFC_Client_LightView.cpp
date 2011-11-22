// MFC_Client_LightView.cpp : implementation of the CMFC_Client_LightView class
//

#include "stdafx.h"
#include "MFC_Client_Light.h"
#include "MainFrm.h"
#include "MFC_Client_LightDoc.h"
#include "MFC_Client_LightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightView

IMPLEMENT_DYNCREATE(CMFC_Client_LightView, CView)

BEGIN_MESSAGE_MAP(CMFC_Client_LightView, CView)
	//{{AFX_MSG_MAP(CMFC_Client_LightView)
	ON_UPDATE_COMMAND_UI(ID_CONNECT, OnUpdateConnect)
	ON_COMMAND(ID_QUIT, OnQuit)
	ON_COMMAND(ID_CONNECT, OnConnect)
	ON_UPDATE_COMMAND_UI(ID_QUIT, OnUpdateQuit)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightView construction/destruction

CMFC_Client_LightView::CMFC_Client_LightView()
{
	// TODO: add construction code here
	chattrue=TRUE;
}

CMFC_Client_LightView::~CMFC_Client_LightView()
{
}

BOOL CMFC_Client_LightView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightView drawing

void CMFC_Client_LightView::OnDraw(CDC* pDC)
{
	CMFC_Client_LightDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightView printing

BOOL CMFC_Client_LightView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFC_Client_LightView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFC_Client_LightView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightView diagnostics

#ifdef _DEBUG
void CMFC_Client_LightView::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_Client_LightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_Client_LightDoc* CMFC_Client_LightView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_Client_LightDoc)));
	return (CMFC_Client_LightDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMFC_Client_LightView message handlers

void CMFC_Client_LightView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
//	CRichEditView::OnInitialUpdate();
	
	//add begin
	cfm.cbSize=sizeof(cfm);
	cfm.bCharSet=GB2312_CHARSET;
	cfm.crTextColor=RGB(0,0,0);
	cfm.dwMask=CFM_CHARSET | CFM_COLOR ;
	cfm.dwEffects=0;
	//CRichEditView::GetRichEditCtrl  Call this function to retrieve the CRichEditCtrl object associated with the CRichEditView object.
	//�˺�����ԭ���ǣ�CRichEditCtrl& GetRichEditCtrl( ) const; �ҶԴ˺����Ƚ���Ϥ����΢���WordPad�����о����õ��˺�����
	//CRichEditCtrl::SetDefaultCharFormat  Sets the character formatting attributes for new text in this CRichEditCtrl object.
	//GetRichEditCtrl().SetDefaultCharFormat(cfm);
	//add end

	// Set the printing margins (720 twips = 1/2 inch).
	//SetMargins(CRect(720, 720, 720, 720));
	// TODO: Add your specialized code here and/or call the base class
	
}

void CMFC_Client_LightView::OnConnect()
{
	int m_sport_ci;
	CMainFrame* pWnd=(CMainFrame*)AfxGetMainWnd();
	CMFC_Client_LightApp* pApp=(CMFC_Client_LightApp*)AfxGetApp();
	pWnd->m_ip1.GetWindowText(m_csIP);
	pWnd->m_port.GetWindowText(m_Sport);
	m_sport_ci=atoi(m_Sport);
	if(chattrue)
		{
			m_chatsocket=new CClientSocket(this);
			m_chatsocket->Create();
			if(!m_chatsocket->Connect(m_csIP,m_sport_ci))
			{
				//m_chatsocket.Init(this);
				//SetDlgItemText(IDC_SHOWMESSAGE,"�ͻ������ӳɹ�!");
				//m_bInit=true;
				AfxMessageBox("�ͻ�������ʧ��!");
				m_chatsocket->Close();
				chattrue=true;
			}
			else
			{
				//m_chatsocket->Close();
				AfxMessageBox("�ͻ������ӳɹ�!");
				chattrue=FALSE;
			}
		
			//chattrue=FALSE;//��Ϊfalse����ʾ����socket�Ѿ����Ӻ��ˡ�
		}

}

void CMFC_Client_LightView::OnUpdateConnect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(chattrue)
	{
		pCmdUI->Enable(TRUE);
		//chattrue=false;
	}
	else
	{
		pCmdUI->Enable(false);
		//chattrue=true;
	}

}

void CMFC_Client_LightView::OnQuit() 
{
	// TODO: Add your command handler code here
	m_chatsocket->Close();
	chattrue=true;
}

void CMFC_Client_LightView::OnUpdateQuit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}
