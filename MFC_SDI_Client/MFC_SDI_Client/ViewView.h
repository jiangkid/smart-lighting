#pragma once
#include "afxext.h"


// CViewView view

class CViewView : public CView
{
	DECLARE_DYNCREATE(CViewView)

protected:
	CViewView();           // protected constructor used by dynamic creation
	virtual ~CViewView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CSplitterWnd m_wndSplitter;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


