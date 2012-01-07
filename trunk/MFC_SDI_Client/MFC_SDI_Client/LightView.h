#pragma once


// CLightView view

class CLightView : public CView
{
	DECLARE_DYNCREATE(CLightView)

protected:
	CLightView();           // protected constructor used by dynamic creation
	virtual ~CLightView();

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
};


