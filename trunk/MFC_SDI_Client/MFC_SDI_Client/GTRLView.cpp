// GTRLView.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_SDI_Client.h"
#include "GTRLView.h"


// CGTRLView

IMPLEMENT_DYNCREATE(CGTRLView, CTreeView)

CGTRLView::CGTRLView()
{

}

CGTRLView::~CGTRLView()
{
}

BEGIN_MESSAGE_MAP(CGTRLView, CTreeView)
END_MESSAGE_MAP()


// CGTRLView diagnostics

#ifdef _DEBUG
void CGTRLView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CGTRLView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGTRLView message handlers

int CGTRLView::GetTNum(LPGInfo wGtInfo)
{
	int m_GTnum(0);
	for (int i=0;i<m_InitInfo.TNum;i++)
	{
		if (m_InitInfo.m_InitTInfo[i].GID==wGtInfo->GID)
		{
			m_GTnum++;
		}
	}
	return m_GTnum;
}
