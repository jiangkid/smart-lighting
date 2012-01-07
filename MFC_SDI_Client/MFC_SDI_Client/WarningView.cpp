// WarningView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "WarningView.h"


// CWarningView

IMPLEMENT_DYNCREATE(CWarningView, CListView)

CWarningView::CWarningView()
{

}

CWarningView::~CWarningView()
{
}

BEGIN_MESSAGE_MAP(CWarningView, CListView)
END_MESSAGE_MAP()


// CWarningView diagnostics

#ifdef _DEBUG
void CWarningView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CWarningView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWarningView message handlers
