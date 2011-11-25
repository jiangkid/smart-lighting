#if !defined(AFX_LIGHTVIEW_H__76C8F620_2035_439B_BEEF_59313AE8761D__INCLUDED_)
#define AFX_LIGHTVIEW_H__76C8F620_2035_439B_BEEF_59313AE8761D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LightView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLightView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "BtnST.h"
class CLightView : public CFormView
{
protected:
	CLightView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLightView)

// Form Data
public:
	//{{AFX_DATA(CLightView)
	enum { IDD = IDD_DIALOG3 };
	CButtonST	m_btnID33;
	CButtonST	m_btnID22;
	CButtonST	m_btnID11;
	CButtonST	m_btnID1;
	CButtonST	m_btnID3;
	CButtonST	m_btnID2;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL m_id3true;
	BOOL m_id2true;
	BOOL m_id1true;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLightView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLightView)
	afx_msg void OnBtnLight1();
	afx_msg void OnBtnLight2();
	afx_msg void OnBtnLight3();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIGHTVIEW_H__76C8F620_2035_439B_BEEF_59313AE8761D__INCLUDED_)
