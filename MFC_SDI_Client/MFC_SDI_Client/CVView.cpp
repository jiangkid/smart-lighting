// CVView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "CVView.h"


// CCVView

IMPLEMENT_DYNCREATE(CCVView, CListView)

CCVView::CCVView()
{

}

CCVView::~CCVView()
{
}

BEGIN_MESSAGE_MAP(CCVView, CListView)
END_MESSAGE_MAP()


// CCVView diagnostics

#ifdef _DEBUG
void CCVView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CCVView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCVView message handlers
