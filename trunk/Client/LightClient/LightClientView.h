// LightClientView.h : interface of the CLightClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTCLIENTVIEW_H__8F25ECCD_73C7_4EB3_90B5_F5B1A8AD0354__INCLUDED_)
#define AFX_LIGHTCLIENTVIEW_H__8F25ECCD_73C7_4EB3_90B5_F5B1A8AD0354__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLightClientDoc;
class CLightClientView : public CView
{
protected: // create from serialization only
	CLightClientView();
	DECLARE_DYNCREATE(CLightClientView)

// Attributes
public:
	CLightClientDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightClientView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLightClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLightClientView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LightClientView.cpp
inline CLightClientDoc* CLightClientView::GetDocument()
   { return (CLightClientDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTCLIENTVIEW_H__8F25ECCD_73C7_4EB3_90B5_F5B1A8AD0354__INCLUDED_)
