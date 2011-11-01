// LightClientView.cpp : implementation of the CLightClientView class
//

#include "stdafx.h"
#include "LightClient.h"

#include "LightClientDoc.h"
#include "LightClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightClientView

IMPLEMENT_DYNCREATE(CLightClientView, CView)

BEGIN_MESSAGE_MAP(CLightClientView, CView)
	//{{AFX_MSG_MAP(CLightClientView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightClientView construction/destruction

CLightClientView::CLightClientView()
{
	// TODO: add construction code here

}

CLightClientView::~CLightClientView()
{
}

BOOL CLightClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLightClientView drawing

void CLightClientView::OnDraw(CDC* pDC)
{
	CLightClientDoc* pDoc = (CLightClientDoc*)CLightClientView::GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLightClientView printing

BOOL CLightClientView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLightClientView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLightClientView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CLightClientView diagnostics

#ifdef _DEBUG
void CLightClientView::AssertValid() const
{
	CView::AssertValid();
}

void CLightClientView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLightClientDoc* CLightClientView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLightClientDoc)));
	return (CLightClientDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLightClientView message handlers
