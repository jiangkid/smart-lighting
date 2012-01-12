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

void CWarningView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl &listCtrl=(CListCtrl&)GetListCtrl();
	listCtrl.ModifyStyle (0,LVS_REPORT);
	listCtrl.ModifyStyle (LVS_EDITLABELS,0);//禁止修改列标题
	listCtrl.SetExtendedStyle (LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_INFOTIP|LVS_EX_SUBITEMIMAGES|LVS_EX_GRIDLINES);
	listCtrl.InsertColumn (0,"时间",LVCFMT_LEFT,50);
	listCtrl.InsertColumn (1,"报警信息",LVCFMT_LEFT,100);
	// TODO: Add your specialized code here and/or call the base class
}
