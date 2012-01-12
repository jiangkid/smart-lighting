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

void CCVView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl &listCtrl=(CListCtrl&)GetListCtrl();
	listCtrl.ModifyStyle (0,LVS_REPORT);
	listCtrl.ModifyStyle (LVS_EDITLABELS,0);//禁止修改列标题
	listCtrl.SetExtendedStyle (LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_INFOTIP|LVS_EX_SUBITEMIMAGES|LVS_EX_GRIDLINES);
	listCtrl.InsertColumn (0,"终端",LVCFMT_LEFT,50);
	listCtrl.InsertColumn (1,"电压",LVCFMT_LEFT,100);
	listCtrl.InsertColumn (2,"电流",LVCFMT_LEFT,150);
	// TODO: Add your specialized code here and/or call the base class
}
