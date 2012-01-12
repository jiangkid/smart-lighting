#pragma once
#include "LightView.h"
#include "WaitDlg.h"
// CGTRLView view

class CGTRLView : public CTreeView
{
	DECLARE_DYNCREATE(CGTRLView)
public:
	CImageList list;

protected:
	CGTRLView();           // protected constructor used by dynamic creation
	virtual ~CGTRLView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	int GetTNum(LPGInfo wGtInfo);
	virtual void OnInitialUpdate();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	CLightView* m_lightView;
	CWaitDlg   m_WaitDlg;
	//afx_msg void OnTvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
};


