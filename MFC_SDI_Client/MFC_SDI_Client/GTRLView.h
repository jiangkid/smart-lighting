#pragma once


// CGTRLView view

class CGTRLView : public CTreeView
{
	DECLARE_DYNCREATE(CGTRLView)

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
};


