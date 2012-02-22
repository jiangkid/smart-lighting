#pragma once


// CLightListView view

class CLightListView : public CListView
{
	DECLARE_DYNCREATE(CLightListView)

protected:
	CLightListView();           // protected constructor used by dynamic creation
	virtual ~CLightListView();

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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	void DeleteAll(void);
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32799();
	void LightToView(int nLCount);
};


