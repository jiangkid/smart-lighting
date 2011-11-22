// MFC_Client_LightView.h : interface of the CMFC_Client_LightView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFC_CLIENT_LIGHTVIEW_H__39DA4CFD_DB40_4451_A931_876B59B521B2__INCLUDED_)
#define AFX_MFC_CLIENT_LIGHTVIEW_H__39DA4CFD_DB40_4451_A931_876B59B521B2__INCLUDED_

#include "ClientSocket.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ClientSocket.h"
#include "MFC_Client_LightDoc.h"


class CMFC_Client_LightView : public CView
{
protected: // create from serialization only
	CMFC_Client_LightView();
	DECLARE_DYNCREATE(CMFC_Client_LightView)

// Attributes
public:
	CMFC_Client_LightDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMFC_Client_LightView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool chattrue;
	CClientSocket* m_chatsocket;
	CString m_csIP;
	CString m_Sport;
	void OnConnect();
	CHARFORMAT cfm;
	virtual ~CMFC_Client_LightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMFC_Client_LightView)
	afx_msg void OnUpdateConnect(CCmdUI* pCmdUI);
	afx_msg void OnQuit();
	afx_msg void OnUpdateQuit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFC_Client_LightView.cpp
inline CMFC_Client_LightDoc* CMFC_Client_LightView::GetDocument()
   { return (CMFC_Client_LightDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC_CLIENT_LIGHTVIEW_H__39DA4CFD_DB40_4451_A931_876B59B521B2__INCLUDED_)
