#if !defined(AFX_ONVIEWSTATUSLIST_H__C78D3B96_4C1C_401F_AF68_4D8C6B59A54A__INCLUDED_)
#define AFX_ONVIEWSTATUSLIST_H__C78D3B96_4C1C_401F_AF68_4D8C6B59A54A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OnVIewStatusList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COnVIewStatusList window
struct roadinfo;
class COnVIewStatusList : public CListCtrl
{
// Construction
public:
	COnVIewStatusList();
	CImageList m_imgList;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnVIewStatusList)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddItem(roadinfo* recvinfo);
	virtual ~COnVIewStatusList();
	void Remove(char *roadname);
	// Generated message map functions
protected:
	//{{AFX_MSG(COnVIewStatusList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONVIEWSTATUSLIST_H__C78D3B96_4C1C_401F_AF68_4D8C6B59A54A__INCLUDED_)
