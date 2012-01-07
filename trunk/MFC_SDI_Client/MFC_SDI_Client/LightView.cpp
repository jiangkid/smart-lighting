// LightView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "LightView.h"


// CLightView

IMPLEMENT_DYNCREATE(CLightView, CView)

CLightView::CLightView()
{

}

CLightView::~CLightView()
{
}

BEGIN_MESSAGE_MAP(CLightView, CView)
END_MESSAGE_MAP()


// CLightView drawing

void CLightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CLightView diagnostics

#ifdef _DEBUG
void CLightView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CLightView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLightView message handlers
