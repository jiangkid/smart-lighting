#pragma once
#include "afxcmn.h"
#include "LightView.h"
#include "RoudInfoView.h"
#include "GprsInfoCtrlDlg.h"
// CTabListView form view

class CTabListView : public CFormView
{
	DECLARE_DYNCREATE(CTabListView)

protected:
	CTabListView();           // protected constructor used by dynamic creation
	virtual ~CTabListView();

public:
	enum { IDD = IDD_VIEW_DLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_Tab;
	virtual void OnInitialUpdate();
	void UpDateMainFrame(void);
	CRect m_rect;
	CLightView* m_LightView;
	CGprsInfoCtrlDlg* m_GprsInfoView;
	afx_msg void OnTcnSelchangeViewTab(NMHDR *pNMHDR, LRESULT *pResult);
};


