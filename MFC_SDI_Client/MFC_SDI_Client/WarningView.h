#pragma once


// CWarningView view

class CWarningView : public CListView
{
	DECLARE_DYNCREATE(CWarningView)

protected:
	CWarningView();           // protected constructor used by dynamic creation
	virtual ~CWarningView();

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
	virtual void OnInitialUpdate();
};


