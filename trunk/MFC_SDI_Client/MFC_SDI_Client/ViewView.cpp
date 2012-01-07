// ViewView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "ViewView.h"
#include "CVView.h"
#include "WarningView.h"

// CViewView

IMPLEMENT_DYNCREATE(CViewView, CView)

CViewView::CViewView()
{

}

CViewView::~CViewView()
{
}

BEGIN_MESSAGE_MAP(CViewView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CViewView drawing

void CViewView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CViewView diagnostics

#ifdef _DEBUG
void CViewView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewView message handlers

int CViewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	CCreateContext *pContext = (CCreateContext *)lpCreateStruct->lpCreateParams;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CCVView), CSize(100, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CWarningView), CSize(0, 0), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

void CViewView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	int width = cx/7;
	m_wndSplitter.MoveWindow(-2, -2, cx, cy+3);
	m_wndSplitter.SetColumnInfo(0, cx-width*2, 0);
	m_wndSplitter.SetColumnInfo(1, width*2, 0);
	m_wndSplitter.RecalcLayout();
}
