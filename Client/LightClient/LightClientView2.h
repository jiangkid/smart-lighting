#if !defined(AFX_LIGHTCLIENTVIEW2_H__9AE76330_3936_4CF9_A86B_D80D547CDACB__INCLUDED_)
#define AFX_LIGHTCLIENTVIEW2_H__9AE76330_3936_4CF9_A86B_D80D547CDACB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightClientView2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightClientView2 view
class CLightClientDoc;
class CLightClientView2 : public CTreeView
{
protected:
	CLightClientView2();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLightClientView2)

// Attributes
public:
	CLightClientDoc* GetDocument();

// Operations
public:
	void InitTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightClientView2)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLightClientView2();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLightClientView2)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTCLIENTVIEW2_H__9AE76330_3936_4CF9_A86B_D80D547CDACB__INCLUDED_)
