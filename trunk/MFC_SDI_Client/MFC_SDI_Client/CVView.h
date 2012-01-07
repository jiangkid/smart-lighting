#pragma once


// CCVView view

class CCVView : public CListView
{
	DECLARE_DYNCREATE(CCVView)

protected:
	CCVView();           // protected constructor used by dynamic creation
	virtual ~CCVView();

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


