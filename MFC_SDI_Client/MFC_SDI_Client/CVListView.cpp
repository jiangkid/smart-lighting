// CVListView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "CVListView.h"


// CCVListView

IMPLEMENT_DYNCREATE(CCVListView, CListView)

CCVListView::CCVListView()
{

}

CCVListView::~CCVListView()
{
}

BEGIN_MESSAGE_MAP(CCVListView, CListView)
END_MESSAGE_MAP()


// CCVListView diagnostics

#ifdef _DEBUG
void CCVListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CCVListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCVListView message handlers

BOOL CCVListView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	theApp.m_pCVListView=this;
	return CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
