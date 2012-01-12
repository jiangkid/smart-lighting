#pragma once
#include "BtnST.h"
#include "BCMenu.h"

// CLightView view

class CLightView : public CView
{
	DECLARE_DYNCREATE(CLightView)

protected:
	CLightView();           // protected constructor used by dynamic creation
	virtual ~CLightView();

public:
	CButtonST *p_MyBut[9];
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
	afx_msg void OnMyButton1();
	afx_msg void OnMyButton2();
	afx_msg void OnMyButton3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void OnLightShow();
	CButtonST* NewMyButton(int nID, CRect rect , int nStyle);
	void ChangeButtonOn(void);
	void ChangeSecondButtonOn(void);
	void ChangeDoubleOn(void);
	void ChangeButtonOff(void);
	void ChangeSecondButtonOff(void);
	void ChangeDoubleOff(void);

};


