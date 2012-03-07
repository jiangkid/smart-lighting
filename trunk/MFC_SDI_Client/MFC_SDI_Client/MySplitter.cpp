#include "StdAfx.h"
#include "MySplitter.h"

CMySplitter::CMySplitter(void)
{
}

CMySplitter::~CMySplitter(void)
{
}
BEGIN_MESSAGE_MAP(CMySplitter, CSplitterWnd)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

LRESULT CMySplitter::OnNcHitTest(CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	return HTNOWHERE;
}
